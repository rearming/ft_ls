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

void	recursion_callback(const t_filestruct *filestruct, const t_longest_strs *l_strs)
{
	if (filestruct->is_dir)
		ls_recursive(filestruct, FALSE);
	else
		print_file_formatted(filestruct, l_strs);
}

void	ls_recursive(const t_filestruct *filestruct, t_flag is_first_dir)
{
	t_dirstruct		*dirstruct;

	//todo это все вынести в отдельную generic функцию, total будет всегда (когда директория),
	// а имя директории только в случае рекурсии или нескольких указанных файлов
	if (!is_first_dir)
		ft_printf("%s%s:\n", g_options.is_verbose || g_options.is_one_column ? "\n" : "\n\n",
				filestruct->filename); //полный относительный путь директории, которая в данный момент читается
	dirstruct = get_dir_btree(filestruct->filename, filestruct->total_len);
	if (g_options.is_verbose && dirstruct->tree)
		ft_printf("total %lli\n", dirstruct->total_blocks);

	if (dirstruct->tree)
	{
		ls_apply_inorder(dirstruct->tree, recursion_callback, &dirstruct->longest);
		free_btree(dirstruct->tree, free_filestruct);
		free(dirstruct);
	}
}
