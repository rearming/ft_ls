/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sleonard <sleonard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/09 16:57:58 by sleonard          #+#    #+#             */
/*   Updated: 2019/10/28 20:14:35 by sleonard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LS_H
# define FT_LS_H

# include <stdio.h>
# include <unistd.h>
# include <dirent.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <time.h>
# include <errno.h>

# include "libft.h"
# include "ft_ls_structs.h"
# include "ft_ls_errors.h"
# include "ft_ls_defines.h"

extern t_list		*g_lstbuf;

void	print_file_info(t_file_info *info);
void	recursive_btree(void *dirstruct_ptr);
void	recursive_list(char *dirname, size_t total_len);
void	raise_error(int err_code);

t_dirstruct *get_dirstruct(char *name, size_t total_len, t_flag is_dir);

#endif
