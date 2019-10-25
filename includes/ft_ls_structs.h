/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls_structs.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sleonard <sleonard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/25 18:07:52 by sleonard          #+#    #+#             */
/*   Updated: 2019/10/25 18:36:27 by sleonard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LS_STRUCTS_H
# define FT_LS_STRUCTS_H

typedef struct dirent		t_dirent;
typedef struct stat			t_stat;
typedef unsigned char		t_flag;

# define MEM_BLOCK_SIZE 20000000
typedef struct			s_memblock
{
	unsigned char		*begin;
	size_t				allocated;
}						t_memblock;

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
