/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sleonard <sleonard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/09 16:57:16 by sleonard          #+#    #+#             */
/*   Updated: 2019/10/28 20:14:35 by sleonard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

t_bool						dir_exists(const char *filename)
{
	DIR		*dir;

	dir = opendir(filename);
	if (dir)
	{
		closedir(dir);
		return (TRUE);
	}
	else
		return (FALSE);
}

inline void					get_winsize(void)
{
	struct winsize	winsize;

	ioctl(STDOUT_FILENO, TIOCGWINSZ, &winsize);
	g_term_width = winsize.ws_col;
}

inline int					process_no_args(void)
{
	g_options.is_many_files = FALSE;
	g_options.is_many_dirs = FALSE;
	g_options.is_first_eol = FALSE;
	process_dir(&(t_filestruct){.filename = ".", .is_dir_recursive = TRUE});
	return (0);
}
