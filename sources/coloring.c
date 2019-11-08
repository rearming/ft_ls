/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coloring.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sleonard <sleonard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/09 16:57:58 by sleonard          #+#    #+#             */
/*   Updated: 2019/10/28 20:14:35 by sleonard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

const t_colors		g_colors =
{
	.len = 5,
	.reset_len = 4,
	.reset = "\x1b[0m",
	.red = "\x1b[31m",
	.grey = "\x1b[34m",
	.yellow = "\x1b[33m",
	.blue = "\x1b[34m",
	.magenta = "\x1b[35m",
	.black = "\x1b[30m",
	.green = "\x1b[32m",

	.bg_len = 5,
	.bg_black = "\x1b[40m",
	.bg_red = "\x1b[41m",
	.bg_yellow = "\x1b[43m",
	.bg_green = "\x1b[42m",
	.bg_cyan = "\x1b[46m"
};

inline const char	*choose_text_color(const char *rights)
{
	if (rights[0] == 's')
		return (g_colors.green);
	if (rights[0] == 'p')
		return (g_colors.yellow);
	if (rights[0] == 'd')
		return (g_colors.blue);
	if (rights[0] == 'l')
		return (g_colors.magenta);
	if (rights[0] == 'b' || rights[3] == 's' || rights[6] == 's')
		return (g_colors.black);
	if (rights[0] == 'c')
		return (g_colors.grey);
	if ((rights[3] == 'x' || rights[6] == 'x' || rights[9] == 'x')
		&& rights[3] != 's' && rights[6] != 's'
		&& rights[3] != 'S' && rights[6] != 'S')
		return (g_colors.red);
	return (NULL);
}

inline const char	*choose_bg_color(const char *rights)
{
	if (rights[0] == 'c')
		return (g_colors.bg_yellow);
	if (rights[0] == 'b')
		return (g_colors.bg_cyan);
	if (rights[9] == 't')
		return (g_colors.bg_green);
	if (rights[3] == 's')
		return (g_colors.bg_red);
	if (rights[6] == 's')
		return (g_colors.bg_cyan);
	return (NULL);
}

inline char			*get_colored_text(const char *str, const char *color,
						const char *bg_color, size_t len)
{
	char		*result;
	const int	col_len = color ? g_colors.len : 0;
	const int	bg_len = bg_color ? g_colors.bg_len : 0;
	int			total_col_len;

	total_col_len = col_len + bg_len +
			(color || bg_color ? g_colors.reset_len : 0);
	if (!g_options.is_colored)
		return ((char*)str);
	if (!(result = (char*)malloc(sizeof(char)
		* (len + total_col_len + 1))))
		raise_error(ERR_MALLOC);
	if (bg_color)
		ft_memcpy(result, bg_color, bg_len);
	if (color)
		ft_memcpy(&result[bg_len], color, col_len);
	ft_memcpy(&result[col_len + bg_len], str, len);
	if (color || bg_color)
		ft_memcpy(&result[col_len + bg_len + len],
				g_colors.reset, g_colors.reset_len);
	result[len + total_col_len] = 0;
	return (result);
}
