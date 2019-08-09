/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_structs.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sleonard <sleonard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/09 18:51:05 by sleonard          #+#    #+#             */
/*   Updated: 2019/08/09 20:16:37 by sleonard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_STRUCTS_H
# define FT_STRUCTS_H

typedef struct dirent		t_dirent;
typedef struct stat			t_stat;

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
