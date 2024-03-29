/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha256.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vifonne <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/13 16:02:10 by vifonne           #+#    #+#             */
/*   Updated: 2019/11/15 15:46:58 by vifonne          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl_md5.h"

uint32_t	g_sha256_sintab[64] = {0x428a2f98, 0x71374491, 0xb5c0fbcf,
	0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
	0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74,
	0x80deb1fe, 0x9bdc06a7, 0xc19bf174, 0xe49b69c1, 0xefbe4786,
	0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc,
	0x76f988da, 0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
	0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967, 0x27b70a85,
	0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb,
	0x81c2c92e, 0x92722c85, 0xa2bfe8a1, 0xa81a664b, 0xc24b8b70,
	0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3,
	0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3, 0x748f82ee, 0x78a5636f,
	0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7,
	0xc67178f2};

void		sha256_word_extend(uint32_t *buffer)
{
	size_t		idx;
	uint32_t	ss0;
	uint32_t	ss1;

	idx = 0;
	while (idx < 16)
	{
		buffer[idx] = bswap_32(buffer[idx]);
		idx++;
	}
	while (idx < 64)
	{
		ss0 = ROTATE_RIGHT(buffer[idx - 15], 7)
			^ ROTATE_RIGHT(buffer[idx - 15], 18)
			^ (buffer[idx - 15] >> 3);
		ss1 = ROTATE_RIGHT(buffer[idx - 2], 17)
			^ ROTATE_RIGHT(buffer[idx - 2], 19)
			^ (buffer[idx - 2] >> 10);
		buffer[idx] = buffer[idx - 16] + ss0 + buffer[idx - 7] + ss1;
		idx++;
	}
}

void		sha256_loop(uint32_t *buffer, t_msg *msg, t_functions *fct_table)
{
	uint32_t	tmp1;
	uint32_t	tmp2;
	size_t		idx;

	idx = 0;
	sha256_word_extend(buffer);
	fct_table->init_hash(msg);
	while (idx < 64)
	{
		tmp1 = msg->hash.h + S1(msg->hash.e)
			+ CH(msg->hash.e, msg->hash.f, msg->hash.g)
			+ g_sha256_sintab[idx] + buffer[idx];
		tmp2 = S0(msg->hash.a) + MAJ(msg->hash.a, msg->hash.b, msg->hash.c);
		msg->hash.h = msg->hash.g;
		msg->hash.g = msg->hash.f;
		msg->hash.f = msg->hash.e;
		msg->hash.e = msg->hash.d + tmp1;
		msg->hash.d = msg->hash.c;
		msg->hash.c = msg->hash.b;
		msg->hash.b = msg->hash.a;
		msg->hash.a = tmp1 + tmp2;
		idx++;
	}
	fct_table->add_hash(msg);
}

void		sha256_string(uint8_t *str, ssize_t length, t_msg *msg
				, t_functions *fct_table)
{
	uint32_t	sha_buff[MD5_BUFF_SIZE];

	if (msg->internal_buffer_len > 0)
	{
		length = fct_table->basic_string(str, length, msg, fct_table);
	}
	while (length >= 64)
	{
		ft_memcpy(sha_buff, str, 64);
		fct_table->loop(sha_buff, msg, fct_table);
		str += 64;
		length -= 64;
	}
	if (length > 0)
	{
		ft_memcpy(msg->internal_buffer, str, length);
		msg->internal_buffer_len = length;
	}
}
