/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dir_tree_getter.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sleonard <sleonard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/28 19:22:56 by sleonard          #+#    #+#             */
/*   Updated: 2019/10/28 20:51:42 by sleonard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static inline t_bool	check_link_nofollow(const char *dirname, mode_t st_mode)
{
	return (S_ISLNK(st_mode) && g_options.is_verbose)
	|| (!dir_exists(dirname) && !g_options.is_recursive && errno == ENOENT);
}

static inline DIR		*get_dir(const char *dirname, t_dirstruct *dirstruct,
									size_t relative_path_name_len)
{
	DIR				*dir;
	t_file_info		file_info;
	t_filestruct	*filestruct;
	t_bool			link_nofollow;

	link_nofollow = lstat(dirname, &file_info.file) != FT_ERR ?
			check_link_nofollow(dirname, file_info.file.st_mode) : FALSE;
	if (!(dir = opendir(dirname)) || g_options.dirs_like_files || link_nofollow)
	{
		if (errno == ENOTDIR || g_options.dirs_like_files || link_nofollow)
		{
			file_info.dirent = NULL;
			filestruct = get_filestruct(ft_strdup(dirname),
					relative_path_name_len, FALSE, &file_info);
			update_longest(&dirstruct->longest, filestruct);
			avl_insert_data(&dirstruct->tree, filestruct, generic_cmpfunc);
		}
		else
			ft_printf_fd(STDERR_FILENO,
					"ft_ls: %s: %s\n", dirname, strerror(errno));
		if (dir)
			closedir(dir);
		return (NULL);
	}
	return (dir);
}

static inline int		process_dir_entry(char *path,
		size_t relative_path_name_len,
		t_dirstruct *dirstruct,
		t_file_info *file_info)
{
	t_filestruct	*filestruct;

	if (lstat(path, &file_info->file) == FT_ERR)
	{
		free(path);
		return (FALSE);
	}
	if (is_dir_not_dot(file_info->file.st_mode, file_info->dirent->d_name))
		avl_insert_data(
			&dirstruct->tree,
			get_filestruct(ft_strdup(path),
			relative_path_name_len, TRUE,
			file_info),
		generic_cmpfunc);
	filestruct = get_filestruct(path,
			file_info->dirent->d_namlen, FALSE, file_info);
	avl_insert_data(&dirstruct->tree, filestruct, generic_cmpfunc);
	update_longest(&dirstruct->longest, filestruct);
	dirstruct->total_blocks += filestruct->st_blocks;
	dirstruct->files_num++;
	return (TRUE);
}

t_dirstruct				*get_dir_btree(const char *dirname, size_t path_len)
{
	t_dirstruct		*dirstruct;
	t_file_info		file_info;
	DIR				*dir;
	char			*path_name;

	init_dirstruct(&dirstruct);
	if (!(dir = get_dir(dirname, dirstruct, path_len)))
		return (dirstruct);
	while ((file_info.dirent = readdir(dir)))
	{
		if (!check_hidden(file_info.dirent->d_name))
			continue;
		path_name = get_new_path_name(&file_info, dirname, &path_len);
		if (!process_dir_entry(path_name, path_len, dirstruct, &file_info))
			return (dirstruct);
		path_len -= file_info.dirent->d_namlen + !is_root(dirname);
	}
	closedir(dir);
	return (dirstruct);
}
