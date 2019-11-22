/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   options.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vifonne <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/15 15:02:09 by vifonne           #+#    #+#             */
/*   Updated: 2019/11/22 13:35:19 by vifonne          ###   ########.fr       */
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
	int			tmp;

	idx = 0;
	opts = "pqrs";
	while (str[idx] != '\0')
	{
		if (ft_strchr(opts, str[idx]) != 0)
		{
			tmp = opt_triggers(opt, str, idx);
			if (tmp == -3)
				return (idx);
			else if (tmp != -2)
				return (tmp);
		}
		idx++;
	}
	return (0);
}

int		launch_arg(char *av, int algo_choosen, t_options *opt
			, t_functions fct_table)
{
	int	tmp;
	int	s_tmp;

	tmp = parse_opt(av, opt);
	if (opt->p == 1)
	{
		s_tmp = opt->s;
		opt->s = 0;
		if (!fct_table.hash_main(NULL, &fct_table, *opt, algo_choosen))
			return (0);
		opt->p = 0;
		opt->s = s_tmp;
	}
	if (tmp > 0)
	{
		if (!fct_table.hash_main(av + tmp + 1, &fct_table, *opt
				, algo_choosen))
			return (0);
		opt->s = 0;
	}
	return (1);
}

int		get_opt(int ac, char **av, int algo_choosen)
{
	size_t		arg_idx;
	t_options	opt;
	t_functions	fct_table;

	arg_idx = 1;
	opt = (t_options){0, 0, 0, 0};
	fct_table = set_fct_table(algo_choosen);
	while (arg_idx < (size_t)ac)
	{
		if (av[arg_idx][0] == '-')
		{
			launch_arg(av[arg_idx], algo_choosen, &opt, fct_table);
		}
		else
		{
			fct_table.hash_main(av[arg_idx], &fct_table, opt, algo_choosen);
			opt.p = 0;
			opt.s = 0;
		}
		arg_idx++;
	}
	return (1);
}
