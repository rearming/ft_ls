/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sleonard <sleonard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/09 16:57:16 by sleonard          #+#    #+#             */
/*   Updated: 2019/08/09 20:47:44 by sleonard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	print_file_info(t_file_info *info)
{
	ft_putchar('\n');
	ft_printf("is dir: %i\n", S_ISDIR(info->file.st_mode));
	ft_printf("name: [%s]\n", info->dir->d_name);
	ft_printf("type: [%hhu]\n", info->dir->d_type);
	ft_printf("inode: [%llu]\n", info->dir->d_ino);
	ft_printf("-------------------------------\n");
	ft_printf("file size: [%lli]\n", info->file.st_size);
	ft_printf("block's alloced: [%lli]\n", info->file.st_blocks);
	ft_printf("file's flags: [%u]\n", info->file.st_flags);
	ft_printf("file's mode: [%u]\n", info->file.st_mode);
	ft_printf("-------------------------------\n");
	ft_printf("last access: [%s]\n", ctime(&info->file.st_atimespec.tv_sec));
	ft_printf("last modification: [%s]\n", ctime(&info->file.st_mtimespec.tv_sec));
	ft_printf("last status change: [%s]\n", ctime(&info->file.st_ctimespec.tv_sec));
	ft_printf("creation time: [%s]\n", ctime(&info->file.st_birthtimespec.tv_sec));
	ft_putchar('\n');
}

int		main(int argc, char **argv)
{
	DIR				*dir;
	t_file_info		info;
	int				ret;

	dir = opendir(argv[1]);
	while ((info.dir = readdir(dir)))
	{
		ret = stat(info.dir->d_name, &info.file);
		if (ret != FT_ERR)
			print_file_info(&info);
	}
	closedir(dir);
}
