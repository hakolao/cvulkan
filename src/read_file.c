/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/10 13:42:18 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/10 14:00:20 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cvulkan.h"

char	*read_file(char *filename)
{
	char	*line;
	char	*contents;
	char	*tmp;
	int		fd;
	int		i;

	if ((fd = open(filename, O_RDONLY)) == -1 &&
		ft_dprintf(2, "Failed to open file %s\n", filename))
		error_check(true, "");
	contents = ft_strnew(0);
	i = 0;
	while (get_next_line(fd, &line) > 0)
	{
		tmp = contents;
		contents = ft_strjoin(contents, i++ > 0 ? "\n" : "");
		ft_strdel(&tmp);
		tmp = contents;
		contents = ft_strjoin(contents, line);
		ft_strdel(&tmp);
		ft_strdel(&line);
	}
	if ((fd = close(fd)) == -1 &&
		ft_dprintf(2, "Failed to close file %s\n", filename))
		error_check(true, "");
	return (contents);
}
