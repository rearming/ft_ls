/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sleonard <sleonard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/09 16:57:16 by sleonard          #+#    #+#             */
/*   Updated: 2019/10/25 19:21:49 by sleonard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

t_list		*g_lstbuf;
t_memblock	g_memblock;

void	print_file_info(t_file_info *info)
{
	ft_putchar('\n');
	if (!S_ISDIR(info->file.st_mode))
		return ;
	ft_printf("is dir: %i\n", S_ISDIR(info->file.st_mode));
	ft_printf("name: [%s]\n", info->dir->d_name);
	ft_printf("type: [%hhu]\n", info->dir->d_type);
	ft_printf("inode: [%llu]\n", info->dir->d_ino);
	ft_printf("-------------------------------\n");
	ft_printf("file size: [%lli]\n", info->file.st_size);
	ft_printf("block's alloced: [%lli]\n", info->file.st_blocks);
	ft_printf("file's flags: [%u]\n", info->file.st_flags);
	ft_printf("number of links: [%i]\n", info->file.st_nlink);
	ft_printf("file's mode: [%u]\n", info->file.st_mode);
	ft_printf("-------------------------------\n");
	ft_printf("last access: %s", ctime(&info->file.st_atimespec.tv_sec));
	ft_printf("last modification: %s", ctime(&info->file.st_mtimespec.tv_sec));
	ft_printf("last status change: %s", ctime(&info->file.st_ctimespec.tv_sec));
	ft_printf("creation time: %s", ctime(&info->file.st_birthtimespec.tv_sec));
	ft_putchar('\n');
}

char		*get_buff(t_list **list, size_t total_len)
{
	char		*buffer;
	int			start;
	t_list		*temp;

	start = 0;
	if (!(buffer = (char*)malloc(sizeof(char) * (total_len + 1))))
		raise_error(ERR_MALLOC);
	while (*list)
	{
		ft_memcpy(&buffer[start], (*list)->content, (*list)->content_size - 1);
		start += (*list)->content_size - 1;
		temp = (*list);
		(*list) = (*list)->next;
		free(temp->content);
		free(temp);
	}
	return (buffer);
}

int			print_lst(t_list **list, int fd)
{
	size_t		total_len;
	t_list		*tmp_count;
	char		*buffer;

	if (!list)
		raise_error(ERR_NULL_LIST);
	tmp_count = *list;
	total_len = 0;
	while (tmp_count)
	{
		total_len += tmp_count->content_size - 1;
		tmp_count = tmp_count->next;
	}
	printf("total_len: [%zu]\n", total_len);
	buffer = get_buff(&g_lstbuf, total_len);
	write(fd, buffer, total_len);
	free(buffer);
	return (total_len);
}

int		main(int argc, char **argv)
{
	(void)argc;
	g_lstbuf = NULL;
	g_memblock.begin = (unsigned char*)malloc(MEM_BLOCK_SIZE);
	g_memblock.allocated = 0;
	search_recursive(ft_strdup(argv[1]), ft_strlen(argv[1]));
	free(g_memblock.begin);
}
