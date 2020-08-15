/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vulkan_instance_info.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/07 20:26:04 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/15 22:49:06 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cvulkan.h"

void				vulkan_populate_instance_create_info(
					t_vkrenderer *app, VkApplicationInfo *app_info,
					VkDebugUtilsMessengerCreateInfoEXT *debug_create_info,
					VkInstanceCreateInfo *create_info)
{
	create_info->sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	create_info->pApplicationInfo = app_info;
	if (ENABLE_VALIDATION_LAYERS)
	{
		create_info->enabledLayerCount = app->vk_enabled_layer_count;
		create_info->ppEnabledLayerNames = (const char**)app->vk_enabled_layers;
		vulkan_populate_debug_messenger_create_info(debug_create_info);
		create_info->pNext = debug_create_info;
	}
	else
	{
		create_info->enabledLayerCount = 0;
		create_info->pNext = NULL;
		create_info->ppEnabledLayerNames = NULL;
	}
	create_info->enabledExtensionCount = app->vk_enabled_extension_count;
	create_info->ppEnabledExtensionNames = (const char**)app->vk_extension_names;
	create_info->flags = 0;
}
