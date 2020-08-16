/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/16 19:23:49 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/16 19:31:58 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mu_test.h"
#include "tests.h"

static void all_tests() {

}

int main(void) {
	bool			success;
	int				failures;
	int				i;
	int				tests_run;
	t_test_state	test_states[MAX_TESTS];

	tests_run = 0;
	all_tests();
	success = true;
	failures = 0;
	i = -1;
	while (++i < tests_run)
	if (!test_states[i].success)
	{
		success = false;
		failures++;
	}
	ft_printf("===========\n");
	if (!success) {
		ft_printf("%d out of %d TESTS FAILED\n", failures, tests_run);
	} else {
		ft_printf("ALL %d TESTS PASSED\n", tests_run);
	}
	return !success;
}
