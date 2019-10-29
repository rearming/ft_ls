#include <pwd.h>
#include <grp.h>
#include "ft_ls.h"

static inline char	*get_rights(mode_t st_mode)
{
	char	*rights;

	rights = (char*)malloc(11);
	rights[0] = (S_ISDIR(st_mode)) ? 'd' : '-';
	rights[1] = (st_mode & S_IRUSR) ? 'r' : '-';
	rights[2] = (st_mode & S_IWUSR) ? 'w' : '-';
	rights[3] = (st_mode & S_IXUSR) ? 'x' : '-';
	rights[4] = (st_mode & S_IRGRP) ? 'r' : '-';
	rights[5] = (st_mode & S_IWGRP) ? 'w' : '-';
	rights[6] = (st_mode & S_IXGRP) ? 'x' : '-';
	rights[7] = (st_mode & S_IROTH) ? 'r' : '-';
	rights[8] = (st_mode & S_IWOTH) ? 'w' : '-';
	rights[9] = (st_mode & S_IXOTH) ? 'x' : '-';
	rights[10] = 0;
	return rights;
}

void	print_file_formatted(void *filestruct_ptr)
{
	t_filestruct		*file;
	char				*rights;
	char				*time_str;

	file = (t_filestruct*)filestruct_ptr;
	if (file->is_dir)
		return ;
	if (!g_options.is_verbose)
	{
		if (g_options.is_one_column)
			ft_printf("%s\n", file->filename);
		else
			ft_printf("%-*s", g_longest_filename + 1, file->filename); //todo сделать нормальные колонки
		return ;
	}
	char	*format = "%s\t%i\t%s\t%s\t%lli\t%s\t%s\n";
	rights = get_rights(file->st_mode);
	time_str = ft_trim_c(ctime(&file->last_modif), '\n'); //todo format date
	ft_printf(format,
			rights,
			file->hard_links,
			getpwuid(file->user_id)->pw_name,
			getgrgid(file->group_id)->gr_name,
			file->file_size,
			time_str,
			file->filename
			);
	free(rights);
	//todo "total (block size)"
}
