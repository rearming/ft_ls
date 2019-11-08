/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   size_print_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sleonard <sleonard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/28 19:22:56 by sleonard          #+#    #+#             */
/*   Updated: 2019/10/28 20:51:42 by sleonard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

inline void				print_total(t_dirstruct *dirstruct,
		const char *filename)
{
	t_stat	stat;
	t_bool	link_noprint;

	link_noprint = ((lstat(filename, &stat) != FT_ERR
			&& S_ISLNK(stat.st_mode) && g_options.is_verbose));
	if (g_options.is_verbose && dirstruct->tree && !link_noprint)
		ft_printf("total %lli\n", dirstruct->total_blocks);
}

static inline long long	ft_round(double nbr)
{
	if ((long long)(nbr + 0.5) == (long long)nbr + 1)
		return ((long long)nbr + 1);
	else
		return ((long long)nbr);
}

static inline void		get_human_readable_size(off_t file_size,
		char **out_size)
{
	const int	w = 4;

	if (file_size - KB < 0)
		ft_sprintf(out_size, "%*lliB", w, file_size);
	else if (file_size - KB * 1024 < 0)
	{
		if (file_size - KB_LOW < 0)
			ft_sprintf(out_size, "%*.1fK", w, (double)file_size / KB);
		else
			ft_sprintf(out_size, "%*lliK", w, ft_round((double)file_size / KB));
	}
	else if (file_size - MB * 1024 < 0)
	{
		if (file_size - MB_LOW < 0)
			ft_sprintf(out_size, "%*.1fM", w, (double)file_size / MB);
		else
			ft_sprintf(out_size, "%*lliM", w, ft_round((double)file_size / MB));
	}
	else
	{
		if (file_size - GB_LOW < 0)
			ft_sprintf(out_size, "%*.1fG", w, (double)file_size / GB);
		else
			ft_sprintf(out_size, "%*lliG", w, ft_round((double)file_size / GB));
	}
}

inline char				*get_file_size_or_major_minor(
		const t_filestruct *filestruct, const t_longest_strs *l_strs,
		t_bool is_device)
{
	char		*field;

	if (!is_device)
	{
		if (!g_options.human_readable)
			ft_sprintf(&field, "%*lli", l_strs->file_size,
					filestruct->file_size);
		else
			get_human_readable_size(filestruct->file_size, &field);
		return (field);
	}
	ft_sprintf(&field, "%*i, %*i", l_strs->major_nbr,
			filestruct->major_nbr, l_strs->minor_nbr, filestruct->minor_nbr);
	return (field);
}
