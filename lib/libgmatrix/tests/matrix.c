/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/16 19:37:03 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/17 00:22:45 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libgmatrix.h"
#include "mu_test.h"

/*
** ToDo
*/

const char	*test_matrix_ops()
{
	t_mat4	id;
	t_mat4	rot;
	char	res1[256];
	char	res2[256];

	ft_printf("Testing Matrix operations\n");
	ml_matrix4_id(id);
	mu_assert("Id matrix is wrong 1", id[0][0] == 1 &&
		id[1][1] == 1 && id[2][2] == 1 && id[3][3] == 1);
	ml_matrix4_to_str(id, res1);
	mu_assert("Id matrix is wrong 2",
		ft_strequ(res1,
		"Matrix4:\n"
		"| 1.000 | 0.000 | 0.000 | 0.000 |\n"
		"| 0.000 | 1.000 | 0.000 | 0.000 |\n"
		"| 0.000 | 0.000 | 1.000 | 0.000 |\n"
		"| 0.000 | 0.000 | 0.000 | 1.000 |\n"));
	ml_matrix4_rotate_z(id, ml_rad(90.0f), rot);
	ml_matrix4_to_str(rot, res2);
	mu_assert("rot matrix is wrong",
		ft_strequ(res2,
		"Matrix4:\n"
		"|-0.000 |-1.000 | 0.000 | 0.000 |\n"
		"| 1.000 |-0.000 | 0.000 | 0.000 |\n"
		"| 0.000 | 0.000 | 1.000 | 0.000 |\n"
		"| 0.000 | 0.000 | 0.000 | 1.000 |\n"));
	return (0);
}
