/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sleonard <sleonard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/10 18:58:37 by sleonard          #+#    #+#             */
/*   Updated: 2019/10/28 15:51:41 by sleonard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	raise_error(int err_code)
{
	if (err_code == ERR_LSTAT)
		ft_printf("lstat() failed, errno: [%s]\n", strerror(errno));
	else if (err_code == ERR_READLINK)
		ft_printf("readlink() failed, errno: [%s]\n", strerror(errno));
	else
	{
		ft_printf_fd(STDERR_FILENO,
				"ft_ft_ls: illegal option -- %c\n", err_code);
		ft_printf_fd(STDERR_FILENO,
				"usage: ft_ls [%s] [file ...]\n", ALLOWED_OPTIONS);
	}
	exit(1);
}
