/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/07 17:11:46 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/09 20:04:45 by ohakola          ###   ########.fr       */
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

static void		vulkan_init(t_cvulkan *app)
{
	vulkan_create_instance(app);
	vulkan_setup_debug_messenger(app);
	vulkan_create_surface(app);
	vulkan_pick_physical_device(app);
}

static void		scleanup(t_cvulkan *app)
{
	if (ENABLE_VALIDATION_LAYERS) {
		destroy_debug_utils_messenger_ext(app->vk_instance,
			app->vk_debug_utils_messenger, NULL);
	}
	vkDestroySurfaceKHR(app->vk_instance, app->vk_surface, NULL);
	vkDestroyInstance(app->vk_instance, NULL);

	// SDL_DestroyWindow(app->window); // seems to segfault
	// IMG_Quit();
	SDL_Quit();
}

void			app_run(t_cvulkan *app)
{
	error_check(SDL_Init(SDL_INIT_VIDEO) != 0, SDL_GetError());
	window_init(app);
	vulkan_init(app);
	main_loop(app);
	scleanup(app);
}
