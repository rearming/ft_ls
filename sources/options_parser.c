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

const char	*g_allowed_options;

t_options	default_options(void)
{
	return ((t_options)
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
	});
}

static inline int	option(char actual_option, char expected_option, int set_option)
{
	if (!set_option)
		return (actual_option == expected_option);
	else
		return (set_option);
}

static inline int	get_sort_type(char actual_option, int set_option)
{
	if (!set_option)
	{
		if (actual_option == 't')
			return (BY_TIME);
		if (actual_option == 'S')
			return (BY_SIZE);
		if (actual_option == 'f')
			return (NO_SORT);
	}
	return (set_option);
}

static inline int	get_time_mode(char actual_option, int set_option)
{
	if (!set_option)
	{
		if (actual_option == 'u')
			return (ACCESS);
		if (actual_option == 'c')
			return (STATUS_MODIFICATION);
	}
	return (set_option);
}

void		parse_options_pack(char *pack, t_options *options)
{
	int		i;

	i = 0;
	pack++;
	while (pack[i])
	{
		if (!ft_strchr(g_allowed_options, pack[i]))
			raise_error(pack[i]);
		options->is_verbose = option(pack[i], 'l', options->is_verbose);
		options->is_recursive = option(pack[i], 'R', options->is_recursive);
		options->sort_reverse = option(pack[i], 'r', options->sort_reverse);
		options->time_mode = get_time_mode(pack[i], options->time_mode);
		options->sort_type = get_sort_type(pack[i], options->sort_type);
		options->human_readable = option(pack[i], 'h', options->human_readable);
		options->display_hidden = option(pack[i], 'a', options->display_hidden);
		options->is_one_column = option(pack[i], '1', options->is_one_column);
		options->is_colored = option(pack[i], 'G', options->is_colored);
		options->hidden_no_dots = option(pack[i], 'A', options->hidden_no_dots);
		options->dirs_like_files = option(pack[i], 'd', options->dirs_like_files);
		i++;
	}
}

t_options	get_options(char **args, int options_num, int *first_filename)
{
	t_options	options;
	int			i;

	i = 0;
	options = default_options();
	if (options_num == 0)
		return (options);
	while (i < options_num)
	{
		if (args[i][0] != '-' || (args[i][0] == '-' && (!args[i][1])))
			return (options);
		parse_options_pack(args[i], &options);
		(*first_filename)++;
		i++;
	}
	return (options);
}
