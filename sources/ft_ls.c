/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sleonard <sleonard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/09 16:57:16 by sleonard          #+#    #+#             */
/*   Updated: 2019/10/28 20:14:35 by sleonard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

t_options	g_options;

void	start_recursion(char *dirname)
{
	t_filestruct		*filestruct;

	filestruct = get_filestruct(
			ft_strdup(dirname),
			ft_strlen(dirname),
			TRUE,NULL);
	ls_recursive(filestruct, TRUE);
	free_filestruct(filestruct);
}

void	get_directory_info(char *dirname)
{
	t_dirstruct		*dirstruct;

	dirstruct = get_dir_btree(dirname, ft_strlen(dirname));
	ls_apply_inorder(dirstruct->tree, print_file_formatted, &dirstruct->longest);
	free_btree(dirstruct->tree, free_filestruct);
	free(dirstruct);
}

void	process_dir(char *dirname)
{
	if (g_options.is_recursive)
		start_recursion(dirname);
	else
		get_directory_info(dirname);
}

int		main(int argc, char **argv)
{
	int		first_filename;
	int		i;

	first_filename = 0;
	g_options = get_options(++argv, --argc, &first_filename);
	i = first_filename;
	if (i == argc)
		process_dir(".");
	while (i < argc)
	{
		process_dir(argv[i]); // тут тоже надо в бинарое дерево заносить, потому что оно сука сортированно должно читаться
		//todo если директорий указано несколько,
		i++;
	}
	return (0);
}
