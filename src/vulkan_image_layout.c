/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vulkan_image_layout.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/11 17:03:55 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/11 17:25:46 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cvulkan.h"

void			vulkan_transition_image_layout(t_cvulkan *app,
				t_image_info *info,
				VkImageLayout old_layout,
				VkImageLayout new_layout)
{
	VkCommandBuffer			command_buffer;
	VkImageMemoryBarrier	barrier;
	VkPipelineStageFlags	source_stage;
	VkPipelineStageFlags	destination_stage;

	source_stage = 0;
	destination_stage = 0;
	command_buffer = vulkan_begin_single_time_commands(app);
	ft_memset(&barrier, 0, sizeof(barrier));
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout = old_layout;
	barrier.newLayout = new_layout;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.image = *info->image;
	vulkan_set_barrier_subresource_range(new_layout, info, &barrier);
	vulkan_set_barrier_layout_transition((VkImageLayout[2]){old_layout,
		new_layout}, &source_stage, &destination_stage, &barrier);
	vkCmdPipelineBarrier(command_buffer, source_stage, destination_stage, 0, 0,
		NULL, 0, NULL, 1, &barrier);
	vulkan_end_single_time_commands(app, command_buffer);
}
