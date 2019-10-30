#include "ft_ls.h"

void	ls_apply_inorder(t_avl_tree *tree, void (*applyf)(const t_filestruct*,
		 	const t_longest_strs*), const t_longest_strs *l_strs)
{
	if (!tree)
		return ;
	ls_apply_inorder(tree->left, applyf, l_strs);
	applyf(tree->data, l_strs);
	ls_apply_inorder(tree->right, applyf, l_strs);
}

static inline char	*get_rights(mode_t st_mode)
{
	char	*rights;

	rights = (char*)malloc(11);
	if (S_ISDIR(st_mode))
		rights[0] = 'd';
	else if (S_ISLNK(st_mode))
		rights[0] = 'l';
	else
		rights[0] = '-';
	rights[1] = (st_mode & S_IRUSR) ? 'r' : '-';
	rights[2] = (st_mode & S_IWUSR) ? 'w' : '-';
	rights[3] = (st_mode & S_IXUSR) ? 'x' : '-';
	rights[4] = (st_mode & S_IRGRP) ? 'r' : '-';
	rights[5] = (st_mode & S_IWGRP) ? 'w' : '-';
	rights[6] = (st_mode & S_IXGRP) ? 'x' : '-';
	rights[7] = (st_mode & S_IROTH) ? 'r' : '-';
	rights[8] = (st_mode & S_IWOTH) ? 'w' : '-';
	rights[9] = (st_mode & S_IXOTH) ? 'x' : '-';
	rights[10] = 0;
	return rights;
}

static inline char	*get_formatted_time(const t_filestruct *filestruct)
{
	char		*result;
	time_t		timestamp;

	if (g_options.time_mode == MODIFICATION)
		timestamp = filestruct->last_modif;
	if (g_options.time_mode == ACCESS)
		timestamp = filestruct->last_access;
	if (g_options.time_mode == STATUS_MODIFICATION)
		timestamp = filestruct->last_change;
	result = ctime(&timestamp);
	result[24] = 0;
	result = &result[4];
	if (time(NULL) - timestamp >= SIX_MONTH)
	{
		ft_memcpy(&result[6], &result[15], 5);
		result[11] = ' ';
		result[12] = '\0';
	}
	else
		result[12] = '\0';
	return (result);
}

static inline char *get_link_str(const char *path, t_flag is_link, off_t link_len)
{
	char		arrow[] = " -> ";
	char		*link_str;
	size_t		buff_size;

	if (!is_link)
		return "";
	buff_size = (link_len != 0 ? link_len : PATH_MAX) + 4; // +4 для стрелочки [ -> ]
	link_str = (char*)malloc(sizeof(char) * buff_size + 1); //todo нужна ли проверка на ошибку (-1) ?
	buff_size = readlink(path, &link_str[4], buff_size) + 4;
	ft_memcpy(link_str, arrow, 4);
	link_str[buff_size] = '\0';
	return (link_str);
}

void	print_file_formatted(const t_filestruct *filestruct, const t_longest_strs *l_strs)
{
	char				*rights;

	filestruct = (t_filestruct*)filestruct;
	if (filestruct->is_dir)
		return ;
	if (!g_options.is_verbose)
	{
		if (g_options.is_one_column)
			ft_printf("%s\n", filestruct->filename);
		else
			ft_printf("%-*s", l_strs->filename + 1, filestruct->filename); //todo сделать по размеру терминала
		return ;
	}
	rights = get_rights(filestruct->st_mode);
	printf("%s %*i %*s %*s %*lli %s %s%s\n",
			rights,
			l_strs->h_links + 1,
			filestruct->hard_links,
			l_strs->user_name,
			filestruct->user_name,
			l_strs->group_name + 1,
			filestruct->group_name,
			l_strs->file_size + 1,
		    filestruct->file_size,
		    get_formatted_time(filestruct),
			filestruct->filename,
			get_link_str(filestruct->full_path, rights[0] == 'l', filestruct->file_size)
			);
	free(rights);
}
