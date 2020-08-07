/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/07 16:47:31 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/07 17:13:28 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cvulkan.h"

int main(void)
{
	t_cvulkan	app;

	if (!(app_run(&app)))
		return (EXIT_SUCCESS);
	else
	{
		ft_printf(app.error);
		return (EXIT_FAILURE);
	}
}

