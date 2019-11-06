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
	if (!g_options.is_verbose && !g_options.is_one_column)
		ft_putchar('\n');
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

static inline void			get_args_btrees(int argc, char **argv,
		t_avl_tree **files_tree, t_avl_tree **dirs_tree)
{
	DIR			*dir;
	t_stat		stat;
	int			lstat_ret;
	int			i;

	i = 0;
	while (i < argc)
	{
		lstat_ret = lstat(argv[i], &stat);
		if (!(dir = opendir(argv[i])) && errno != ENOTDIR)
			ft_printf_fd(STDERR_FILENO,	"ft_ls: %s: %s\n", argv[i], strerror(errno));
		else if (!g_options.dirs_like_files && lstat_ret != FT_ERR && S_ISDIR(stat.st_mode))
			avl_insert_data(dirs_tree,
					get_filestruct(ft_strdup(argv[i]), ft_strlen(argv[i]), TRUE, NULL),
					generic_cmpfunc);
		else
			avl_insert_data(files_tree,
					get_filestruct(ft_strdup(argv[i]), ft_strlen(argv[i]),FALSE,NULL),
					generic_cmpfunc);
		if (dir)
			closedir(dir);
		i++;
	}
}

void						print_files_args(t_avl_tree *files_tree)
{
	int				longest_str;
	t_longest_strs	l_strs;

	if (!files_tree)
		return ;
	longest_str = 0;
	l_strs.filename = longest_str;
	ls_apply_inorder(files_tree, print_file_formatted, &l_strs);
	if (!g_options.is_one_column)
		ft_putchar('\n');
}

int							main(int argc, char **argv)
{
	t_avl_tree		*files_tree;
	t_avl_tree		*dirs_tree;
	int				first_filename;
	struct winsize	winsize;

	files_tree = NULL;
	dirs_tree = NULL;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &winsize);
	g_term_width = winsize.ws_col;
	first_filename = 0;
	get_options(++argv, --argc, &first_filename);
	if (first_filename + 1 < argc)
		g_options.is_many_args = TRUE;
	if (first_filename == argc)
		process_dir(&(t_filestruct){.filename = ".", .is_dir_recursive = TRUE});

	get_args_btrees(argc - first_filename, &argv[first_filename], &files_tree, &dirs_tree);

	if (!g_options.is_verbose && !g_options.is_one_column)
		print_files_args(files_tree);
	else
		btree_apply_inorder((t_btree*)files_tree, process_dir);
	btree_apply_inorder((t_btree*)dirs_tree, process_dir);

	free_btree(dirs_tree, free_filestruct);
	free_btree(files_tree, free_filestruct);
	return (0);
}
