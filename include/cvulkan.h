/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cvulkan.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/07 16:54:33 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/08 17:38:19 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CVULKAN_H

#define WIDTH 1280
#define HEIGHT 720
#define NAME "CVulkan"

# define EXIT_FAILURE 1
# define EXIT_SUCCESS 0

# define ENABLE_VALIDATION_LAYERS 1

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_vulkan.h>
#include <vulkan/vulkan.h>
# include "libft.h"

typedef struct		s_window_info {
	uint32_t		window_id;
	bool			is_hidden;
	void			*parent;
}					t_window_info;

typedef struct		s_cvulkan {
	bool						is_running;
	bool						frame_buffer_resized;
	SDL_Window					*window;
	t_window_info				window_info;
	VkInstance					vk_instance;
	uint32_t					vk_enabled_extension_count;
	uint32_t					vk_enabled_layer_count;
	VkDebugUtilsMessengerEXT	vk_debug_utils_messenger;
	char*						vk_extension_names[64];
	char*						vk_enabled_layers[64];
}					t_cvulkan;

/*
** Debugging
*/
void				error_check(int test, const char *message);
void				populate_debug_messenger_create_info(
					VkDebugUtilsMessengerCreateInfoEXT *create_info);
void				vulkan_setup_debug_messenger(t_cvulkan *app);
void				destroy_debug_utils_messenger_ext(
					VkInstance instance,
					VkDebugUtilsMessengerEXT p_debug_messenger,
					const VkAllocationCallbacks *p_allocator);

/*
** App
*/
void				app_run(t_cvulkan *app);

/*
** Window
*/
void				window_init(t_cvulkan *app);

/*
** Vulkan instance
*/
void				vulkan_create_instance(t_cvulkan *app);
void				populate_create_info(
					t_cvulkan *app, VkInstanceCreateInfo *create_info,
					VkApplicationInfo *app_info,
					VkDebugUtilsMessengerCreateInfoEXT *debug_create_info);

#endif
