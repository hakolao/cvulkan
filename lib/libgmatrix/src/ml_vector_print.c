/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ml_vector_print.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/16 17:05:25 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/16 17:07:46 by ohakola          ###   ########.fr       */
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
