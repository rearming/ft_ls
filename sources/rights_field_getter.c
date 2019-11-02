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

static inline char	get_user_exec_right(unsigned int mode)
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

static inline char	get_group_exec_right(unsigned int mode)
{
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

	if (type == USER)
		return (get_user_exec_right(filestruct->st_mode));
	if (type == GROUP)
		return (get_group_exec_right(filestruct->st_mode));
	if (mode & S_ISVTX && mode & S_IXOTH)
		return ('t');
	else if (mode & S_ISVTX && !(mode & S_IXOTH))
		return ('T');
	return ('-');
}

inline char			*get_rights(const char *path,
						const t_filestruct *filestruct)
{
	char	*rights;

	if (!(rights = (char*)malloc(12)))
		raise_error(ERR_MALLOC);
	rights[0] = S_ISDIR(filestruct->st_mode) ? 'd' : '-';
	if (S_ISBLK(filestruct->st_mode))
		rights[0] = 'b';
	else if (S_ISCHR(filestruct->st_mode))
		rights[0] = 'c';
	else if (S_ISLNK(filestruct->st_mode))
		rights[0] = 'l';
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
