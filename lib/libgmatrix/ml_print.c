/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ml_print.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/16 17:05:25 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/16 23:43:04 by ohakola          ###   ########.fr       */
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

void		ml_vector4_to_str(t_vec3 v, char *res)
{
	ft_sprintf(res, "Vec4[%f, %f, %f]\n", v[0], v[1], v[2], v[3]);
}

void		ml_vector3_to_str(t_vec3 v, char *res)
{
	ft_sprintf(res, "Vec3[%f, %f, %f]\n", v[0], v[1], v[2]);
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
			ft_printf("%6.3f |", m[r][c]);
		ft_printf("\n");
	}
}

void		ml_matrix4_to_str(t_mat4 m, char *res)
{
	ft_sprintf(res,
		"Matrix4:\n"
		"|%6.3f |%6.3f |%6.3f |%6.3f |\n"
		"|%6.3f |%6.3f |%6.3f |%6.3f |\n"
		"|%6.3f |%6.3f |%6.3f |%6.3f |\n"
		"|%6.3f |%6.3f |%6.3f |%6.3f |\n",
		m[0][0], m[1][0], m[2][0], m[3][0],
		m[0][1], m[1][1], m[2][1], m[3][1],
		m[0][2], m[1][2], m[2][2], m[3][2],
		m[0][3], m[1][3], m[2][3], m[3][3]);
}
