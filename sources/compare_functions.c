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

static inline long	time_cmpfunc(void *dir1_ptr, void *dir2_ptr)
{
	t_filestruct		*dir1;
	t_filestruct		*dir2;

	dir1 = (t_filestruct*)dir1_ptr;
	dir2 = (t_filestruct*)dir2_ptr;
	if (g_options.time_mode == MODIFICATION)
		return (dir1->last_modif - dir2->last_modif);
	if (g_options.time_mode == ACCESS)
		return (dir1->last_access - dir2->last_access);
	if (g_options.time_mode == STATUS_MODIFICATION)
		return (dir1->last_change - dir2->last_change);
	raise_error(ERR_UNKNOWN_TIME);
	return (0);
}

static inline int	lexical_cmpfunc(void *dir1_ptr, void *dir2_ptr)
{
	t_filestruct		*dir1;
	t_filestruct		*dir2;

	dir1 = (t_filestruct*)dir1_ptr;
	dir2 = (t_filestruct*)dir2_ptr;

	if (dir1->is_dir != dir2->is_dir) // нужно для того, чтобы сначала обходить файлы, а потому уже директории
		return (dir1->is_dir - dir2->is_dir);
	return (ft_strcmp(dir1->filename, dir2->filename));
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
	if (dir1->is_dir != dir2->is_dir)
		return (dir1->is_dir - dir2->is_dir);
	return (result < 0 ? -1 : 1);
}
