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
													t_flag is_dir)
{
	t_dirstruct		*dirstruct;

	if (is_dir == TRUE)
		print_entry_dir_path(filename, !g_options.is_many_args, "\n");
	dirstruct = get_dir_btree(filename, ft_strlen(filename));
	if (is_dir == TRUE && !g_options.dirs_like_files)
		print_total(dirstruct);
	ls_apply_inorder(
			dirstruct->tree, print_file_formatted, &dirstruct->longest);
	free_btree(dirstruct->tree, free_filestruct);
	free(dirstruct);
}

static inline void			process_dir(void *filestruct_ptr)
{
	const t_filestruct	*filestruct = ((t_filestruct*)filestruct_ptr);
	const char			*filename = filestruct->filename;

	if (g_options.is_recursive)
		start_recursion(filename);
	else
		get_directory_info(filename, filestruct->is_dir_recursive);
}

static inline t_avl_tree	*get_args_btree(int first_filename,
											int argc, char **argv)
{
	t_avl_tree	*args_tree;
	t_stat		stat;
	int			lstat_ret;

	args_tree = NULL;
	while (first_filename < argc)
	{
		lstat_ret = lstat(argv[first_filename], &stat);
		avl_insert_data(&args_tree,
			get_filestruct(
				ft_strdup(argv[first_filename]),
				ft_strlen(argv[first_filename]),
				lstat_ret == FT_ERR ? FT_ERR : S_ISDIR(stat.st_mode),
				NULL),
			generic_cmpfunc);
		first_filename++;
	}
	return (args_tree);
}

int							main(int argc, char **argv)
{
	t_avl_tree	*args_tree;
	int			first_filename;

	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	printf("term width: [%u], height: [%u]\n", w.ws_col, w.ws_row);

	first_filename = 0;
	get_options(++argv, --argc, &first_filename);
	if (first_filename + 1 < argc)
		g_options.is_many_args = TRUE;
	if (first_filename == argc)
		process_dir(&(t_filestruct){.filename = ".", .is_dir_recursive = TRUE});
	args_tree = get_args_btree(first_filename, argc, argv);
	btree_apply_inorder((t_btree*)args_tree, process_dir);
	free_btree(args_tree, free_filestruct);
	return (0);
}
