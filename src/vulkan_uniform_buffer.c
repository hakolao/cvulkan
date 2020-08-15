/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vulkan_uniform_buffer.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/12 18:17:07 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/15 22:49:06 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cvulkan.h"

void	vulkan_create_uniform_buffers(t_vkrenderer *app)
{
	VkDeviceSize	buffer_size;
	size_t			i;

	buffer_size = sizeof(t_uniform_buffer_object);
	i = -1;
	while (++i < app->vk_swap_chain_images_count)
		vulkan_create_buffer(app, &(t_buffer_info){.size = buffer_size,
			.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			.properties =  VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
				VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			.buffer = &app->vk_uniform_buffers[i],
			.buffer_memory = &app->vk_uniform_buffers_memory[i]});
}
