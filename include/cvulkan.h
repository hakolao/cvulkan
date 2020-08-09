/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cvulkan.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/07 16:54:33 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/09 20:47:25 by ohakola          ###   ########.fr       */
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

typedef struct			s_queue_family_indices
{
	int32_t			graphics_family;
	int32_t			present_family;
}						t_queue_family_indices;

typedef struct			s_swap_chain_support_details {
	VkSurfaceCapabilitiesKHR	capabilities;
	VkSurfaceFormatKHR			formats[64];
	uint32_t					format_count;
	VkPresentModeKHR			present_modes[64];
	uint32_t					present_mode_count;
}						t_swap_chain_support_details;

typedef struct			s_window_info {
	uint32_t		window_id;
	bool			is_hidden;
	void			*parent;
}						t_window_info;

typedef struct			s_cvulkan {
	bool						is_running;
	bool						frame_buffer_resized;
	SDL_Window					*window;
	t_window_info				window_info;
	VkInstance					vk_instance;
	VkSurfaceKHR				vk_surface;
	VkPhysicalDevice			vk_physical_device;
	VkDevice					vk_logical_device;
	VkQueue						vk_graphics_queue;
	VkQueue						vk_present_queue;
	VkSampleCountFlagBits		vk_msaa_samples;
	uint32_t					vk_enabled_extension_count;
	uint32_t					vk_enabled_layer_count;
	uint32_t					vk_required_extension_count;
	VkDebugUtilsMessengerEXT	vk_debug_utils_messenger;
	char						*vk_extension_names[64];
	char						*vk_enabled_layers[64];
	char						*vk_required_device_extensions[64];
}						t_cvulkan;

/*
** Debugging
*/
void					error_check(int test, const char *message);
void					populate_debug_messenger_create_info(
						VkDebugUtilsMessengerCreateInfoEXT *create_info);
void					vulkan_setup_debug_messenger(t_cvulkan *app);
void					destroy_debug_utils_messenger_ext(
						VkInstance instance,
						VkDebugUtilsMessengerEXT p_debug_messenger,
						const VkAllocationCallbacks *p_allocator);

/*
** App
*/
void					app_run(t_cvulkan *app);

/*
** Window
*/
void					window_init(t_cvulkan *app);

/*
** Vulkan instance
*/
void					vulkan_create_instance(t_cvulkan *app);
void					populate_create_info(
						t_cvulkan *app, VkInstanceCreateInfo *create_info,
						VkApplicationInfo *app_info,
						VkDebugUtilsMessengerCreateInfoEXT *debug_create_info);

/*
** Vulkan surface
*/
void					vulkan_create_surface(t_cvulkan *app);


/*
** Vulkan device
*/
void					query_swap_chain_support(t_cvulkan *app,
						VkPhysicalDevice device, t_swap_chain_support_details
						*details);
void					vulkan_pick_physical_device(t_cvulkan *app);
bool					check_device_extension_support(t_cvulkan *app,
						VkPhysicalDevice device);
VkSampleCountFlagBits	get_max_usable_sample_count(t_cvulkan *app);
void					find_queue_families(t_cvulkan *app,
						VkPhysicalDevice device,
						t_queue_family_indices *indices);

/*
** Vulkan logical device
*/
void					vulkan_create_logical_device(t_cvulkan *app);

#endif
