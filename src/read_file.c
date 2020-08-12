/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/10 13:42:18 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/12 14:40:53 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cvulkan.h"

static t_file_contents	*read_while(int fd)
{
	char			buf[FILE_READ_BUF + 1];
	int				i;
	int				ret;
	void			*tmp;
	t_file_contents	*contents;

	i = 0;
	error_check(!(contents = malloc(sizeof(*contents))), "Failed to malloc!");
	while ((ret = read(fd, buf, FILE_READ_BUF)) > 0)
	{
		if (i == 0 && (contents->buf = malloc(ret)))
			ft_memcpy(contents->buf, buf, ret);
		else if (i == 0)
			error_check(true, "Failed to malloc!");
		else
		{
			tmp = contents->buf;
			error_check(!(contents->buf = malloc((i * FILE_READ_BUF + ret))),
				"Failed  to malloc!");
			ft_memcpy(contents->buf, tmp, (i * FILE_READ_BUF));
			ft_memcpy(contents->buf + i * FILE_READ_BUF, buf, ret);
			free(tmp);
		}
		contents->size = i * FILE_READ_BUF + ret;
		i++;
	}
	return (contents);
}

void					free_file_contents(t_file_contents *contents)
{
	free(contents->buf);
	free(contents);
}

t_file_contents			*read_file(const char *filename)
{
	t_file_contents	*contents;
	int				fd;

	if ((fd = open(filename, O_RDONLY)) == -1 &&
		ft_dprintf(2, "Failed to open file %s\n", filename))
		error_check(true, "");
	contents = read_while(fd);
	if ((fd = close(fd)) == -1 &&
		ft_dprintf(2, "Failed to close file %s\n", filename))
		error_check(true, "");
	return (contents);
}
