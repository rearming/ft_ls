/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls_structs.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sleonard <sleonard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/25 18:07:52 by sleonard          #+#    #+#             */
/*   Updated: 2019/10/28 20:14:35 by sleonard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LS_STRUCTS_H
# define FT_LS_STRUCTS_H

typedef struct passwd			t_passwd;
typedef struct group			t_group;
typedef struct dirent			t_dirent;
typedef struct stat				t_stat;
typedef int						t_bool;

typedef struct			s_filestruct
{
	char				*filename;
	char				*full_path;
	size_t				filename_len;
	t_bool				is_dir_recursive;
	dev_t				major_nbr;
	dev_t				minor_nbr;
	nlink_t				hard_links;
	char				*user_name;
	char				*group_name;
	off_t				file_size;
	time_t				last_access;
	time_t				last_modif;
	time_t				last_change;
	mode_t				st_mode;
	blkcnt_t			st_blocks;
}						t_filestruct;

typedef	struct			s_longest_strs
{
	int					filename;
	int					h_links;
	int					user_name;
	int					group_name;
	int					file_size;
	int					major_nbr;
	int					minor_nbr;
}						t_longest_strs;

typedef	struct			s_dirstruct
{
	t_avl_tree			*tree;
	long long			total_blocks;
	t_longest_strs		longest;
	int					files_num;
}						t_dirstruct;

typedef struct			s_file_info
{
	t_dirent			*dirent;
	t_stat				file;
}						t_file_info;

typedef enum			e_sort_type
{
	BY_ALPHA,
	BY_TIME,
	BY_SIZE,
	NO_SORT
}						t_sort_type;

typedef enum			e_time_modes
{
	MODIFICATION,
	ACCESS,
	STATUS_MODIFICATION
}						t_time_modes;

typedef struct			s_options
{
	t_bool				is_verbose;
	t_bool				is_recursive;
	t_bool				sort_reverse;
	t_sort_type			sort_type;
	t_time_modes		time_mode;
	t_bool				human_readable;
	t_bool				display_hidden;
	t_bool				is_one_column;
	t_bool				is_colored;
	t_bool				hidden_no_dots;
	t_bool				dirs_like_files;
	t_bool				is_many_files;
	t_bool				is_many_dirs;
	t_bool				is_first_eol;
}						t_options;

typedef struct			s_colors
{
	const int			len;
	const int			reset_len;
	const char			*esc;
	const char			*reset;
	const char			*red;
	const char			*grey;
	const char			*yellow;
	const char			*blue;
	const char			*magenta;
	const char			*black;
	const char			*green;

	const int			bg_len;
	const char			*bg_black;
	const char			*bg_red;
	const char			*bg_green;
	const char			*bg_yellow;
	const char			*bg_cyan;
}						t_colors;

extern t_options		g_options;
extern int				g_line_len;
extern int				g_term_width;

#endif
