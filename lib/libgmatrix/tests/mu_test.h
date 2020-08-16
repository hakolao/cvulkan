/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mu_test.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/16 19:15:20 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/16 19:22:37 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MU_TEST_H
# define MU_TEST_H

# include "libft.h"

# define MAX_TESTS 1024

typedef struct s_test_state
{
	bool		success;
	const char	*result;
	int			id;
}				t_test_state;

t_test_state	test_state_create(bool success, const char *result, int id)
{
	t_test_state	state;

	state = (t_test_state){success, result, id};
	return (state);
}

/*
** Modified version of
** http://www.jera.com/techinfo/jtns/jtn002.html
** using vector and a TestState struct.
*/

#define mu_assert(message, test)      \
	do {                             \
		if (!(test)) return message; \
	} while (0)
#define mu_run_test(test)                                                 \
	do {                                                                  \
		const char *message = test();                                     \
		tests_run++;                                                      \
		t_test_state state =                                              \
			test_state_create(message ? false : true,                     \
				message, tests_run - 1);                                  \
		test_states[tests_run] = state;                                   \
		ft_printf(" test: %d: %s\n", state.id,                            \
			   state.success ? "Success" : state.result);                 \
	} while (0)

extern int tests_run;
extern t_test_state test_states[MAX_TESTS];

#endif
