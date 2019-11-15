/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commons.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vifonne <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/15 11:35:08 by vifonne           #+#    #+#             */
/*   Updated: 2019/11/15 14:14:43 by vifonne          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl_md5.h"

void	read_from_fd(int fd, t_msg *msg, t_functions *fct_table, t_options opt)
{
	size_t	ret;
	char	read_buffer[READ_BUFF_SIZE];

	while ((ret = read(fd, read_buffer, READ_BUFF_SIZE)) > 0)
	{
		if (opt.p == 1)
			write(1, read_buffer, ret);
		msg->original_len += ret;
		fct_table->string((uint8_t *)read_buffer, ret, msg, fct_table);
	}
	if (opt.p == 1)
		ft_putchar('\n');
	fct_table->preparation(msg, fct_table);
}

ssize_t	basic_string(uint8_t *str, ssize_t length, t_msg *msg, t_functions *fct_table)
{
	int cpy_len;
	cpy_len = MD5_BUFF_SIZE - msg->internal_buffer_len;
	if (length < cpy_len)
		cpy_len = length;
	ft_memcpy(msg->internal_buffer + msg->internal_buffer_len, str, cpy_len);
	msg->internal_buffer_len += cpy_len;
	length -= cpy_len;
	if (msg->internal_buffer_len == MD5_BUFF_SIZE)
	{
		fct_table->loop((uint32_t *)msg->internal_buffer, msg, fct_table);
		msg->internal_buffer_len = 0;
	}
	return (length);
}

void	init_md_buffer(t_msg *msg)
{
	if (msg->algo_choosen == 0)
	{
		msg->md_buffer.h[0] = 0x67452301;
		msg->md_buffer.h[1] = 0xEFCDAB89;
		msg->md_buffer.h[2] = 0x98BADCFE;
		msg->md_buffer.h[3] = 0x10325476;
	}
	else if (msg->algo_choosen == 1)
	{
		msg->md_buffer.h[0] = 0x6a09e667;
		msg->md_buffer.h[1] = 0xbb67ae85;
		msg->md_buffer.h[2] = 0x3c6ef372;
		msg->md_buffer.h[3] = 0xa54ff53a;
		msg->md_buffer.h[4] = 0x510e527f;
		msg->md_buffer.h[5] = 0x9b05688c;
		msg->md_buffer.h[6] = 0x1f83d9ab;
		msg->md_buffer.h[7] = 0x5be0cd19;
	}
}

void	init_hash(t_msg *msg)
{
	msg->hash.a = msg->md_buffer.h[0];
	msg->hash.b = msg->md_buffer.h[1];
	msg->hash.c = msg->md_buffer.h[2];
	msg->hash.d = msg->md_buffer.h[3];
	if (msg->algo_choosen == 1)
	{
		msg->hash.e = msg->md_buffer.h[4];
		msg->hash.f = msg->md_buffer.h[5];
		msg->hash.g = msg->md_buffer.h[6];
		msg->hash.h = msg->md_buffer.h[7];
	}
}

void	add_hash(t_msg *msg)
{
	msg->md_buffer.h[0] += msg->hash.a;
	msg->md_buffer.h[1] += msg->hash.b;
	msg->md_buffer.h[2] += msg->hash.c;
	msg->md_buffer.h[3] += msg->hash.d;
	if (msg->algo_choosen == 1)
	{
		msg->md_buffer.h[4] += msg->hash.e;
		msg->md_buffer.h[5] += msg->hash.f;
		msg->md_buffer.h[6] += msg->hash.g;
		msg->md_buffer.h[7] += msg->hash.h;
	}
}

void	preparation(t_msg *msg, t_functions *fct_table)
{
	*(msg->internal_buffer + msg->internal_buffer_len) = 1 << 7;
	msg->internal_buffer_len += 1;
	ft_bzero(msg->internal_buffer + msg->internal_buffer_len,
			MD5_BUFF_SIZE - msg->internal_buffer_len);
	if (msg->internal_buffer_len > 56)
	{
		fct_table->loop((uint32_t *)msg->internal_buffer, msg, fct_table);
		msg->internal_buffer_len = 0;
		ft_bzero(msg->internal_buffer, MD5_BUFF_SIZE);
	}
	if (msg->algo_choosen == 1)
	{
	*(uint64_t *)(msg->internal_buffer + (MD5_BUFF_SIZE - sizeof(uint64_t))) =
		bswap_64((uint64_t)(msg->original_len * 8));
	}
	else if (msg->algo_choosen == 0)
	{
		*(uint64_t *)(msg->internal_buffer + (MD5_BUFF_SIZE - sizeof(uint64_t))) =
		(uint64_t)(msg->original_len * 8);
	}
	fct_table->loop((uint32_t *)msg->internal_buffer, msg, fct_table);
}

void	fill_algo_name(t_msg *msg)
{
	if (msg->algo_choosen == 0)
		msg->algo_name = "MD5";
	else if (msg->algo_choosen == 1)
		msg->algo_name = "SHA256";
}

int		hash_main(char *str, t_functions *fct_table, t_options opt, int algo_choosen)
{
	int		fd;
	t_msg	*msg;

	if (!(msg = (t_msg *)ft_memalloc(sizeof(t_msg))))
		return (0);
	msg->algo_choosen = algo_choosen;
	fill_algo_name(msg);
	msg->filename = str;
	fct_table->init_md_buffer(msg);
	if (opt.s == 1)
	{
		msg->original_len += ft_strlen(str);
		fct_table->string((uint8_t *)str, (ssize_t)msg->original_len, msg, fct_table);
		fct_table->preparation(msg, fct_table);
	}
	else
	{
		if (str == NULL)
		{
			fd = 0;
		}
		else
		{
			fd = open(str, O_RDONLY);
			if (fd < 0)
			{
				free(msg);
				return (0);
			}
		}
		fct_table->read_from_fd(fd, msg, fct_table, opt);
		close(fd);
	}
	print_output(msg, opt);
	free(msg);
	return (1);
}
