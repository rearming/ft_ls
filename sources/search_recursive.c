/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search_recursive.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sleonard <sleonard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/28 19:22:56 by sleonard          #+#    #+#             */
/*   Updated: 2019/10/28 20:51:42 by sleonard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

t_list	*g_lstbuf;

/*
**	Оригинальный ls просто добавляет новый / к директориям, то есть ////usr///
**	никак не исправляется и дальше так же работает
*/

char	*get_new_path_name(t_file_info *file_info, const char *prev_path_name,
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
	new_path_name[new_path_size] = 0; // если сначала сделать +2 в new_path_size тут все наебнется
	*total_len = new_path_size;
	return (new_path_name);
}

/*
**	пока что печатает скрытые файлы и не печатает . и ..
 *  сравнивать с ls -1aR !
*/

// 	-> ./ft_ls /  5.08s user 32.84s system 20% cpu 3:05.34 total

void	fake_delfunc(void *todel)
{
	(void)todel;
}

int		dirstruct_cmpf(void *str1, void *str2)
{
	t_dirstruct		*dir1;
	t_dirstruct		*dir2;

	dir1 = (t_dirstruct*)str1;
	dir2 = (t_dirstruct*)str2;

	if (dir1->is_dir != dir2->is_dir)
		return (dir1->is_dir - dir2->is_dir);
	return (ft_strcmp(dir1->dirname, dir2->dirname));
}

void	recursion_callback(void *dirstruct_ptr)
{
	t_dirstruct		*dirstruct;

	dirstruct = (t_dirstruct*)dirstruct_ptr;

	ft_printf("%s\n", dirstruct->dirname); //нужно показывать все файлы, включая директории

	if (dirstruct->is_dir)
	{
		recursive_btree(dirstruct); //а еще директории с permission denied идут как обычные файлы
									// (то есть надо убрать / и всякое такое)
	}
	else
	{
//		free(dirstruct->dirname);
	}
}

void	recursive_btree(void *dirstruct_ptr)
{
	t_dirstruct		*dirstruct;
	t_file_info		info;
	DIR				*dir;
	t_avl_tree		*tree;
	char			*name;

	dirstruct = (t_dirstruct*)dirstruct_ptr;
	tree = NULL;
	ft_printf("\n%s:\n", dirstruct->dirname);

	if (!(dir = opendir(dirstruct->dirname))) // тут может сфейлиться опендир, если имя кривое (мусор в конце например)
	{
		ft_printf("./ft_ls: %s: %s\n", dirstruct->dirname, strerror(errno));
		return (free(dirstruct->dirname));
	}

	while ((info.dir = readdir(dir)))
	{
		name = get_new_path_name(&info, dirstruct->dirname, &dirstruct->total_len);

		if (lstat(name, &info.file) == FT_ERR)
		{
			return (free(dirstruct->dirname));
		}
		if (S_ISDIR(info.file.st_mode)
		&& !ft_strequ(info.dir->d_name, ".")
		&& !ft_strequ(info.dir->d_name, ".."))
			avl_insert_data(&tree, get_dirstruct(name, dirstruct->total_len, TRUE), dirstruct_cmpf);
		else
		{
//			ft_printf("%s\n", info.dir->d_name);
			free(name);
		}
		avl_insert_data(&tree, get_dirstruct(info.dir->d_name, info.dir->d_namlen, FALSE), dirstruct_cmpf);

		dirstruct->total_len -= info.dir->d_namlen + 1;
		//тут -1 потому что добавили '/', который (очевидно) не учитывается в d_namelen
	}
	closedir(dir);
	if (tree)
	{
//		btree_apply_inorder((t_btree*)tree, recursive_btree);
		btree_apply_inorder((t_btree*)tree, recursion_callback);
		free_btree(tree, fake_delfunc);
	}
}

void	recursive_list(char *dirname, size_t total_len)
{
	t_file_info		info;
	DIR				*dir;
	t_list			*dirs;
	t_list			*begin;
	char			*name;

	dirs = NULL;

	ft_printf("\n%s:\n", dirname);

	if (!(dir = opendir(dirname))) // тут может сфейлиться опендир, если имя кривое (мусор в конце например)
	{
		ft_printf("./ft_ls: %s: %s\n", dirname, strerror(errno));
		return (free(dirname));
	}

	while ((info.dir = readdir(dir)))
	{
		name = get_new_path_name(&info, dirname, &total_len);

		if (lstat(name, &info.file) == FT_ERR)
		{
//			ft_printf("./ft_ls: [%s]\n", strerror(errno));
			return (free(dirname));
		}

		ft_printf("%s\n", info.dir->d_name);

		if (S_ISDIR(info.file.st_mode)
		&& !ft_strequ(info.dir->d_name, ".")
		&& !ft_strequ(info.dir->d_name, ".."))
			ft_lstaddback_p(&dirs, name, total_len);
		else
			free(name);
		total_len -= info.dir->d_namlen + 1;
		//тут -1 потому что добавили '/', который (очевидно) не учитывается в d_namelen
	}
	begin = dirs;
	while (dirs)
	{
		recursive_list(dirs->content, dirs->content_size);
		dirs = dirs->next;
	}
	closedir(dir);
	ft_lstdel_p(&begin);
}
