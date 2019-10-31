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

void get_directory_info(const char *filename, t_flag is_dir)
{
	t_dirstruct		*dirstruct;

	if (is_dir == TRUE)
		print_entry_dir_path(filename, !g_options.is_many_args, "\n");
	dirstruct = get_dir_btree(filename, ft_strlen(filename));
	if (is_dir == TRUE)
		print_total(dirstruct);
	ls_apply_inorder(dirstruct->tree, print_file_formatted, &dirstruct->longest);
	free_btree(dirstruct->tree, free_filestruct);
	free(dirstruct);
}

void	process_dir(void *filestruct_ptr)
{
	const t_filestruct	*filestruct = ((t_filestruct*)filestruct_ptr);
	const char			*filename = filestruct->filename;

	if (g_options.is_recursive)
		start_recursion(filename);
	else
		get_directory_info(filename, filestruct->is_dir_recursive);
}

int		main(int argc, char **argv)
{
	t_avl_tree	*all_params;
	int			first_filename;
	int			i;

	all_params = NULL;
	first_filename = 0;
	g_allowed_options = "lrR1hauctSf";
	g_options = get_options(++argv, --argc, &first_filename);
	i = first_filename;
	if (i + 1 < argc)
		g_options.is_many_args = TRUE;
	if (i == argc)
		process_dir(&(t_filestruct){.filename = ".", .is_dir_recursive = TRUE});
	while (i < argc)
	{
		t_stat		stat;
		const int lstat_ret = lstat(argv[i], &stat);
		avl_insert_data(&all_params,
				get_filestruct(ft_strdup(argv[i]), ft_strlen(argv[i]), lstat_ret == FT_ERR ? FT_ERR : S_ISDIR(stat.st_mode), NULL),
				generic_cmpfunc);
		i++;
	}
	btree_apply_inorder((t_btree*)all_params, process_dir);
	free_btree(all_params, free_filestruct);
	return (0);
}
