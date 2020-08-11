/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vulkan_command_buffer.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/11 15:11:38 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/11 16:57:31 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cvulkan.h"

VkCommandBuffer		vulkan_begin_single_time_commands(t_cvulkan *app)
{
	VkCommandBufferAllocateInfo	alloc_info;
	VkCommandBuffer				command_buffer;
	VkCommandBufferBeginInfo	begin_info;

	ft_memset(&alloc_info, 0, sizeof(alloc_info));
	alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	alloc_info.commandPool = app->vk_command_pool;
	alloc_info.commandBufferCount = 1;
	vkAllocateCommandBuffers(app->vk_logical_device, &alloc_info,
		&command_buffer);
	ft_memset(&begin_info, 0, sizeof(begin_info));
	begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	vkBeginCommandBuffer(command_buffer, &begin_info);
	return (command_buffer);
}

void				vulkan_end_single_time_commands(t_cvulkan *app,
					VkCommandBuffer command_buffer)
{
	VkSubmitInfo	submit_info;

	vkEndCommandBuffer(command_buffer);
	ft_memset(&submit_info, 0, sizeof(submit_info));
	submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submit_info.commandBufferCount = 1;
	submit_info.pCommandBuffers = &command_buffer;
	vkQueueSubmit(app->vk_graphics_queue, 1, &submit_info, VK_NULL_HANDLE);
	vkQueueWaitIdle(app->vk_graphics_queue);
	vkFreeCommandBuffers(app->vk_logical_device, app->vk_command_pool,
		1, &command_buffer);
}

void				vulkan_create_buffer(t_cvulkan *app, t_buffer_info *info)
{
	VkBufferCreateInfo		bufferInfo;
	VkMemoryRequirements	memRequirements;
	VkMemoryAllocateInfo	allocInfo;

	ft_memset(&bufferInfo, 0, sizeof(bufferInfo));
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = info->size;
	bufferInfo.usage = info->usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	error_check(vkCreateBuffer(app->vk_logical_device, &bufferInfo, NULL,
		info->buffer) != VK_SUCCESS, "Failed to create vertex buffer!");
	vkGetBufferMemoryRequirements(app->vk_logical_device, *info->buffer,
		&memRequirements);
	ft_memset(&allocInfo, 0, sizeof(allocInfo));
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = vulkan_find_memory_type(app,
		memRequirements.memoryTypeBits, info->properties);
	error_check(vkAllocateMemory(app->vk_logical_device, &allocInfo, NULL,
		info->buffer_memory) != VK_SUCCESS,
		"Failed to allocate vertex buffer memory!");
	vkBindBufferMemory(app->vk_logical_device, *info->buffer,
		*info->buffer_memory, 0);
}
