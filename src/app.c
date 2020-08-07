/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/07 17:11:46 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/07 17:13:25 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cvulkan.h"

static int 		app_init(t_cvulkan *app)
{
	app->error = "Hello!";
	return (1);
}

int		app_run(t_cvulkan *app)
{
	return (app_init(app));
}
