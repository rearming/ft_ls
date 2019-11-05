/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   format_print.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sleonard <sleonard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/09 16:57:58 by sleonard          #+#    #+#             */
/*   Updated: 2019/10/28 20:14:35 by sleonard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

int					g_term_width;

void				ls_apply_inorder(t_avl_tree *tree,
		void (*applyf)(const t_filestruct*, const t_longest_strs*),
		const t_longest_strs *l_strs)
{
	if (!tree)
		return ;
	ls_apply_inorder(tree->left, applyf, l_strs);
	applyf(tree->data, l_strs);
	ls_apply_inorder(tree->right, applyf, l_strs);
}

void				print_verbose(const t_filestruct *filestruct,
		const t_longest_strs *l_strs,
		const char *colored_filename,
		const char *rights)
{
	char			*link_str;
	char			*size_field;

	link_str = get_link_str(filestruct->full_path,
			rights[0] == 'l', filestruct->file_size);
	size_field = get_file_size_or_major_minor(filestruct, l_strs,
			rights[0] == 'c' || rights[0] == 'b');
	ft_printf(
		"%s %*i %-*s %-*s %s %s %s%s\n",
		rights,
		l_strs->h_links,
		filestruct->hard_links,
		l_strs->user_name + 1,
		filestruct->user_name,
		l_strs->group_name + 1,
		filestruct->group_name,
		size_field,
		get_formatted_time(filestruct),
		colored_filename,
		link_str);
	free(link_str);
	free(size_field);
}

static inline void	print_no_options(const t_longest_strs *l_strs,
										const char *colored_filename)
{
	if (g_line_len + TERM_OFFSET >= g_term_width)
	{
		ft_printf("%-*s\n", l_strs->filename + 1, colored_filename);
		g_line_len = 0;
	}
	else
	{
		ft_printf("%-*s", l_strs->filename + 1, colored_filename);
		g_line_len += l_strs->filename + 1;
	}
}

void				print_file_formatted(const t_filestruct *filestruct,
											const t_longest_strs *l_strs)
{
	char			*rights;
	char			*colored_filename;

	filestruct = (t_filestruct*)filestruct;
	if (filestruct->is_dir_recursive)
		return ;
	rights = get_rights(filestruct->full_path, filestruct);
	colored_filename = get_colored_text(filestruct->filename,
			choose_text_color(rights),
			choose_bg_color(rights),
			filestruct->filename_len);
	if (!g_options.is_verbose)
	{
		if (g_options.is_one_column)
			ft_printf("%s\n", colored_filename);
		else
			print_no_options(l_strs, colored_filename);
	}
	else
		print_verbose(filestruct, l_strs, colored_filename, rights);
	free(rights);
	if (g_options.is_colored)
		free(colored_filename);
}
