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
# include <pwd.h>
# include <grp.h>
# include <limits.h>

# include "libft.h"
# include "ft_ls_structs.h"
# include "ft_ls_errors.h"
# include "ft_ls_defines.h"

/*
**	Core functions
*/

t_dirstruct 	*get_dir_btree(char *dirname, size_t relative_path_name_len);
void			start_recursion(char *dirname);
void			ls_recursive(const t_filestruct *filestruct, t_flag is_first_dir);
t_options		get_options(char **args, int options_num, int *first_filename);
void			print_file_formatted(const t_filestruct *filestruct, const t_longest_strs *l_strs);
void			ls_apply_inorder(t_avl_tree *tree, void (*applyf)(const t_filestruct*,
					const t_longest_strs*), const t_longest_strs *l_strs);
/*
**	Comparers
*/
int				generic_cmpfunc(void *dir1_ptr, void *dir2_ptr);

/*
**	Helpers
*/

t_filestruct	*get_filestruct(char *relative_path_name, size_t total_len, t_flag is_dir,
							 t_file_info *file_info);
void			free_filestruct(void *filestruct_ptr);
int				is_dir_not_dot(mode_t st_mode, char *dirname);

/*
**	Error management
*/

void			raise_error(int err_code);

/*
**	Debug functions
*/

void			print_file_info(t_file_info *info);

#endif
