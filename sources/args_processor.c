/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_processor.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sleonard <sleonard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/10 18:58:37 by sleonard          #+#    #+#             */
/*   Updated: 2019/10/28 15:51:41 by sleonard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static inline void	loop_func(char *arg,
		t_avl_tree **files_tree, t_avl_tree **dirs_tree)
{
	t_file_info	file_info;
	int			lstat_ret;

	file_info.dirent = NULL;
	lstat_ret = lstat(arg, &file_info.file);
	if (!g_options.dirs_like_files && lstat_ret != FT_ERR
		&& (S_ISDIR(file_info.file.st_mode)
			|| (S_ISLNK(file_info.file.st_mode)
				&& file_info.file.st_size > 1)))
	{
		avl_insert_data(dirs_tree,
				get_filestruct(ft_strdup(arg),
						ft_strlen(arg), TRUE, &file_info),
				generic_cmpfunc);
		g_options.is_many_dirs++;
	}
	else
	{
		avl_insert_data(files_tree,
				get_filestruct(ft_strdup(arg), ft_strlen(arg), FALSE,
						lstat_ret == FT_ERR ? NULL : &file_info),
				generic_cmpfunc);
		g_options.is_many_files++;
	}
}

void				get_args_btrees(int argc, char **argv,
		t_avl_tree **files_tree, t_avl_tree **dirs_tree)
{
	int			i;

	i = 0;
	while (i < argc)
	{
		loop_func(argv[i], files_tree, dirs_tree);
		i++;
	}
	if (g_options.is_many_files >= 0 && g_options.is_many_dirs >= 0)
	{
		g_options.is_many_files = TRUE;
		g_options.is_many_dirs = TRUE;
	}
}

static inline void	get_max_strlen_print_errors(t_avl_tree *files_tree,
		int *l_str)
{
	DIR			*dir;

	if (!files_tree)
		return ;
	get_max_strlen_print_errors(files_tree->left, l_str);
	if (!(dir = opendir(((t_filestruct*)files_tree->data)->filename))
		&& errno != ENOTDIR
		&& !S_ISLNK(((t_filestruct*)files_tree->data)->st_mode))
	{
		ft_printf_fd(STDERR_FILENO, "ft_ls: %s: %s\n",
				((t_filestruct*)files_tree->data)->filename, strerror(errno));
		free(((t_filestruct*)files_tree->data)->filename);
		((t_filestruct*)files_tree->data)->filename = NULL;
	}
	else
		(*l_str) = ft_max(*l_str,
				((t_filestruct*)files_tree->data)->filename_len);
	if (dir)
		closedir(dir);
	get_max_strlen_print_errors(files_tree->right, l_str);
}

static void			apply_without_errors(t_avl_tree *files_tree,
		void (*applyf)(const t_filestruct *, const t_longest_strs *),
		const t_longest_strs *l_strs, t_bool *printed_files)
{
	if (!files_tree)
		return ;
	apply_without_errors(files_tree->left, applyf, l_strs, printed_files);
	if (((t_filestruct*)files_tree->data)->filename)
	{
		*printed_files = TRUE;
		applyf(files_tree->data, l_strs);
	}
	apply_without_errors(files_tree->right, applyf, l_strs, printed_files);
}

int					print_files_args(t_avl_tree *files_tree,
		t_bool is_dir_btree)
{
	int				longest_str;
	t_longest_strs	l_strs;
	t_bool			some_files_printed;

	some_files_printed = FALSE;
	if (!files_tree)
		return (some_files_printed);
	longest_str = 0;
	get_max_strlen_print_errors(files_tree, &longest_str);
	l_strs.filename = longest_str;
	if (!g_options.is_verbose && !g_options.is_one_column)
	{
		apply_without_errors(files_tree, print_file_formatted,
				&l_strs, &some_files_printed);
		if ((!g_options.is_recursive || !is_dir_btree) && some_files_printed)
			ft_putchar('\n');
	}
	return (some_files_printed);
}
