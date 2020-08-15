/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vulkan_instance.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/07 18:04:19 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/15 22:49:06 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cvulkan.h"

static bool			check_available_layers(t_vkrenderer *app,
					VkLayerProperties *available_layers,
					size_t found_layer_count)
{
	size_t				i;
	size_t				j;
	bool				layer_found;

	i = -1;
	while (++i < app->vk_enabled_layer_count)
	{
		layer_found = false;
		j = -1;
		while (++j < found_layer_count)
		{
			if (ft_strcmp(app->vk_enabled_layers[i],
				available_layers[j].layerName) == 0)
			{
				layer_found = true;
				break ;
			}
		}
		if (!layer_found)
			return (false);
	}
	return (true);
}

static bool			check_validation_layer_support(t_vkrenderer *app)
{
	uint32_t			layer_count;
	VkLayerProperties	*available_layers;
	bool				result;

	vkEnumerateInstanceLayerProperties(&layer_count, NULL);
	error_check(!(available_layers =
		malloc(sizeof(VkLayerProperties) * layer_count)),
		"Failed to malloc available layers!");
	vkEnumerateInstanceLayerProperties(&layer_count, available_layers);
	result = check_available_layers(app, available_layers, layer_count);
	free(available_layers);
	return (result);
}

static void			get_required_extensions(t_vkrenderer *app) {
	app->vk_enabled_extension_count = 0;
	error_check(
		!SDL_Vulkan_GetInstanceExtensions(app->window,
		&app->vk_enabled_extension_count, NULL),
		"Failed to get instance extensions");
	error_check(!SDL_Vulkan_GetInstanceExtensions(app->window,
		&app->vk_enabled_extension_count,
		(const char**)&app->vk_extension_names), "Failed to get extension names");
	if (ENABLE_VALIDATION_LAYERS) {
		app->vk_extension_names[app->vk_enabled_extension_count] =
			VK_EXT_DEBUG_UTILS_EXTENSION_NAME;
		app->vk_enabled_extension_count++;
	}
}

static bool			valid_vulkan_extensions(t_vkrenderer *app) {
	uint32_t				available_ext_count;
	uint32_t				match_count;
	VkExtensionProperties	*available_extensions;
	size_t					i;
	size_t					j;

	available_ext_count = 0;
	vkEnumerateInstanceExtensionProperties(NULL, &available_ext_count, NULL);
	error_check(!(available_extensions =
		malloc(sizeof(*available_extensions) * available_ext_count)),
		"Failed to malloc available_extensions!");
	vkEnumerateInstanceExtensionProperties(NULL, &available_ext_count,
		available_extensions);
	match_count = 0;
	i = -1;
	while (++i < available_ext_count)
	{
		j = -1;
		while (++j < app->vk_enabled_extension_count)
			if (ft_strcmp(app->vk_extension_names[j],
				available_extensions[i].extensionName) == 0)
				match_count++;
	}
	free(available_extensions);
	return match_count == app->vk_enabled_extension_count;
}

void					vulkan_create_instance(t_vkrenderer *app) {
	VkApplicationInfo					app_info;
	VkInstanceCreateInfo				create_info;
	VkDebugUtilsMessengerCreateInfoEXT	debug_create_info;

	app->vk_enabled_layers[0] = "VK_LAYER_KHRONOS_validation";
	app->vk_enabled_layer_count = 1;
	error_check(ENABLE_VALIDATION_LAYERS &&
		!check_validation_layer_support(app),
				"Validation layers requested, but not available!");
	get_required_extensions(app);
	error_check(!valid_vulkan_extensions(app),
				"Some SDL vulkan extensions not found in availalbe extensions");
	app_info.pApplicationName = NAME;
	app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	app_info.pEngineName = "No Engine";
	app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	app_info.apiVersion = VK_API_VERSION_1_0;
	app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	app_info.pNext = NULL;
	vulkan_populate_instance_create_info(app, &app_info, &debug_create_info,
		&create_info);
	error_check(vkCreateInstance(&create_info, NULL, &app->vk_instance) !=
		VK_SUCCESS, "Failed to create a Vulkan Instance");
}
