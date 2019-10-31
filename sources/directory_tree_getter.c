/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   directory_btree_getter.c                           :+:      :+:    :+:   */
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
	(*p_dirstruct)->longest = (t_longest_strs){0, 0, 0, 0, 0};
	(*p_dirstruct)->total_blocks = 0;
	(*p_dirstruct)->tree = NULL;
}

static inline char		*get_new_path_name(t_file_info *file_info, const char *prev_path_name,
						   size_t *total_len)
{
	char	*new_path_name;
	size_t	new_path_size;

	new_path_size = *total_len + file_info->dirent->d_namlen + 1; // +1 для '/'
	if (!(new_path_name = (char*)malloc(sizeof(char) * new_path_size + 1))) // +1 для \0
		raise_error(ERR_MALLOC);
	ft_memcpy(new_path_name, prev_path_name, *total_len);
	new_path_name[*total_len] = '/';
	ft_memcpy(&new_path_name[*total_len + 1], file_info->dirent->d_name, file_info->dirent->d_namlen);
	new_path_name[new_path_size] = 0; 	// если сначала сделать +2 в new_path_size тут все наебнется
	// потому что '\0' добавится на один индекс позже, чем надо
	*total_len = new_path_size;
	return (new_path_name);
}

static inline int		check_hidden(const char *filename)
{
	if (filename[0] != '.')
		return (TRUE);
	else
		return (g_options.display_hidden);
}

static inline void		update_longest(t_longest_strs *l_strs,
							t_filestruct *filestruct)
{
	l_strs->filename = ft_max(l_strs->filename, filestruct->total_len);
	l_strs->h_links = ft_max(l_strs->h_links, ft_count_digits(filestruct->hard_links));
	l_strs->user_name = ft_max(l_strs->user_name, ft_strlen(filestruct->user_name));
	l_strs->group_name = ft_max(l_strs->group_name, ft_strlen(filestruct->group_name));
	l_strs->file_size = ft_max(l_strs->file_size, ft_count_digits(filestruct->file_size));
}

t_dirstruct				*get_dir_btree(const char *dirname, size_t relative_path_name_len)
{
	t_dirstruct		*dirstruct;
	t_filestruct	*filestruct;
	t_file_info		file_info;
	DIR				*dir;
	char			*relative_path_name;

	init_dirstruct(&dirstruct);
	if (!(dir = opendir(dirname))) // opendir() failed (мусор в конце / нет прав)
	{
		if (errno == ENOTDIR)
		{
			file_info.dirent = NULL;
			lstat(dirname, &file_info.file);
			avl_insert_data(&dirstruct->tree, get_filestruct(ft_strdup(dirname), relative_path_name_len, FALSE, &file_info), generic_cmpfunc);
		}
		else
			ft_printf_fd(STDERR_FILENO, "./ft_ls: %s: %s\n", dirname, strerror(errno));
		return (dirstruct);
	}
	while ((file_info.dirent = readdir(dir)))
	{
		if (!check_hidden(file_info.dirent->d_name)) // если не задана -a пропустить скрытые (но мы не пропустим начальную, потому что проверяется d_name!)
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
		relative_path_name_len -= file_info.dirent->d_namlen + 1;
		//тут -1 потому что добавили '/', который (очевидно) не учитывается в d_namelen
		dirstruct->total_blocks += filestruct->st_blocks; //для 'total'
	}
	closedir(dir);
	return (dirstruct);
}
