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

typedef struct dirent		t_dirent;
typedef struct stat			t_stat;
typedef unsigned char		t_flag;

typedef struct			s_dirstruct
{
	char				*dirname;
	size_t				total_len;
	t_flag				is_dir;
}						t_dirstruct;

typedef struct			s_file_info
{
	t_dirent			*dir;
	t_stat				file;
	long				last_access;
	long				last_modif;
	long				last_change;
	long				creation;
}						t_file_info;

#endif
