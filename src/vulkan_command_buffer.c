/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vulkan_command_buffer.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/11 15:11:38 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/13 14:28:39 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cvulkan.h"

static void			set_render_pass_info(t_cvulkan *app, size_t i,
					VkRenderPassBeginInfo *render_pass_info)
{
		VkClearValue					clear_values[2];

		ft_memset(render_pass_info, 0, sizeof(*render_pass_info));
		render_pass_info->sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		render_pass_info->renderPass = app->vk_render_pass;
		render_pass_info->framebuffer = app->vk_swap_chain_frame_buffers[i];
		render_pass_info->renderArea.offset = (VkOffset2D){0, 0};
		render_pass_info->renderArea.extent = app->vk_swap_chain_extent;
		clear_values[0].color = (VkClearColorValue){{0.0f, 0.0f, 0.0f, 1.0f}};
		clear_values[1].depthStencil = (VkClearDepthStencilValue){1.0f, 0};
		render_pass_info->clearValueCount = 2;
		render_pass_info->pClearValues = clear_values;
}

static void			allocate_command_buffers(t_cvulkan *app)
{
	VkCommandBufferAllocateInfo		allocInfo;

	ft_memset(&allocInfo, 0, sizeof(allocInfo));
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = app->vk_command_pool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = app->vk_swap_chain_images_count;
	error_check(vkAllocateCommandBuffers(app->vk_logical_device, &allocInfo,
		app->vk_command_buffers) != VK_SUCCESS,
		"Failed to allocate command buffers!");
}

static void			record_command_buffer_commands(t_cvulkan *app,
					VkRenderPassBeginInfo *render_pass_info, size_t i)
{
	vkCmdBeginRenderPass(app->vk_command_buffers[i], render_pass_info,
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
}

void				vulkan_create_command_buffers(t_cvulkan *app)
{
	VkRenderPassBeginInfo			render_pass_info;
	VkCommandBufferBeginInfo		begin_info;
	size_t							i;

	allocate_command_buffers(app);
	i = -1;
	while (++i < app->vk_swap_chain_images_count)
	{
		ft_memset(&begin_info, 0, sizeof(begin_info));
		begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		error_check(vkBeginCommandBuffer(app->vk_command_buffers[i], &begin_info)
			!= VK_SUCCESS, "failed to begin recording command buffer!");
		set_render_pass_info(app, i, &render_pass_info);
		record_command_buffer_commands(app, &render_pass_info, i);
		error_check(vkEndCommandBuffer(app->vk_command_buffers[i]) !=
			VK_SUCCESS, "Failed to record command buffer!");
	}
}

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
