/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commons_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vifonne <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/15 15:15:59 by vifonne           #+#    #+#             */
/*   Updated: 2019/11/22 15:07:19 by vifonne          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl_md5.h"

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

int		launch_file_stdin(t_msg *msg, char *str, t_functions *fct_table
			, t_options opt)
{
	int fd;

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
			ft_error(str, -2);
			return (0);
		}
	}
	fct_table->read_from_fd(fd, msg, fct_table, opt);
	close(fd);
	return (1);
}
