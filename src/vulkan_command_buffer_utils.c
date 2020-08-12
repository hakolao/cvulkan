/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vulkan_command_buffer_utils.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/12 23:18:34 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/12 23:19:05 by ohakola          ###   ########.fr       */
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

void				vulkan_copy_buffer(t_cvulkan *app, VkBuffer src_buffer,
					VkBuffer dst_buffer, VkDeviceSize size)
{
	VkCommandBuffer		command_buffer;
	VkBufferCopy		copy_region;

	command_buffer = vulkan_begin_single_time_commands(app);
	ft_memset(&copy_region, 0, sizeof(copy_region));
	copy_region.size = size;
	vkCmdCopyBuffer(command_buffer, src_buffer, dst_buffer, 1, &copy_region);
	vulkan_end_single_time_commands(app, command_buffer);
}
