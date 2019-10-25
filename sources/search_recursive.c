/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search_recursive.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sleonard <sleonard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/10 13:46:07 by sleonard          #+#    #+#             */
/*   Updated: 2019/10/25 20:40:04 by sleonard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

t_list	*g_lstbuf;

/*
**	Оригинальный ls просто добавляет новый / к директориям, то есть ////usr///
**	никак не исправляется и дальше так же работает
*/

void	*alloc_mem(size_t size)
{
	void	*alloced;

	alloced = &g_memblock.begin[g_memblock.allocated];
	g_memblock.allocated += size;
	if (g_memblock.allocated > MEM_BLOCK_SIZE - 100000)
	{
		printf("OUT OF MEMORY BLOCK!\n");
		exit(4);
	}
	return (alloced);
}

char	*get_new_path_name(t_file_info *file_info, const char *prev_path_name,
						   size_t *total_len)
{
	char	*new_path_name;
	size_t	new_path_size;

	new_path_name = NULL;
	new_path_size = *total_len + file_info->dir->d_namlen + 1; // +2 для '/' и '\0'
	if (!(new_path_name = (char*)malloc(sizeof(char) * new_path_size)))
		// todo перепроверить, но походу ничего не меняется по скорости
//	if (!(new_path_name = (char*)alloc_mem(sizeof(char) * new_path_size)))
		raise_error(ERR_MALLOC);
	ft_memcpy(new_path_name, prev_path_name, *total_len);
	new_path_name[*total_len] = '/';
	ft_memcpy(&new_path_name[*total_len + 1], file_info->dir->d_name, file_info->dir->d_namlen);
	new_path_name[new_path_size] = 0;
	*total_len += file_info->dir->d_namlen;
	return (new_path_name);
}

char	*get_new_name(t_file_info *info, char *prev, int *total_len)
{
	char	*new;
	char	*temp;

	if (!(temp = (char*)malloc(sizeof(char) * (*total_len + (!ft_strequ(prev, "/") ? 2 : 1)))))
		raise_error(ERR_MALLOC);
	ft_memcpy(temp, prev, *total_len);
	if (!ft_strequ(prev, "/"))
	{
		temp[*total_len] = '/';
		*total_len += 1;
	}
	*total_len += info->dir->d_namlen;
	new = ft_strljoin(temp, info->dir->d_name, *total_len);
	free(temp);
	return (new);
}

/*
**	пока что печатает скрытые файлы и не печатает . и ..
 *  сравнивать с ls -1aR !
 *
 *  -> todo сделать буфер (листы очень медленные, тк получается куча нод)
 *
 *  -> todo info.file.st_nlink показывает кол-во файлов в директории,
 *  можно кидать в рекурсивный вызов, чтобы не юзать листы
*/

void	new_search_recoursive(char *dirname, size_t total_len, int files_in_directory)
{
	t_file_info		info;
	DIR				*dir;
	char 			*dirnames[files_in_directory]; //todo потом можно изменить на массив структур
	int				file_name_nbr;
	int				i;

	file_name_nbr = 0;
	if (!(dir = opendir(dirname)))
		return ;
	while ((info.dir = readdir(dir)))
	{
		dirnames[file_name_nbr] = get_new_path_name(&info, dirname, &total_len);
		lstat(dirnames[file_name_nbr], &info.file);
		ft_printf("%s\n", info.dir->d_name);
		if (S_ISDIR(info.file.st_mode) && !ft_strequ(info.dir->d_name, ".")
			&& !ft_strequ(info.dir->d_name, ".."))
			file_name_nbr++;
		total_len -= info.dir->d_namlen;
	}
	i = 0;
	while (i < file_name_nbr)
	{
//		search_recursive(dirnames[file_name_nbr], )
	}

}

// 	-> ./ft_ls /  5.08s user 32.84s system 20% cpu 3:05.34 total


//todo new code ->

void	search_recursive(char *dirname, size_t total_len)
{
	t_file_info		file_info;
	DIR				*dir;
	char			*name;

	if (!(dir = opendir(dirname))) // тут может сфейлиться опендир, если имя кривое (мусор в конце например)
		return ;
//	ft_printf("%s\n", dirname);
//		return (free(dirname)); // todo пока что мой аллокатор

	while ((file_info.dir = readdir(dir)))
	{
		name = get_new_path_name(&file_info, dirname, &total_len);
		lstat(name, &file_info.file);
//		if (!ft_strequ(file_info.dir->d_name, ".") && !ft_strequ(file_info.dir->d_name, ".."))
		if (S_ISDIR(file_info.file.st_mode) && !ft_strequ(file_info.dir->d_name, ".")
			&& !ft_strequ(file_info.dir->d_name, ".."))
			search_recursive(name, total_len + 1);
			// todo чтобы вывод был как у оригинального, надо после запускать в цикле
//		write(1, file_info.dir->d_name, file_info.dir->d_namlen);
//		write(1, "\n", 1);
		ft_printf("%s\n", file_info.dir->d_name);
		total_len -= file_info.dir->d_namlen;
	}
//	ft_printf("\n");
	closedir(dir);
}
