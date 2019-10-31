/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compare_functions.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sleonard <sleonard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/28 19:22:56 by sleonard          #+#    #+#             */
/*   Updated: 2019/10/28 20:51:42 by sleonard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static inline int	lexical_cmpfunc(void *dir1_ptr, void *dir2_ptr)
{
	t_filestruct		*dir1;
	t_filestruct		*dir2;

	dir1 = (t_filestruct*)dir1_ptr;
	dir2 = (t_filestruct*)dir2_ptr;
	return (ft_strcmp(dir1->filename, dir2->filename));
}

static inline long	time_cmpfunc(void *dir1_ptr, void *dir2_ptr)
{
	t_filestruct		*dir1;
	t_filestruct		*dir2;
	long long			result;

	dir1 = (t_filestruct*)dir1_ptr;
	dir2 = (t_filestruct*)dir2_ptr;
	result = 0;
	if (g_options.time_mode == MODIFICATION)
		result = dir2->last_modif - dir1->last_modif;
	else if (g_options.time_mode == ACCESS)
		result = dir2->last_access - dir1->last_access;
	else if (g_options.time_mode == STATUS_MODIFICATION)
		result = dir2->last_change - dir1->last_change;
	else
		raise_error(ERR_UNKNOWN_TIME);
	if (result == 0)
		return (lexical_cmpfunc(dir1_ptr, dir2_ptr));
	return (result);
}

static inline long	size_cmpfunc(void *dir1_ptr, void *dir2_ptr)
{
	t_filestruct		*dir1;
	t_filestruct		*dir2;

	dir1 = (t_filestruct*)dir1_ptr;
	dir2 = (t_filestruct*)dir2_ptr;
	return (dir1->file_size - dir2->file_size);
}

static inline int	no_cmpfunc(void *dir1_ptr, void *dir2_ptr)
{
	(void)dir1_ptr;
	(void)dir2_ptr;
	return (-1);
}

inline int			generic_cmpfunc(void *dir1_ptr, void *dir2_ptr)
{
	long			result;
	t_filestruct		*dir1;
	t_filestruct		*dir2;

	dir1 = (t_filestruct*)dir1_ptr;
	dir2 = (t_filestruct*)dir2_ptr;
	if (g_options.sort_type == BY_ALPHA)
		result = lexical_cmpfunc(dir1_ptr, dir2_ptr);
	else if (g_options.sort_type == BY_TIME)
		result = time_cmpfunc(dir1_ptr, dir2_ptr);
	else if (g_options.sort_type == BY_SIZE)
		result = size_cmpfunc(dir1_ptr, dir2_ptr);
	else if (g_options.sort_type == NO_SORT)
		result = no_cmpfunc(dir1_ptr, dir2_ptr);
	else
		raise_error(ERR_UNKNOWN_SORT);
	if (g_options.sort_reverse)
		result = -result;
	if (dir1->is_dir_recursive != dir2->is_dir_recursive)
		return (dir1->is_dir_recursive - dir2->is_dir_recursive);
	return (result < 0 ? -1 : 1);
}
