/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vulkan_command_buffer.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/11 15:11:38 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/12 23:23:22 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cvulkan.h"

void				vulkan_create_buffer(t_cvulkan *app, t_buffer_info *info)
{
	VkBufferCreateInfo		buffer_info;
	VkMemoryRequirements	mem_requirements;
	VkMemoryAllocateInfo	alloc_info;

	ft_memset(&buffer_info, 0, sizeof(buffer_info));
	buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	buffer_info.size = info->size;
	buffer_info.usage = info->usage;
	buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	error_check(vkCreateBuffer(app->vk_logical_device, &buffer_info, NULL,
		info->buffer) != VK_SUCCESS, "Failed to create buffer!");
	vkGetBufferMemoryRequirements(app->vk_logical_device, *info->buffer,
		&mem_requirements);
	ft_memset(&alloc_info, 0, sizeof(alloc_info));
	alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	alloc_info.allocationSize = mem_requirements.size;
	alloc_info.memoryTypeIndex = vulkan_find_memory_type(app,
		mem_requirements.memoryTypeBits, info->properties);
	error_check(vkAllocateMemory(app->vk_logical_device, &alloc_info, NULL,
		info->buffer_memory) != VK_SUCCESS,
		"Failed to allocate buffer memory!");
	vkBindBufferMemory(app->vk_logical_device, *info->buffer,
		*info->buffer_memory, 0);
}

void				vulkan_create_command_buffers(t_cvulkan *app)
{
	VkCommandBufferAllocateInfo		allocInfo;
	size_t							i;
	VkClearValue					clearValues[2];
	VkRenderPassBeginInfo			renderPassInfo;
	VkCommandBufferBeginInfo		beginInfo;

	ft_memset(&allocInfo, 0, sizeof(allocInfo));
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = app->vk_command_pool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = app->vk_swap_chain_images_count;
	error_check(vkAllocateCommandBuffers(app->vk_logical_device, &allocInfo,
		app->vk_command_buffers) != VK_SUCCESS,
		"Failed to allocate command buffers!");
	i = -1;
	while (++i < app->vk_swap_chain_images_count)
	{
		ft_memset(&beginInfo, 0, sizeof(beginInfo));
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		error_check(vkBeginCommandBuffer(app->vk_command_buffers[i], &beginInfo)
			!= VK_SUCCESS, "failed to begin recording command buffer!");
		ft_memset(&renderPassInfo, 0, sizeof(renderPassInfo));
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = app->vk_render_pass;
		renderPassInfo.framebuffer = app->vk_swap_chain_frame_buffers[i];
		renderPassInfo.renderArea.offset = (VkOffset2D){0, 0};
		renderPassInfo.renderArea.extent = app->vk_swap_chain_extent;
		clearValues[0].color = (VkClearColorValue){{0.0f, 0.0f, 0.0f, 1.0f}};
		clearValues[1].depthStencil = (VkClearDepthStencilValue){1.0f, 0};
		renderPassInfo.clearValueCount = 2;
		renderPassInfo.pClearValues = clearValues;
		vkCmdBeginRenderPass(app->vk_command_buffers[i], &renderPassInfo,
			VK_SUBPASS_CONTENTS_INLINE);
		vkCmdBindPipeline(app->vk_command_buffers[i],
			VK_PIPELINE_BIND_POINT_GRAPHICS, app->vk_graphics_pipeline);
		vkCmdBindVertexBuffers(app->vk_command_buffers[i], 0, 1,
			(VkBuffer[1]){app->vk_vertex_buffer}, (VkDeviceSize[1]){0});
		vkCmdBindIndexBuffer(app->vk_command_buffers[i], app->vk_index_buffer, 0,
			VK_INDEX_TYPE_UINT32);
		vkCmdBindDescriptorSets(app->vk_command_buffers[i],
			VK_PIPELINE_BIND_POINT_GRAPHICS, app->vk_pipeline_layout,
			0, 1, &app->vk_descriptor_sets[i], 0, NULL);
		vkCmdDrawIndexed(app->vk_command_buffers[i],
			app->num_indices, 1, 0, 0, 0);
		vkCmdEndRenderPass(app->vk_command_buffers[i]);
		error_check(vkEndCommandBuffer(app->vk_command_buffers[i]) !=
			VK_SUCCESS, "Failed to record command buffer!");
	}
}
