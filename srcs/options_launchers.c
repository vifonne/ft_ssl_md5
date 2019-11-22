/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   options_launchers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vifonne <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/22 16:42:57 by vifonne           #+#    #+#             */
/*   Updated: 2019/11/22 16:43:04 by vifonne          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl_md5.h"

int		launch_opt_p(t_parsing *parsing)
{
	int	s_tmp;

	s_tmp = parsing->opt.s;
	parsing->opt.s = 0;
	if (!parsing->fct_table.hash_main(NULL, &parsing->fct_table
		, parsing->opt, parsing->algo_choosen))
		return (0);
	parsing->opt.p = 0;
	parsing->opt.s = s_tmp;
	return (1);
}

int		launch_arg(char **av, t_parsing *parsing)
{
	int	status;

	status = parse_opt(av[parsing->arg_idx] + 1, &parsing->opt);
	if (status == -1)
	{
		ft_error(NULL, -3);
		return (-1);
	}
	if (parsing->opt.p == 1)
		launch_opt_p(parsing);
	if (status > 0)
	{
		if (!parsing->fct_table.hash_main(av[parsing->arg_idx] + status + 1
			, &parsing->fct_table, parsing->opt, parsing->algo_choosen))
			return (0);
		parsing->opt.s = 0;
	}
	if (av[parsing->arg_idx + 1] == NULL)
		parsing->fct_table.hash_main(NULL, &parsing->fct_table
			, parsing->opt, parsing->algo_choosen);
	return (1);
}

int		launch_no_opt(char **av, int status, t_parsing *parsing)
{
	if (status != -1)
	{
		if (!parsing->fct_table.hash_main(av[parsing->arg_idx]
			, &parsing->fct_table, parsing->opt, parsing->algo_choosen))
			return (0);
	}
	parsing->opt.p = 0;
	parsing->opt.s = 0;
	return (1);
}
