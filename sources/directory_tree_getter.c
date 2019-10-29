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

static char			*get_new_path_name(t_file_info *file_info, const char *prev_path_name,
						   size_t *total_len)
{
	char	*new_path_name;
	size_t	new_path_size;

	new_path_size = *total_len + file_info->dir->d_namlen + 1; // +1 для '/'
	if (!(new_path_name = (char*)malloc(sizeof(char) * new_path_size + 1))) // +1 для \0
		raise_error(ERR_MALLOC);
	ft_memcpy(new_path_name, prev_path_name, *total_len);
	new_path_name[*total_len] = '/';
	ft_memcpy(&new_path_name[*total_len + 1], file_info->dir->d_name, file_info->dir->d_namlen);
	new_path_name[new_path_size] = 0; 	// если сначала сделать +2 в new_path_size тут все наебнется
	// потому что '\0' добавится на один индекс позже, чем надо
	*total_len = new_path_size;
	return (new_path_name);
}

static inline int		check_hidden(char *filename)
{
	if (filename[0] != '.')
		return (TRUE);
	else
		return (g_options.display_hidden);
}

t_avl_tree				*get_dir_btree(char *dirname, size_t relative_path_name_len)
{
	t_avl_tree		*tree;
	t_file_info		file_info;
	DIR				*dir;
	char			*relative_path_name;

	tree = NULL;
	if (!(dir = opendir(dirname))) // opendir() failed (мусор в конце / нет прав)
	{
		ft_printf("./ft_ls: %s: %s\n", dirname, strerror(errno));
		return (NULL);
	}
	while ((file_info.dir = readdir(dir)))
	{
		if (!check_hidden(file_info.dir->d_name)) // если не задана -a пропустить скрытые (но мы не пропустим начальную, потому что проверяется d_name!)
			continue ;
		relative_path_name = get_new_path_name(&file_info, dirname, &relative_path_name_len);
		if (lstat(relative_path_name, &file_info.file) == FT_ERR)
			return (NULL);
		if (is_dir_not_dot(file_info.file.st_mode, file_info.dir->d_name)) // !  эта штука нужна только для -R
			avl_insert_data(&tree, get_filestruct(
					relative_path_name, relative_path_name_len, TRUE, &file_info), generic_cmpfunc);
		avl_insert_data(&tree, get_filestruct(
				relative_path_name, file_info.dir->d_namlen, FALSE, &file_info), generic_cmpfunc);
		g_longest_filename = ft_max(g_longest_filename, file_info.dir->d_namlen);
		relative_path_name_len -= file_info.dir->d_namlen + 1;
		//тут -1 потому что добавили '/', который (очевидно) не учитывается в d_namelen
	}
	closedir(dir);
	return (tree);
}
