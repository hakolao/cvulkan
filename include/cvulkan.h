/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cvulkan.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/07 16:54:33 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/07 17:11:29 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CVULKAN_H

# define EXIT_FAILURE 1
# define EXIT_SUCCESS 0

# include "libft.h"

typedef struct	s_cvulkan {
	char		*error;
}				t_cvulkan;

int			app_run(t_cvulkan *app);

#endif
