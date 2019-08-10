/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search_recursive.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sleonard <sleonard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/10 13:46:07 by sleonard          #+#    #+#             */
/*   Updated: 2019/08/10 18:47:32 by sleonard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

t_list	*g_lstbuf;

char	*get_new_name(t_file_info *info, char *prev, int *total_len)
{
	char	*new;
	char	*temp;

	if (!(temp = (char*)ft_memalloc(sizeof(char) * (*total_len + (!ft_strequ(prev, "/") ? 2 : 1)))))
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
*/

/*
 *	todo сделать буфер (листы очень медленные, тк получается куча нод)
*/

void	search_recursive(char *dirname, int total_len)
{
	t_file_info		info;
	DIR				*dir;
	t_list			*list;
	t_list			*tmp;
	char			*name;

	dir = opendir(dirname);
	if (!dir)
	{
		free(dirname);
		return ;
	}
//	ft_lstaddback(&g_lstbuf, dirname, total_len + 1);
//	ft_putstr(dirname);
	write(1, dirname, total_len);
	write(1, "\n", 1);
//	ft_putchar('\n');
//	ft_lstaddback(&g_lstbuf, "\n", 2);
	list = NULL;
	while ((info.dir = readdir(dir)))
	{
		name = get_new_name(&info, dirname, &total_len);
		lstat(name, &info.file);
		if (!ft_strequ(info.dir->d_name, ".") && !ft_strequ(info.dir->d_name, ".."))
		{
//			ft_lstaddback(&g_lstbuf, info.dir->d_name, info.dir->d_namlen + 1);
//			ft_lstaddback(&g_lstbuf, "\n", 2);
			write(1, info.dir->d_name, info.dir->d_namlen);
			write(1, "\n", 1);
		}
		if (S_ISDIR(info.file.st_mode) && !ft_strequ(info.dir->d_name, ".")
		&& !ft_strequ(info.dir->d_name, ".."))
			ft_lstaddback(&list, name, total_len + 1);
		total_len -= info.dir->d_namlen + (ft_strequ(dirname, "/") ? 0 : 1);
		free(name);
	}
	ft_putchar('\n');
//	ft_lstaddback(&g_lstbuf, "\n", 2);
	while (list)
	{
		search_recursive((char *)list->content, list->content_size - 1);
		tmp = list;
		list = list->next;
		free(tmp);
	}
	free(dirname);
	closedir(dir);
}
