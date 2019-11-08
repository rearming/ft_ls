/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dir_tree_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sleonard <sleonard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/28 19:22:56 by sleonard          #+#    #+#             */
/*   Updated: 2019/10/28 20:51:42 by sleonard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

inline void		init_dirstruct(t_dirstruct **p_dirstruct)
{
	if (!(*p_dirstruct = (t_dirstruct*)malloc(sizeof(t_dirstruct))))
		raise_error(ERR_MALLOC);
	(*p_dirstruct)->longest = (t_longest_strs){0, 0, 0, 0, 0, 0, 0};
	(*p_dirstruct)->total_blocks = 0;
	(*p_dirstruct)->files_num = 0;
	(*p_dirstruct)->tree = NULL;
}

inline t_bool	is_root(const char *path_name)
{
	return ((path_name[0] == '/' && path_name[1] == '\0'));
}

char			*get_new_path_name(t_file_info *file_info,
							const char *prev_path_name, size_t *total_len)
{
	char	*new_path_name;
	size_t	new_path_size;
	t_bool	root;

	root = is_root(prev_path_name);
	new_path_size = *total_len + file_info->dirent->d_namlen + !root;
	if (!(new_path_name = (char*)malloc(sizeof(char) * new_path_size + 1)))
		raise_error(ERR_MALLOC);
	ft_memcpy(new_path_name, prev_path_name, *total_len);
	if (!root)
		new_path_name[*total_len] = '/';
	ft_memcpy(&new_path_name[*total_len + !root],
			file_info->dirent->d_name, file_info->dirent->d_namlen);
	new_path_name[new_path_size] = 0;
	*total_len = new_path_size;
	return (new_path_name);
}

inline int		check_hidden(const char *filename)
{
	if (filename[0] != '.')
		return (TRUE);
	else if (((filename[0] == '.' && filename[1] == '\0')
	|| (filename[0] == '.' && filename[1] == '.' && filename[2] == '\0'))
	&& g_options.hidden_no_dots && !g_options.display_hidden)
		return (FALSE);
	else
		return (g_options.hidden_no_dots || g_options.display_hidden);
}

inline void		update_longest(t_longest_strs *l_strs,
								t_filestruct *filestruct)
{
	l_strs->filename =
			ft_max(l_strs->filename, filestruct->filename_len);
	l_strs->h_links =
			ft_max(l_strs->h_links, ft_count_digits(filestruct->hard_links));
	l_strs->user_name =
			ft_max(l_strs->user_name, ft_strlen(filestruct->user_name));
	l_strs->group_name =
			ft_max(l_strs->group_name, ft_strlen(filestruct->group_name));
	l_strs->file_size =
			ft_max(l_strs->file_size, ft_count_digits(filestruct->file_size));
	l_strs->major_nbr =
			ft_max(l_strs->major_nbr, ft_count_digits(filestruct->major_nbr));
	l_strs->minor_nbr =
			ft_max(l_strs->minor_nbr, ft_count_digits(filestruct->minor_nbr));
}
