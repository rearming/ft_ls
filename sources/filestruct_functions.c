/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filestruct_functions.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sleonard <sleonard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/28 19:22:56 by sleonard          #+#    #+#             */
/*   Updated: 2019/10/28 20:51:42 by sleonard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

inline int			is_dir_not_dot(mode_t st_mode, char *dirname)
{
	return (S_ISDIR(st_mode)
			&& !ft_strequ(dirname, ".")
			&& !ft_strequ(dirname, ".."));
}

void				free_filestruct(void *filestruct_ptr)
{
	t_filestruct		*filestruct;

	filestruct = (t_filestruct*)filestruct_ptr;
	free(filestruct->filename);
	free(filestruct);
}

inline t_filestruct *get_filestruct(char *relative_path_name, size_t total_len, t_flag is_dir,
									t_file_info *file_info)
{
	t_filestruct		*result;

	if (!(result = (t_filestruct*)malloc(sizeof(t_filestruct))))
		raise_error(ERR_MALLOC);
	result->full_path = relative_path_name;
	if (!is_dir)
		result->filename = ft_strdup_l(file_info->dir->d_name, file_info->dir->d_namlen);
	else
		result->filename = relative_path_name;
	result->total_len = total_len;
	if (!file_info)
		return (result);
	result->is_dir = is_dir;
	result->st_mode = file_info->file.st_mode;
	result->is_hidden = file_info->dir->d_name[0] == '.' ? TRUE : FALSE;
	result->last_access = file_info->file.st_atimespec.tv_sec;
	result->last_modif = file_info->file.st_mtimespec.tv_sec;
	result->last_change = file_info->file.st_ctimespec.tv_sec;
	result->st_blocks = file_info->file.st_blocks;
	result->inode = file_info->file.st_ino;
	result->file_size = file_info->file.st_size;
	result->hard_links = file_info->file.st_nlink;
	result->user_id = file_info->file.st_uid;
	result->group_id = file_info->file.st_gid;
	return (result);
}
