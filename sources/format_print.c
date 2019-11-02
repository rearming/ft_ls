#include "ft_ls.h"

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
			filestruct->total_len);
	if (!g_options.is_verbose)
	{
		if (g_options.is_one_column)
			ft_printf("%s\n", colored_filename);
		else
			ft_printf("%-*s", l_strs->filename + 2, colored_filename); //todo сделать по размеру терминала
	}
	else
		print_verbose(filestruct, l_strs, colored_filename, rights);
	free(rights);
	if (g_options.is_colored)
		free(colored_filename);
}
