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

t_options	g_options;
int			g_longest_filename = 0;

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
	ft_printf("number of links: [%i]\n", info->file.st_nlink);
	ft_printf("file's mode: [%u]\n", info->file.st_mode);
	ft_printf("-------------------------------\n");
	ft_printf("last access: %s", ctime(&info->file.st_atimespec.tv_sec));
	ft_printf("last modification: %s", ctime(&info->file.st_mtimespec.tv_sec));
	ft_printf("last status change: %s", ctime(&info->file.st_ctimespec.tv_sec));
	ft_printf("creation time: %s", ctime(&info->file.st_birthtimespec.tv_sec));
	ft_putchar('\n');
}

void	start_recursion(char *dirname)
{
	t_filestruct		*filestruct;

	filestruct = get_filestruct(
			ft_strdup(dirname),
			ft_strlen(dirname),
			TRUE,NULL);
	ls_recursive(filestruct, TRUE);
	free_filestruct(filestruct);
}



void	get_directory_info(char *dirname)
{
	t_avl_tree		*tree;

	tree = get_dir_btree(dirname, ft_strlen(dirname));
	btree_apply_inorder((t_btree *) tree, print_file_formatted);
	free_btree(tree, free_filestruct);
}

void	process_dir(char *dirname)
{
	if (g_options.is_recursive)
		start_recursion(dirname);
	else
		get_directory_info(dirname);
}

int		main(int argc, char **argv)
{
	int		first_filename;
	int		i;

	first_filename = 0;
	g_options = get_options(++argv, --argc, &first_filename);
	i = first_filename;
	if (i == argc)
		process_dir(".");
	while (i < argc)
	{
		process_dir(argv[i]);
		i++;
	}
	return (0);
}
