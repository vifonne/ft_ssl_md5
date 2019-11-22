/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   options.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vifonne <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/15 15:02:09 by vifonne           #+#    #+#             */
/*   Updated: 2019/11/22 16:44:22 by vifonne          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl_md5.h"

int		no_options(t_options opt)
{
	if (opt.p == 0 && opt.q == 0 && opt.r == 0 && opt.s == 0)
		return (1);
	return (0);
}

int		opt_triggers(t_options *opt, char *str, size_t idx)
{
	if (str[idx] == 's')
	{
		opt->s = 1;
		if (idx + 1 < ft_strlen(str))
			return (-3);
		else
			return (0);
	}
	else if (str[idx] == 'p')
		opt->p = 1;
	else if (str[idx] == 'q')
		opt->q = 1;
	else if (str[idx] == 'r')
		opt->r = 1;
	else
		return (-1);
	return (-2);
}

int		parse_opt(char *str, t_options *opt)
{
	char		*opts;
	size_t		idx;
	int			status;

	idx = 0;
	opts = "pqrs";
	while (str[idx] != '\0')
	{
		if (ft_strchr(opts, str[idx]) != 0)
		{
			status = opt_triggers(opt, str, idx);
			if (status == -3)
				return (idx);
			else if (status != -2)
				return (status);
		}
		else
		{
			return (-1);
		}
		idx++;
	}
	return (0);
}

int		get_opt(int ac, char **av, int algo_choosen)
{
	int			file_trigger;
	int			status;
	t_parsing	parsing;

	parsing.arg_idx = 1;
	file_trigger = 0;
	parsing.opt = (t_options){0, 0, 0, 0};
	parsing.fct_table = set_fct_table(algo_choosen);
	parsing.algo_choosen = algo_choosen;
	if (av[parsing.arg_idx] == NULL)
	{
		parsing.fct_table.hash_main(NULL, &parsing.fct_table, parsing.opt
			, parsing.algo_choosen);
	}
	while (parsing.arg_idx < (size_t)ac)
	{
		if (av[parsing.arg_idx][0] == '-' && file_trigger == 0)
			status = launch_arg(av, &parsing);
		else
			file_trigger = launch_no_opt(av, status, &parsing);
		parsing.arg_idx++;
	}
	return (1);
}
