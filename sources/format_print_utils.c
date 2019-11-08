/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   format_print_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sleonard <sleonard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/28 19:22:56 by sleonard          #+#    #+#             */
/*   Updated: 2019/10/28 20:51:42 by sleonard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static inline time_t	get_timestamp(const t_filestruct *filestruct)
{
	if (g_options.time_mode == MODIFICATION)
		return (filestruct->last_modif);
	if (g_options.time_mode == ACCESS)
		return (filestruct->last_access);
	if (g_options.time_mode == STATUS_MODIFICATION)
		return (filestruct->last_change);
	return (0);
}

inline char				*get_formatted_time(const t_filestruct *filestruct)
{
	char				*result;
	const time_t		timestamp = get_timestamp(filestruct);

	result = ctime(&timestamp);
	result = &result[4];
	if (time(NULL) - timestamp >= SIX_MONTH
	|| timestamp - time(NULL) >= ONE_HOUR)
	{
		if (ft_atoi(&result[20]) < 9999)
		{
			ft_memcpy(&result[7], &result[15], 5);
			result[12] = '\0';
		}
		else
		{
			ft_memcpy(&result[8], &result[20], 6);
			result[13] = '\0';
		}
		result[6] = ' ';
		result[7] = ' ';
	}
	else
		result[12] = '\0';
	return (result);
}

inline char				*get_link_str(const char *path, t_bool is_link,
		off_t link_len)
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

inline void				print_entry_dir_path(const char *filename,
		t_bool is_first_dir, char *prefix_eols)
{
	if (is_first_dir)
		return ;
	ft_printf("%s%s:\n", prefix_eols, filename);
}
