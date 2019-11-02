#include "ft_ls.h"
#include <math.h>

inline char		*get_formatted_time(const t_filestruct *filestruct)
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

inline char		*get_link_str(const char *path, t_flag is_link, off_t link_len)
{
	const char	arrow[] = " -> ";
	char		*link_str;
	size_t		buff_size;

	if (!is_link)
		return (ft_strdup(""));
	buff_size = (link_len != 0 ? link_len : PATH_MAX) + 4;
	if (!(link_str = (char*)malloc(sizeof(char) * buff_size + 1)))
		raise_error(ERR_MALLOC);
	buff_size = readlink(path, &link_str[4], buff_size) + 4;
	ft_memcpy(link_str, arrow, 4);
	link_str[buff_size] = '\0';
	return (link_str);
}

inline void		print_entry_dir_path(const char *filename, t_flag is_first_dir,
											char *prefix_eols)
{
	if (is_first_dir)
		return ;
	ft_printf("%s%s:\n", prefix_eols, filename);
}

inline void		print_total(t_dirstruct *dirstruct)
{
	if (g_options.is_verbose && dirstruct->tree)
		ft_printf("total %lli\n", dirstruct->total_blocks);
}

static inline long long ft_round(double nbr)
{
	if ((long long)(nbr + 0.5) == (long long)nbr + 1)
		return ((long long)nbr + 1);
	else
		return ((long long)nbr);
}

static inline char	*get_human_readable_size(off_t file_size)
{
	char		*result;
	const int	width = 4;

	if (file_size - 1024 < 0)
		ft_sprintf(&result, "%*lliB", width, file_size);
	else if (file_size - 1024 * 1024 < 0)
	{
		if (file_size - 1024 * 9 < 0)
			ft_sprintf(&result, "%*.1fK", width, (double)file_size / 1024);
		else
			ft_sprintf(&result, "%*lliK", width, ft_round((double)file_size / 1024));
	}
	else if (file_size - 1024 * 1024 * 1024 < 0)
	{
		if (file_size - 1024 * 1024 * 9 < 0)
			ft_sprintf(&result, "%*.1fM", width, (double)file_size / (1024 * 1024));
		else
			ft_sprintf(&result, "%*lliM", width, ft_round((double)file_size / (1024 * 1024)));
	}
	else
	{
		if (file_size - 1024L * 1024L * 1024L * 9L < 0)
			ft_sprintf(&result, "%*.1fG", width, (double)file_size / (1024 * 1024 * 1024));
		else
			ft_sprintf(&result, "%*lliG", width, ft_round((double)file_size / (1024 * 1024 * 1024)));
	}
	return (result);
}

inline char		*get_file_size_or_major_minor(const t_filestruct *filestruct,
					const t_longest_strs *l_strs, t_flag is_device)
{
	char		*field;

	if (!is_device)
	{
		if (!g_options.human_readable)
			ft_sprintf(&field, "%*lli", l_strs->file_size, filestruct->file_size);
		else
			field = get_human_readable_size(filestruct->file_size);
		return (field);
	}
	ft_sprintf(&field, "%*i, %*i", l_strs->major_nbr,
		filestruct->major_nbr, l_strs->minor_nbr, filestruct->minor_nbr);
	return (field);
}
