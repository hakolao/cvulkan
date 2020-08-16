/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ml_print.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/16 17:05:25 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/16 18:58:59 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libgmatrix.h"

void		ml_vector4_print(t_vec3 v)
{
	ft_printf("Vec4[%f, %f, %f]\n", v[0], v[1], v[2], v[3]);
}

void		ml_vector3_print(t_vec3 v)
{
	ft_printf("Vec3[%f, %f, %f]\n", v[0], v[1], v[2]);
}

void		ml_matrix4_print(t_mat4 m)
{
	size_t	c;
	size_t	r;

	c = -1;
	ft_printf("Matrix4:\n");
	while (++c < 4)
	{
		ft_printf("|");
		r = -1;
		while (++r < 4)
			ft_printf(" %3f |", m[c][r]);
		ft_printf("\n");
	}
}
