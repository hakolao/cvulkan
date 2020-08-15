/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vulkan_logical_device.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/09 20:19:10 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/15 22:49:06 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cvulkan.h"

static void		populate_queue_create_info(VkDeviceQueueCreateInfo
				*queue_create_info, uint32_t queue_family_index,
				float queue_priority)
{
	ft_memset(queue_create_info, 0, sizeof(*queue_create_info));
	queue_create_info->sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queue_create_info->queueFamilyIndex = queue_family_index;
	queue_create_info->queueCount = 1;
	queue_create_info->pQueuePriorities = &queue_priority;
}

static void		popilate_device_features(VkPhysicalDeviceFeatures
				*device_features)
{
	ft_memset(device_features, 0, sizeof(*device_features));
	device_features->samplerAnisotropy = VK_TRUE;
	device_features->sampleRateShading = VK_TRUE;
}

static void		populate_logical_device_create_info(t_vkrenderer *app,
				VkDeviceQueueCreateInfo queue_create_infos[2],
				VkDeviceCreateInfo *create_info)
{
	VkPhysicalDeviceFeatures	device_features;

	popilate_device_features(&device_features);
	ft_memset(create_info, 0, sizeof(*create_info));
	create_info->sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	create_info->queueCreateInfoCount = 2;
	create_info->pQueueCreateInfos = queue_create_infos;
	create_info->pEnabledFeatures = &device_features;
	create_info->enabledExtensionCount = app->vk_required_extension_count;
	create_info->ppEnabledExtensionNames =
		(const char **)app->vk_required_device_extensions;
	if (ENABLE_VALIDATION_LAYERS) {
		create_info->enabledLayerCount = app->vk_enabled_layer_count;
		create_info->ppEnabledLayerNames =
			(const char **)app->vk_enabled_layers;
	} else {
		create_info->enabledLayerCount = 0;
	}
}

void			vulkan_create_logical_device(t_vkrenderer *app)
{
	t_queue_family_indices		indices;
	VkDeviceQueueCreateInfo		queue_create_infos[2];
	size_t						i;
	int32_t						queueFamilies[2];
	VkDeviceCreateInfo			create_info;

	vulkan_find_queue_families(app, app->vk_physical_device, &indices);
	queueFamilies[0] = indices.graphics_family;
	queueFamilies[1] = indices.present_family;
	i = -1;
	while (++i < 2)
		populate_queue_create_info(&queue_create_infos[i],
			queueFamilies[i], 1.0);
	populate_logical_device_create_info(app, queue_create_infos, &create_info);
	error_check(vkCreateDevice(app->vk_physical_device, &create_info,
		NULL, &app->vk_logical_device) != VK_SUCCESS,
				"Failed to create logical device!");
	vkGetDeviceQueue(app->vk_logical_device, indices.graphics_family, 0,
		&app->vk_graphics_queue);
	vkGetDeviceQueue(app->vk_logical_device, indices.present_family, 0,
		&app->vk_present_queue);
}
