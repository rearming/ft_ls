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
	if (err_code == ERR_READLINK)
		ft_printf("readlink() failed, errno: [%s]\n", strerror(errno));
	exit(err_code);
}
