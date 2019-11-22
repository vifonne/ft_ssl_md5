/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commons.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vifonne <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/15 11:35:08 by vifonne           #+#    #+#             */
/*   Updated: 2019/11/22 16:22:11 by vifonne          ###   ########.fr       */
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
	fct_table->preparation(msg, fct_table);
}

ssize_t	basic_string(uint8_t *str, ssize_t length, t_msg *msg
			, t_functions *fct_table)
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
		*(uint64_t *)(msg->internal_buffer + (MD5_BUFF_SIZE - 8)) =
			bswap_64((uint64_t)(msg->original_len * 8));
	}
	else if (msg->algo_choosen == 0)
	{
		*(uint64_t *)(msg->internal_buffer + (MD5_BUFF_SIZE - 8)) =
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

int		hash_main(char *str, t_functions *fct_table, t_options opt
			, int algo_choosen)
{
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
		fct_table->string((uint8_t *)str, (ssize_t)msg->original_len, msg
			, fct_table);
		fct_table->preparation(msg, fct_table);
	}
	else
	{
		if (!launch_file_stdin(msg, str, fct_table, opt))
			return (0);
	}
	print_output(msg, opt);
	free(msg);
	return (1);
}
