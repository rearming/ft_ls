/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls_recursive.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sleonard <sleonard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/28 19:22:56 by sleonard          #+#    #+#             */
/*   Updated: 2019/10/28 20:51:42 by sleonard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

int		g_line_len = 0;

void	recursion_callback(const t_filestruct *filestruct,
							const t_longest_strs *l_strs)
{
	if (filestruct->is_dir_recursive)
		ls_recursive(filestruct, FALSE);
	else
		print_file_formatted(filestruct, l_strs);
}

void	ls_recursive(const t_filestruct *filestruct, t_flag is_first_dir)
{
	t_dirstruct		*dirstruct;

	print_entry_dir_path(filestruct->filename, is_first_dir,
		g_options.is_verbose || g_options.is_one_column ? "\n" : "\n\n");
	dirstruct = get_dir_btree(filestruct->filename, filestruct->filename_len);
	print_total(dirstruct);
	if (dirstruct->tree)
	{
		ls_apply_inorder(dirstruct->tree, recursion_callback,
				&dirstruct->longest);
		free_btree(dirstruct->tree, free_filestruct);
	}
	free(dirstruct);
}

void	start_recursion(const char *dirname)
{
	t_filestruct		*filestruct;

	filestruct = get_filestruct(
			ft_strdup(dirname),
			ft_strlen(dirname),
			TRUE, NULL);
	ls_recursive(filestruct, TRUE);
	if (!g_options.is_verbose)
		ft_putchar('\n');
	free_filestruct(filestruct);
}
