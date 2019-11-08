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

inline void			free_filestruct(void *filestruct_ptr)
{
	t_filestruct		*filestruct;

	filestruct = (t_filestruct*)filestruct_ptr;
	free(filestruct->filename);
	free(filestruct->full_path);
	free(filestruct->user_name);
	free(filestruct->group_name);
	free(filestruct);
}

static inline void	parse_file_unfo(t_file_info *file_info,
		t_filestruct *out_filestruct)
{
	t_passwd	*passwd;
	t_group		*group;

	out_filestruct->major_nbr = major(file_info->file.st_rdev);
	out_filestruct->minor_nbr = minor(file_info->file.st_rdev);
	out_filestruct->last_access = file_info->file.st_atimespec.tv_sec;
	out_filestruct->last_modif = file_info->file.st_mtimespec.tv_sec;
	out_filestruct->last_change = file_info->file.st_ctimespec.tv_sec;
	out_filestruct->st_blocks = file_info->file.st_blocks;
	out_filestruct->file_size = file_info->file.st_size;
	out_filestruct->hard_links = file_info->file.st_nlink;
	passwd = getpwuid(file_info->file.st_uid);
	group = getgrgid(file_info->file.st_gid);
	out_filestruct->user_name =
			passwd ? ft_strdup(passwd->pw_name) : ft_strdup("");
	out_filestruct->group_name =
			group ? ft_strdup(group->gr_name) : ft_strdup("");
}

inline t_filestruct	*get_filestruct(char *relative_path_name, size_t total_len,
		t_bool is_dir_recursive, t_file_info *file_info)
{
	t_filestruct		*result;

	if (!(result = (t_filestruct*)malloc(sizeof(t_filestruct))))
		raise_error(ERR_MALLOC);
	result->full_path = relative_path_name;
	result->is_dir_recursive = is_dir_recursive;
	result->user_name = NULL;
	result->group_name = NULL;
	if (!is_dir_recursive && file_info && file_info->dirent)
		result->filename = ft_strdup_l(file_info->dirent->d_name,
				file_info->dirent->d_namlen);
	else
		result->filename = ft_strdup(relative_path_name);
	result->filename_len = total_len;
	if (!file_info)
		return (result);
	result->st_mode = file_info->file.st_mode;
	parse_file_unfo(file_info, result);
	return (result);
}
