/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/07 17:11:46 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/07 19:05:44 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cvulkan.h"

static void		main_loop(t_cvulkan *app) {
	app->is_running = true;
	while (app->is_running) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT)
				app->is_running = false;
		}
		// drawFrame();
	}
	// vkDeviceWaitIdle(device);
}

void			vulkan_init(t_cvulkan *app)
{
	vulkan_create_instance(app);
}

void			app_run(t_cvulkan *app)
{
	error_check(SDL_Init(SDL_INIT_VIDEO) != 0, SDL_GetError());
	window_init(app);
	vulkan_init(app);
	main_loop(app);
	// cleanup()
}
