/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search_recursive.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sleonard <sleonard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/28 19:22:56 by sleonard          #+#    #+#             */
/*   Updated: 2019/10/28 20:51:42 by sleonard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

// 	-> ./ft_ls /  5.08s user 32.84s system 20% cpu 3:05.34 total

void	recursion_callback(void *filestruct_ptr)
{
	t_filestruct		*filestruct;

	filestruct = (t_filestruct*)filestruct_ptr;
	if (filestruct->is_dir)
		ls_recursive(filestruct, FALSE);
	else
		print_file_formatted(filestruct_ptr);
}

void	ls_recursive(void *filestruct_ptr, t_flag is_first_dir)
{
	t_filestruct		*filestruct;
	t_avl_tree		*tree;

	filestruct = (t_filestruct*)filestruct_ptr;
	if (!is_first_dir)
		ft_printf("%s%s:\n",
				g_options.is_verbose || g_options.is_one_column ? "\n" : "\n\n",
				filestruct->filename); //полный относительный путь директории, которая в данный момент читается
	tree = get_dir_btree(filestruct->filename, filestruct->total_len);
	if (tree)
	{
		btree_apply_inorder((t_btree*)tree, recursion_callback);
		free_btree(tree, free_filestruct);
	}
}
