/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vulkan_memory.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/11 12:32:45 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/11 12:35:16 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cvulkan.h"

uint32_t	vulkan_find_memory_type(t_cvulkan *app,
			uint32_t type_filter,
			VkMemoryPropertyFlags properties)
{
	VkPhysicalDeviceMemoryProperties	mem_properties;
	size_t								i;

	vkGetPhysicalDeviceMemoryProperties(app->vk_physical_device,
		&mem_properties);

	i = -1;
	while (++i < mem_properties.memoryTypeCount)
		if ((type_filter & (1 << i)) &&
			(mem_properties.memoryTypes[i].propertyFlags & properties) ==
				properties)
			return i;
	error_check(true, "Failed to find suitable memory type!");
	return (0);
}
