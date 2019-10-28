/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sleonard <sleonard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/09 16:57:16 by sleonard          #+#    #+#             */
/*   Updated: 2019/10/28 20:14:35 by sleonard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

t_list		*g_lstbuf;

void	print_file_info(t_file_info *info)
{
	ft_putchar('\n');
	if (!S_ISDIR(info->file.st_mode))
		return ;
	ft_printf("is dir: %i\n", S_ISDIR(info->file.st_mode));
	ft_printf("name: [%s]\n", info->dir->d_name);
	ft_printf("type: [%hhu]\n", info->dir->d_type);
	ft_printf("inode: [%llu]\n", info->dir->d_ino);
	ft_printf("-------------------------------\n");
	ft_printf("file size: [%lli]\n", info->file.st_size);
	ft_printf("block's alloced: [%lli]\n", info->file.st_blocks);
	ft_printf("file's flags: [%u]\n", info->file.st_flags);
	ft_printf("number of links: [%i]\n", info->file.st_nlink);
	ft_printf("file's mode: [%u]\n", info->file.st_mode);
	ft_printf("-------------------------------\n");
	ft_printf("last access: %s", ctime(&info->file.st_atimespec.tv_sec));
	ft_printf("last modification: %s", ctime(&info->file.st_mtimespec.tv_sec));
	ft_printf("last status change: %s", ctime(&info->file.st_ctimespec.tv_sec));
	ft_printf("creation time: %s", ctime(&info->file.st_birthtimespec.tv_sec));
	ft_putchar('\n');
}

inline t_dirstruct *get_dirstruct(char *name, size_t total_len, t_flag is_dir)
{
	t_dirstruct		*result;

	if (!(result = (t_dirstruct*)malloc(sizeof(t_dirstruct))))
		raise_error(ERR_MALLOC);
	result->dirname = name;
	result->total_len = total_len;
	result->is_dir = is_dir;
	return (result);
}

void	recursion_wrapeer(char *startdir, int dirname_len)
{
	recursive_btree(get_dirstruct(startdir, dirname_len, 0)); //with avl tree
//	recursive_list(startdir, dirname_len); //with lists
}

int		main(int argc, char **argv)
{
	(void)argc;
	g_lstbuf = NULL;
	recursion_wrapeer(ft_strdup(argv[1]), ft_strlen(argv[1]));
}
