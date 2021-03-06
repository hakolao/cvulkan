/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_state.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/16 19:58:24 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/16 20:01:00 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mu_test.h"

t_test_state	test_state_create(bool success, const char *result, int id)
{
	t_test_state	state;

	state = (t_test_state){.success = success, .result = result, .id = id};
	return (state);
}
