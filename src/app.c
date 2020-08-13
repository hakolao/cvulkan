/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/07 17:11:46 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/13 16:31:23 by ohakola          ###   ########.fr       */
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
		vulkan_draw(app);
	}
	vkDeviceWaitIdle(app->vk_logical_device);
}

static void		vulkan_init(t_cvulkan *app)
{
	vulkan_create_instance(app);
	vulkan_setup_debug_messenger(app);
	vulkan_create_surface(app);
	vulkan_pick_physical_device(app);
	vulkan_create_logical_device(app);
	vulkan_create_swap_chain(app);
	vulkan_create_image_views(app);
	vulkan_create_render_pass(app);
	vulkan_create_descriptor_set_layout(app);
	vulkan_create_graphics_pipeline(app);
	vulkan_create_command_pool(app);
	vulkan_create_color_resources(app);
	vulkan_create_depth_resources(app);
	vulkan_create_frame_buffers(app);
	vulkan_create_texture_image(app, TEXTURE_PATH);
	vulkan_create_texture_image_view(app);
	vulkan_create_texture_sampler(app);
	vulkan_load_model(app, MODEL_PATH);
	vulkan_create_vertex_buffer(app);
	vulkan_create_index_buffer(app);
	vulkan_create_uniform_buffers(app);
	vulkan_create_descriptor_pool(app);
	vulkan_create_descriptor_sets(app);
	vulkan_create_command_buffers(app);
	vulkan_create_sync_objects(app);
}

static void		cleanup(t_cvulkan *app)
{
	size_t	i;

	vulkan_cleanup_swap_chain(app);
	vkDestroySampler(app->vk_logical_device, app->vk_texture_sampler, NULL);
	vkDestroyImage(app->vk_logical_device, app->vk_texture_image, NULL);
	vkDestroyImageView(app->vk_logical_device,
		app->vk_texture_image_view, NULL);
	vkFreeMemory(app->vk_logical_device, app->vk_texture_image_memory, NULL);
	vkDestroyDescriptorSetLayout(app->vk_logical_device,
		app->vk_descriptor_set_layout, NULL);
	vkDestroyBuffer(app->vk_logical_device, app->vk_index_buffer, NULL);
	vkFreeMemory(app->vk_logical_device, app->vk_index_buffer_memory, NULL);
	vkDestroyBuffer(app->vk_logical_device, app->vk_vertex_buffer, NULL);
	vkFreeMemory(app->vk_logical_device, app->vk_vertex_buffer_memory, NULL);
	i = -1;
	while (++i < MAXFRAMESINFLIGHT)
	{
		vkDestroySemaphore(app->vk_logical_device,
			app->vk_rndr_finished_semaphores[i], NULL);
		vkDestroySemaphore(app->vk_logical_device,
			app->vk_img_available_semaphores[i], NULL);
		vkDestroyFence(app->vk_logical_device,
			app->vk_in_flight_fences[i], NULL);
	}
	vkDestroyCommandPool(app->vk_logical_device, app->vk_command_pool, NULL);
	vkDestroyDevice(app->vk_logical_device, NULL);
	if (ENABLE_VALIDATION_LAYERS) {
		vulkan_destroy_debug_utils_messenger_ext(app->vk_instance,
			app->vk_debug_utils_messenger, NULL);
	}
	vkDestroySurfaceKHR(app->vk_instance, app->vk_surface, NULL);
	vkDestroyInstance(app->vk_instance, NULL);
	free(app->vertices);
	free(app->indices);
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
	cleanup(app);
}
