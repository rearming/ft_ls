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

char	get_extended_attr(const char *path)
{
	char	result;
	acl_t	acl;

	result = ' ';
	if ((acl = acl_get_link_np(path, ACL_TYPE_EXTENDED)))
	{
		free(acl);
		result = '+';
	}
	if (listxattr(path, NULL, 0, XATTR_NOFOLLOW) > 0)
	{
		result = '@';
	}
	return result;
}

static inline char	*get_rights(const char *path, mode_t st_mode)
{
	char	*rights;

	if (!(rights = (char*)malloc(12)))
		raise_error(ERR_MALLOC);
	if (S_ISDIR(st_mode))
		rights[0] = 'd';
	else if (S_ISBLK(st_mode))
		rights[0] = 'b';
	else if (S_ISCHR(st_mode))
		rights[0] = 'c';
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
	rights[9] = (st_mode & S_ISVTX) ? 't' : rights[9];
	rights[10] = get_extended_attr(path);
	rights[11] = 0;
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
		ft_memcpy(&result[7], &result[15], 5);
		result[6] = ' ';
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
		return ft_strdup("");
	buff_size = (link_len != 0 ? link_len : PATH_MAX) + 4; // +4 для стрелочки [ -> ]
	if (!(link_str = (char*)malloc(sizeof(char) * buff_size + 1)))
		raise_error(ERR_MALLOC);
	buff_size = readlink(path, &link_str[4], buff_size) + 4;
	ft_memcpy(link_str, arrow, 4);
	link_str[buff_size] = '\0';
	return (link_str);
}

inline void	print_entry_dir_path(const char *filename, t_flag is_first_dir,
								 char *prefix_eols)
{
	if (is_first_dir)
		return ;
	ft_printf("%s%s:\n", prefix_eols, filename);
}

inline void	print_total(t_dirstruct *dirstruct)
{
	if (g_options.is_verbose && dirstruct->tree)
		ft_printf("total %lli\n", dirstruct->total_blocks);
}

static inline char *get_file_size_or_major_minor(const t_filestruct *filestruct,
									const t_longest_strs *l_strs,
									t_flag is_device)
{
	char		*field;

	if (!is_device)
	{
		ft_sprintf(&field, "%*lli", l_strs->file_size, filestruct->file_size); //todo работа с -h
		return (field);
	}
	ft_sprintf(&field, "%*i, %*i", l_strs->major_nbr,
			filestruct->major_nbr, l_strs->minor_nbr, filestruct->minor_nbr);
	return (field);
}

t_colors			g_colors =
{
	.len = 5,
	.reset_len = 4,
	.reset = "\x1b[0m",
	.red = "\x1b[31m",
	.grey = "\x1b[34m",
	.yellow = "\x1b[33m",
	.blue = "\x1b[34m",
	.magenta = "\x1b[35m",
	.black = "\x1b[30m",

	.bg_len = 5,
	.bg_black = "\x1b[40m",
	.bg_yellow = "\x1b[43m",
	.bg_green = "\x1b[42m",
	.bg_cyan = "\x1b[46m"
};

static inline const char	*choose_text_color(const char *rights)
{
	if (rights[0] == 'd')
		return (g_colors.blue);
	if (rights[0] == 'l')
		return (g_colors.magenta);
	if (rights[0] == 'b')
		return (g_colors.black);
	if (rights[0] == 'c')
		return (g_colors.grey);
	if (rights[3] == 'x' || rights[6] == 'x' || rights[9] == 'x')
		return (g_colors.red);
	return (NULL);
}

static inline const char	*choose_bg_color(const char *rights)
{
	if (rights[0] == 'c')
		return (g_colors.bg_yellow);
	if (rights[0] == 'b')
		return (g_colors.bg_cyan);
	if (rights[9] == 't')
		return (g_colors.bg_green);
	return (NULL);
}

static inline char	*get_colored_text(const char *str, const char *color,
										const char *bg_color, size_t len)
{
	char		*result;
	const int	col_len = color ? g_colors.len : 0;
	const int	bg_len = bg_color ? g_colors.bg_len : 0;
	const int	total_col_len = col_len + bg_len + (color || bg_color ? g_colors.reset_len : 0);

	if (!g_options.is_colored)
		return ((char*)str);
	if (!(result = (char*)malloc(sizeof(char)
			* (len + total_col_len + 1))))
		raise_error(ERR_MALLOC);

	if (bg_color)
		ft_memcpy(result, bg_color, bg_len);
	if (color)
		ft_memcpy(&result[bg_len], color, col_len);
	ft_memcpy(&result[col_len + bg_len], str, len);
	if (color || bg_color)
		ft_memcpy(&result[col_len + bg_len + len], g_colors.reset, g_colors.reset_len);
	result[len + total_col_len] = 0;
	return (result);
}

void	print_file_formatted(const t_filestruct *filestruct, const t_longest_strs *l_strs)
{
	char			*rights;
	char			*link_str;
	char			*size_field;
	char			*col_filename;

	filestruct = (t_filestruct*)filestruct;
	if (filestruct->is_dir_recursive)
		return ;
	rights = get_rights(filestruct->full_path, filestruct->st_mode);
	col_filename = get_colored_text(filestruct->filename,
			choose_text_color(rights), choose_bg_color(rights), filestruct->total_len);
	if (!g_options.is_verbose)
	{
		if (g_options.is_one_column)
			ft_printf("%s\n", filestruct->filename);
		else
			ft_printf("%-*s", l_strs->filename + 2, filestruct->filename); //todo сделать по размеру терминала
	}
	else
	{
		link_str = get_link_str(filestruct->full_path, rights[0] == 'l', filestruct->file_size);
		size_field = get_file_size_or_major_minor(filestruct, l_strs, rights[0] == 'c' || rights[0] == 'b');

		ft_printf("%s %*i %-*s %-*s %s %s %s%s\n",
				  rights,
				  l_strs->h_links,
				  filestruct->hard_links,
				  l_strs->user_name + 1,
				  filestruct->user_name,
				  l_strs->group_name + 1,
				  filestruct->group_name,
				  size_field,
				  get_formatted_time(filestruct),
				  col_filename,
				  link_str
		);

		free(link_str);
		free(size_field);
	}
	free(rights);
	if (g_options.is_colored)
		free(col_filename);
}
