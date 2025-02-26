/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rights_field_getter.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sleonard <sleonard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/28 19:22:56 by sleonard          #+#    #+#             */
/*   Updated: 2019/10/28 20:51:42 by sleonard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static inline char	get_extended_attr(const char *path)
{
	char	result;
	acl_t	acl;

	result = ' ';
	if ((acl = acl_get_link_np(path, ACL_TYPE_EXTENDED)))
	{
		free(acl);
		result = '+';
	}
	if (listxattr(path, NULL, 0, XATTR_NOFOLLOW) > 0)
	{
		result = '@';
	}
	return (result);
}

static inline char	get_user_or_group_exec_right(unsigned int mode, int type)
{
	if (type == USER)
	{
		if (mode & S_ISUID && mode & S_IXUSR)
			return ('s');
		else if (mode & S_ISUID && !(mode & S_IXUSR))
			return ('S');
		else if (!(mode & S_ISUID) && mode & S_IXUSR)
			return ('x');
		else
			return ('-');
	}
	if (mode & S_ISGID && mode & S_IXGRP)
		return ('s');
	else if (mode & S_ISGID && !(mode & S_IXGRP))
		return ('S');
	else if (!(mode & S_ISGID) && mode & S_IXGRP)
		return ('x');
	else
		return ('-');
}

static inline char	get_exec_right(const t_filestruct *filestruct, char type)
{
	const unsigned int mode = filestruct->st_mode;

	if (type == USER || type == GROUP)
		return (get_user_or_group_exec_right(filestruct->st_mode, type));
	if (mode & S_ISVTX && mode & S_IXOTH)
		return ('t');
	else if (mode & S_ISVTX && !(mode & S_IXOTH))
		return ('T');
	if (mode & S_IXUSR && mode & S_IXGRP)
		return ('x');
	return ('-');
}

static inline char	get_file_type(unsigned int mode)
{
	if (S_ISDIR(mode))
		return ('d');
	else if (S_ISBLK(mode))
		return ('b');
	else if (S_ISCHR(mode))
		return ('c');
	else if (S_ISLNK(mode))
		return ('l');
	else if (S_ISSOCK(mode))
		return ('s');
	else if (S_ISFIFO(mode))
		return ('p');
	else
		return ('-');
}

inline char			*get_rights(const char *path,
						const t_filestruct *filestruct)
{
	char	*rights;

	if (!(rights = (char*)malloc(12)))
		raise_error(ERR_MALLOC);
	rights[0] = get_file_type(filestruct->st_mode);
	rights[1] = (filestruct->st_mode & S_IRUSR) ? 'r' : '-';
	rights[2] = (filestruct->st_mode & S_IWUSR) ? 'w' : '-';
	rights[3] = get_exec_right(filestruct, USER);
	rights[4] = (filestruct->st_mode & S_IRGRP) ? 'r' : '-';
	rights[5] = (filestruct->st_mode & S_IWGRP) ? 'w' : '-';
	rights[6] = get_exec_right(filestruct, GROUP);
	rights[7] = (filestruct->st_mode & S_IROTH) ? 'r' : '-';
	rights[8] = (filestruct->st_mode & S_IWOTH) ? 'w' : '-';
	rights[9] = get_exec_right(filestruct, OTHER);
	rights[10] = get_extended_attr(path);
	rights[11] = 0;
	return (rights);
}
