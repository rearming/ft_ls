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
# include <sys/xattr.h>
# include <sys/acl.h>
# include <sys/ioctl.h>

# include "libft.h"
# include "ft_ls_structs.h"
# include "ft_ls_errors.h"
# include "ft_ls_defines.h"

/*
**	Core functions
*/

t_dirstruct 	*get_dir_btree(const char *dirname, size_t relative_path_name_len);
void			start_recursion(const char *dirname);
void			ls_recursive(const t_filestruct *filestruct, t_flag is_first_dir);
void get_options(char **args, int options_num, int *first_filename);
void			ls_apply_inorder(t_avl_tree *tree,
		void (*applyf)(const t_filestruct*, const t_longest_strs*),
		const t_longest_strs *l_strs);
/*
**	Format print functions
*/
void			print_file_formatted(const t_filestruct *filestruct,
						const t_longest_strs *l_strs);

/*
**	Format print utils
*/
void			print_total(t_dirstruct *dirstruct);
void			print_entry_dir_path(const char *filename, t_flag is_first_dir,
						  char *prefix_eols);
char			*get_file_size_or_major_minor(const t_filestruct *filestruct,
					const t_longest_strs *l_strs, t_flag is_device);
char			*get_rights(const char *path, const t_filestruct *filestruct);
char			*get_link_str(const char *path, t_flag is_link, off_t link_len);
char			*get_formatted_time(const t_filestruct *filestruct);

/*
**	Coloring
*/

const char	*choose_text_color(const char *rights);
const char	*choose_bg_color(const char *rights);
char		*get_colored_text(const char *str, const char *color,
					const char *bg_color, size_t len);
/*
**	Comparers
*/
int				generic_cmpfunc(void *dir1_ptr, void *dir2_ptr);

/*
**	Helpers
*/

t_filestruct	*get_filestruct(char *relative_path_name, size_t total_len, t_flag is_dir_recursive,
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
