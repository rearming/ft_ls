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

static inline void			get_directory_info(const char *filename,
		t_bool is_dir)
{
	t_dirstruct		*dirstruct;

	if (is_dir == TRUE)
		print_entry_dir_path(filename,
				!g_options.is_many_files || !g_options.is_many_dirs,
				!g_options.is_first_eol ? "" : "\n");
	g_options.is_first_eol = TRUE;
	dirstruct = get_dir_btree(filename, ft_strlen(filename));
	if (is_dir == TRUE && !g_options.dirs_like_files)
		print_total(dirstruct, filename);
	ls_apply_inorder(dirstruct->tree,
			print_file_formatted, &dirstruct->longest);
	if (!g_options.is_verbose && !g_options.is_one_column)
		ft_putchar('\n');
	free_btree(dirstruct->tree, free_filestruct);
	free(dirstruct);
}

inline void					process_dir(void *filestruct_ptr)
{
	const t_filestruct	*filestruct = ((t_filestruct*)filestruct_ptr);
	const char			*filename = filestruct->filename;

	if (!filename)
		return ;
	if (g_options.is_recursive && filestruct->is_dir_recursive)
		start_recursion(filename);
	else
		get_directory_info(filename, filestruct->is_dir_recursive);
}

int							main(int argc, char **argv)
{
	t_avl_tree		*files_tree;
	t_avl_tree		*dirs_tree;
	int				first_filename;
	t_bool			some_files_printed;

	get_winsize();
	files_tree = NULL;
	dirs_tree = NULL;
	first_filename = 0;
	get_options(++argv, --argc, &first_filename);
	if (first_filename == argc)
		return (process_no_args());
	get_args_btrees(argc - first_filename,
			&argv[first_filename], &files_tree, &dirs_tree);
	some_files_printed = print_files_args(files_tree, dirs_tree ? TRUE : FALSE);
	g_options.is_first_eol = some_files_printed;
	if (g_options.is_verbose || g_options.is_one_column)
		btree_apply_inorder((t_btree*)files_tree, process_dir);
	btree_apply_inorder((t_btree*)dirs_tree, process_dir);
	if (g_options.is_recursive && dirs_tree
	&& !g_options.is_verbose && !g_options.is_one_column)
		ft_putchar('\n');
	free_btree(dirs_tree, free_filestruct);
	free_btree(files_tree, free_filestruct);
	return (0);
}
