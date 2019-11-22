/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vifonne <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/10 11:48:45 by vifonne           #+#    #+#             */
/*   Updated: 2019/11/22 11:45:07 by vifonne          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl_md5.h"

t_functions	set_fct_table(int algo_choosen)
{
	t_functions		fct_table;

	fct_table.hash_main = hash_main;
	fct_table.basic_string = basic_string;
	fct_table.read_from_fd = read_from_fd;
	fct_table.init_md_buffer = init_md_buffer;
	fct_table.init_hash = init_hash;
	fct_table.add_hash = add_hash;
	fct_table.preparation = preparation;
	if (algo_choosen == 0)
	{
		fct_table.string = md5_string;
		fct_table.loop = md5_loop;
	}
	else
	{
		fct_table.string = sha256_string;
		fct_table.loop = sha256_loop;
	}
	return (fct_table);
}

void		print_output_full(t_msg *msg, int s, int r)
{
	if (r == 0)
	{
		ft_putstr(msg->algo_name);
		ft_putstr(" (");
		if (s == 1)
			ft_putchar('"');
		ft_putstr(msg->filename);
		if (s == 1)
			ft_putchar('"');
		ft_putstr(") = ");
	}
	print_byte(msg);
	if (r == 1)
	{
		ft_putchar(' ');
		if (s == 1)
			ft_putchar('"');
		ft_putstr(msg->filename);
		if (s == 1)
			ft_putchar('"');
	}
}

void		print_output(t_msg *msg, t_options opt)
{
	if (opt.q == 1)
		print_byte(msg);
	else
		print_output_full(msg, opt.s, opt.r);
	ft_putchar('\n');
}
