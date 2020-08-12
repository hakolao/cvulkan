/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vulkan_index_buffer.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/12 18:10:15 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/12 18:15:18 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cvulkan.h"

void	vulkan_create_index_buffer(t_cvulkan *app)
{
	VkDeviceSize	buffer_size;
	VkBuffer		staging_buffer;
	VkDeviceMemory	staging_buffer_memory;
	void			*data;

	buffer_size = sizeof(*app->indices) * app->num_indices;
	vulkan_create_buffer(app, &(t_buffer_info){.size = buffer_size,
		.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		.properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
			VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		.buffer = &staging_buffer, .buffer_memory = &staging_buffer_memory});
	vkMapMemory(app->vk_logical_device, staging_buffer_memory, 0,
		buffer_size, 0, &data);
	ft_memcpy(data, app->indices, (size_t)buffer_size);
	vkUnmapMemory(app->vk_logical_device, staging_buffer_memory);
	vulkan_create_buffer(app, &(t_buffer_info){.size = buffer_size,
		.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT |
			VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
		.properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		.buffer = &app->vk_index_buffer,
		.buffer_memory = &app->vk_index_buffer_memory});
	vulkan_copy_buffer(app, staging_buffer, app->vk_index_buffer, buffer_size);
	vkDestroyBuffer(app->vk_logical_device, staging_buffer, NULL);
	vkFreeMemory(app->vk_logical_device, staging_buffer_memory, NULL);
}
