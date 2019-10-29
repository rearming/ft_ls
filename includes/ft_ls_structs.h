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

typedef struct dirent			t_dirent;
typedef struct stat				t_stat;
typedef int						t_flag;
typedef int (*t_cmpfunc)(void*, void*);

typedef struct			s_filestruct
{
	char				*filename;
	char				*full_path;
	size_t				total_len;
	t_flag				is_dir;
	t_flag				is_hidden;
	long				last_access;
	long				last_modif;
	long				last_change;
	mode_t				st_mode;
	blkcnt_t			st_blocks; //то, что показывается в total
	off_t				file_size;
	__uint64_t			inode;
	nlink_t				hard_links;
	uid_t				user_id;
	gid_t				group_id;
}						t_filestruct;

typedef struct			s_file_info
{
	t_dirent			*dir;
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
	t_flag				is_verbose;
	t_flag				is_recursive;
	t_flag				sort_reverse;
	t_sort_type			sort_type;
	t_time_modes		time_mode;
	t_flag				human_readable;
	t_flag				display_hidden;
	t_flag				is_one_column;
}						t_options;

extern t_options		g_options;
extern int				g_longest_filename;

#endif
