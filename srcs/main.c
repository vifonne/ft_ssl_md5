/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vifonne <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/10 10:17:48 by vifonne           #+#    #+#             */
/*   Updated: 2019/11/22 16:21:57 by vifonne          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl_md5.h"

int		algo_finder(char *algo_name)
{
	size_t	idx;
	char	*algo_name_tab[3];

	algo_name_tab[0] = "md5";
	algo_name_tab[1] = "sha256";
	algo_name_tab[2] = NULL;
	idx = 0;
	while (algo_name_tab[idx] != NULL)
	{
		if (ft_strequ(algo_name, algo_name_tab[idx]) == 1)
			return (idx);
		idx++;
	}
	return (-1);
}

int		main(int ac, char **av)
{
	int			algo_choosen;

	if (ac > 1)
	{
		algo_choosen = algo_finder(av[1]);
		if (algo_choosen == -1)
		{
			ft_error(NULL, -3);
		}
		else
		{
			if (!get_opt(ac - 1, av + 1, algo_choosen))
				ft_error(NULL, -4);
		}
	}
	return (0);
}
