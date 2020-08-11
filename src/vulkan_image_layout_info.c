/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vulkan_image_layout_info.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/11 17:23:02 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/11 17:29:27 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cvulkan.h"

static void			set_transition_undefined_to_dst(
					VkPipelineStageFlags *src_stage,
					VkPipelineStageFlags *dst_stage,
					VkImageMemoryBarrier *barrier)
{
	barrier->srcAccessMask = 0;
	barrier->dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
	*src_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
	*dst_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
}

static void			set_transition_dst_to_shader_rdonly(
					VkPipelineStageFlags *src_stage,
					VkPipelineStageFlags *dst_stage,
					VkImageMemoryBarrier *barrier)
{
	barrier->srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
	barrier->dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
	*src_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	*dst_stage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
}

static void			set_transition_undefined_to_depth(
					VkPipelineStageFlags *src_stage,
					VkPipelineStageFlags *dst_stage,
					VkImageMemoryBarrier *barrier)
{
	barrier->srcAccessMask = 0;
	barrier->dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT |
		VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
	*src_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
	*dst_stage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
}

void				vulkan_set_barrier_layout_transition(VkImageLayout
					layouts[2], VkPipelineStageFlags *src_stage,
					VkPipelineStageFlags *dst_stage,
					VkImageMemoryBarrier *barrier)
{
	VkImageLayout	old_layout;
	VkImageLayout	new_layout;

	old_layout = layouts[0];
	new_layout = layouts[1];
	if (old_layout == VK_IMAGE_LAYOUT_UNDEFINED &&
		new_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
		set_transition_undefined_to_dst(src_stage, dst_stage, barrier);
	else if (old_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL &&
			new_layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
		set_transition_dst_to_shader_rdonly(src_stage, dst_stage, barrier);
	else if (old_layout == VK_IMAGE_LAYOUT_UNDEFINED &&
			new_layout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
		set_transition_undefined_to_depth(src_stage, dst_stage, barrier);
	else
		error_check(true, "Unsupported layout transition!");
}

 void				vulkan_set_barrier_subresource_range(VkImageLayout
					new_layout, t_image_info *info,
					VkImageMemoryBarrier *barrier)
{
	if (new_layout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
	{
		barrier->subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
		if (vulkan_has_stencil_component(info->format))
			barrier->subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
	}
	else
		barrier->subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	barrier->subresourceRange.baseMipLevel = 0;
	barrier->subresourceRange.levelCount = info->mip_levels;
	barrier->subresourceRange.baseArrayLayer = 0;
	barrier->subresourceRange.layerCount = 1;
}
