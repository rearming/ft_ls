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
	nlink_t				hard_links;
	char*				user_name;
	char*				group_name;
	off_t				file_size;
	time_t				last_access;
	time_t				last_modif;
	time_t				last_change;
	mode_t				st_mode;
	blkcnt_t			st_blocks; //то, что показывается в total
	__uint64_t			inode;
}						t_filestruct;

typedef	struct			s_longest_strs
{
	int					filename;
	int					h_links;
	int					user_name;
	int					group_name;
	int					file_size;
}						t_longest_strs;

typedef	struct			s_dirstruct
{
	t_avl_tree			*tree;
	long long			total_blocks;
	t_longest_strs		longest;
	//todo для выравнивания по терминалу может понадобиться (кол-во файлов * макс размер имени) чтобы поставить \n
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

#endif
