/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   directory_tree_getter.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sleonard <sleonard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/28 19:22:56 by sleonard          #+#    #+#             */
/*   Updated: 2019/10/28 20:51:42 by sleonard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static inline void		init_dirstruct(t_dirstruct **p_dirstruct)
{
	if (!(*p_dirstruct = (t_dirstruct*)malloc(sizeof(t_dirstruct))))
		raise_error(ERR_MALLOC);
	(*p_dirstruct)->longest = (t_longest_strs){0, 0, 0, 0, 0, 0, 0};
	(*p_dirstruct)->total_blocks = 0;
	(*p_dirstruct)->tree = NULL;
}

static inline t_flag	is_root(const char *path_name)
{
	return ((path_name[0] == '/' && path_name[1] == '\0'));
}

static char				*get_new_path_name(t_file_info *file_info,
							const char *prev_path_name, size_t *total_len)
{
	char	*new_path_name;
	size_t	new_path_size;
	t_flag	root;

	root = is_root(prev_path_name);
	new_path_size = *total_len + file_info->dirent->d_namlen + !root;

	if (!(new_path_name = (char*)malloc(sizeof(char) * new_path_size + 1)))
		raise_error(ERR_MALLOC);
	ft_memcpy(new_path_name, prev_path_name, *total_len);
	if (!root)
		new_path_name[*total_len] = '/';
	ft_memcpy(&new_path_name[*total_len + !root],
			file_info->dirent->d_name, file_info->dirent->d_namlen);
	new_path_name[new_path_size] = 0;
	*total_len = new_path_size;
	return (new_path_name);
}

static inline int		check_hidden(const char *filename)
{
	if (filename[0] != '.')
		return (TRUE);
	else if (((filename[0] == '.' && filename[1] == '\0')
		|| (filename[0] == '.' && filename[1] == '.' && filename[2] == '\0'))
		&& g_options.hidden_no_dots && !g_options.display_hidden)
		return (FALSE);
	else
		return (g_options.hidden_no_dots || g_options.display_hidden);
}

static inline void		update_longest(t_longest_strs *l_strs,
							t_filestruct *filestruct)
{
	l_strs->filename = ft_max(l_strs->filename, filestruct->total_len);
	l_strs->h_links = ft_max(l_strs->h_links, ft_count_digits(filestruct->hard_links));
	l_strs->user_name = ft_max(l_strs->user_name, ft_strlen(filestruct->user_name));
	l_strs->group_name = ft_max(l_strs->group_name, ft_strlen(filestruct->group_name));
	l_strs->file_size = ft_max(l_strs->file_size, ft_count_digits(filestruct->file_size));
	l_strs->major_nbr = ft_max(l_strs->major_nbr, ft_count_digits(filestruct->major_nbr));
	l_strs->minor_nbr = ft_max(l_strs->minor_nbr, ft_count_digits(filestruct->minor_nbr));
}

t_dirstruct				*get_dir_btree(const char *dirname, size_t relative_path_name_len)
{
	t_dirstruct		*dirstruct;
	t_filestruct	*filestruct;
	t_file_info		file_info;
	DIR				*dir;
	char			*relative_path_name;

	init_dirstruct(&dirstruct);
	if (!(dir = opendir(dirname)) || g_options.dirs_like_files)
	{
		if (errno == ENOTDIR || g_options.dirs_like_files)
		{
			file_info.dirent = NULL;
			lstat(dirname, &file_info.file);
			avl_insert_data(&dirstruct->tree, get_filestruct(ft_strdup(dirname), relative_path_name_len, FALSE, &file_info), generic_cmpfunc);
		}
		else
			ft_printf_fd(STDERR_FILENO, "./ft_ls: %s: %s\n", dirname, strerror(errno));
		if (dir)
			closedir(dir);
		return (dirstruct);
	}
	while ((file_info.dirent = readdir(dir)))
	{
		if (!check_hidden(file_info.dirent->d_name))
			continue ;
		relative_path_name = get_new_path_name(&file_info, dirname, &relative_path_name_len);
		if (lstat(relative_path_name, &file_info.file) == FT_ERR)
		{
			free(relative_path_name);
			return (dirstruct);
		}
		if (is_dir_not_dot(file_info.file.st_mode, file_info.dirent->d_name)) // !  эта штука нужна только для -R
			avl_insert_data(&dirstruct->tree, get_filestruct(ft_strdup(relative_path_name), relative_path_name_len, TRUE, &file_info), generic_cmpfunc);

		filestruct = get_filestruct(relative_path_name, file_info.dirent->d_namlen, FALSE, &file_info);
		avl_insert_data(&dirstruct->tree, filestruct, generic_cmpfunc);

		update_longest(&dirstruct->longest, filestruct);
		relative_path_name_len -= file_info.dirent->d_namlen + !is_root(dirname);
		dirstruct->total_blocks += filestruct->st_blocks;
	}
	closedir(dir);
	return (dirstruct);
}
