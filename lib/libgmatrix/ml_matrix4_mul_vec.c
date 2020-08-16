/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ml_matrix4_mul_vec.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/16 17:24:58 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/16 17:59:14 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libgmatrix.h"

void		ml_matrix4_mul_vec3(t_mat4 m, t_vec3 v, t_vec3 res)
{
	size_t		r;
	size_t		c;
	size_t		i;
	t_vec4		v4;

	ml_vector3_to_vec4(v, v4);
	c = -1;
	while (++c < 4)
	{
		r = -1;
		while (++r < 4)
			res[c] += m[c][r] * v4[c];
	}
	i = -1;
	while (++i < 3)
		v[i] = v4[i];
}

void		ml_matrix4_mul_vec4(t_mat4 m, t_vec4 v, t_vec4 res)
{
	size_t		r;
	size_t		c;
	t_vec4		v4;

	c = -1;
	while (++c < 4)
	{
		r = -1;
		while (++r < 4)
			res[c] += m[c][r] * v4[c];
	}
}
