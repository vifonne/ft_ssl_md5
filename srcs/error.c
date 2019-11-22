/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vifonne <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/13 15:57:55 by vifonne           #+#    #+#             */
/*   Updated: 2019/11/22 15:28:59 by vifonne          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl_md5.h"

void	ft_usage(void)
{
	ft_putstr_fd("ft_ssl <algo_name> [-pqr] [-s string] [files ...]\n", 2);
}

void	ft_error(char *str, int error_code)
{
	ft_putstr_fd("ft_ssl: ", 2);
	if (error_code == -2)
	{
		ft_putstr_fd(str, 2);
		ft_putstr_fd(": no such file or directory.\n", 2);
	}
	else if (error_code == -3)
	{
		ft_putstr_fd("illegal option\n", 2);
		ft_usage();
	}
	else if (error_code == -1)
		ft_putstr_fd("Error\n", 2);
}
