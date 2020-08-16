/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/16 19:37:03 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/16 23:43:21 by ohakola          ###   ########.fr       */
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
	char	res[256];

	ft_printf("Testing Matrix operations\n");
	ml_matrix4_id(id);
	mu_assert("Id matrix is wrong 1", id[0][0] == 1 &&
		id[1][1] == 1 && id[2][2] == 1 && id[3][3] == 1);
	ml_matrix4_to_str(id, res);
	mu_assert("Id matrix is wrong 2",
		ft_strequ(res,
		"Matrix4:\n"
		"| 1.000 | 0.000 | 0.000 | 0.000 |\n"
		"| 0.000 | 1.000 | 0.000 | 0.000 |\n"
		"| 0.000 | 0.000 | 1.000 | 0.000 |\n"
		"| 0.000 | 0.000 | 0.000 | 1.000 |\n"));
	return (0);
}
