/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   options_parser.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sleonard <sleonard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/25 18:07:52 by sleonard          #+#    #+#             */
/*   Updated: 2019/10/28 20:14:35 by sleonard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

t_options g_options =
{
	.is_verbose = FALSE,
	.is_recursive = FALSE,
	.sort_reverse = FALSE,
	.sort_type = BY_ALPHA,
	.time_mode = MODIFICATION,
	.human_readable = FALSE,
	.display_hidden = FALSE,
	.is_one_column = FALSE,
	.is_many_args = FALSE,
	.is_colored = FALSE,
	.hidden_no_dots = FALSE,
	.dirs_like_files = FALSE,
};

static inline void	option(char actual_option, char expected_option,
								int *set_option)
{
	if (!*set_option)
	{
		*set_option = actual_option == expected_option;
		return ;
	}
}

static inline int	get_sort_type(char actual_option, int set_option)
{
	if (actual_option == 't')
		return (BY_TIME);
	if (actual_option == 'S')
		return (BY_SIZE);
	if (actual_option == 'f')
		return (NO_SORT);
	return (set_option);
}

static inline int	get_time_mode(char actual_option, int set_option)
{
	if (actual_option == 'u')
		return (ACCESS);
	if (actual_option == 'c')
		return (STATUS_MODIFICATION);
	return (set_option);
}

static void			parse_options_pack(char *pack)
{
	int		i;

	i = 0;
	pack++;
	while (pack[i])
	{
		if (!ft_strchr(ALLOWED_OPTIONS, pack[i]))
			raise_error(pack[i]);
		option(pack[i], 'l', &g_options.is_verbose);
		option(pack[i], 'R', &g_options.is_recursive);
		option(pack[i], 'r', &g_options.sort_reverse);
		g_options.time_mode = get_time_mode(pack[i], g_options.time_mode);
		g_options.sort_type = get_sort_type(pack[i], g_options.sort_type);
		option(pack[i], 'h', &g_options.human_readable);
		option(pack[i], 'a', &g_options.display_hidden);
		option(pack[i], '1', &g_options.is_one_column);
		option(pack[i], 'G', &g_options.is_colored);
		option(pack[i], 'A', &g_options.hidden_no_dots);
		option(pack[i], 'd', &g_options.dirs_like_files);
		i++;
	}
}

void				get_options(char **args, int options_num,
						int *first_filename)
{
	int		i;

	i = 0;
	if (!isatty(fileno(stdout)))
		g_options.is_one_column = TRUE;
	if (options_num == 0)
		return ;
	while (i < options_num)
	{
		if (args[i][0] != '-' || (args[i][0] == '-' && (!args[i][1])))
			return ;
		parse_options_pack(args[i]);
		(*first_filename)++;
		i++;
	}
}
