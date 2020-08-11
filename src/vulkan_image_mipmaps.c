/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vulkan_image_mipmaps.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/11 17:26:49 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/11 18:14:53 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cvulkan.h"

static void		set_pipeline_barrier_mipmap_base_info(t_image_info *info,
				VkImageMemoryBarrier *barrier)
{
	ft_memset(barrier, 0, sizeof(*barrier));
	barrier->sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier->image = *info->image;
	barrier->srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier->dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier->subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	barrier->subresourceRange.baseArrayLayer = 0;
	barrier->subresourceRange.layerCount = 1;
	barrier->subresourceRange.levelCount = 1;
}

static void		pipeline_barrier_after_mipmaps(VkCommandBuffer command_buffer,
				t_image_info *info,
				VkImageMemoryBarrier *barrier)
{
	barrier->subresourceRange.baseMipLevel = info->mip_levels - 1;
	barrier->oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	barrier->newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	barrier->srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
	barrier->dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
	vkCmdPipelineBarrier(command_buffer, VK_PIPELINE_STAGE_TRANSFER_BIT,
		VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, NULL, 0, NULL, 1,
		barrier);
}

static void		validate_mipmap_format_properties(t_cvulkan *app,
				t_image_info *info)
{
	VkFormatProperties		format_properties;

	vkGetPhysicalDeviceFormatProperties(app->vk_physical_device,
		info->format, &format_properties);
	error_check(!(format_properties.optimalTilingFeatures &
		VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT),
		"Texture image format does not support linear blitting!");
}

static void		pipeline_barrier_before_blit_cmd(VkCommandBuffer
				command_buffer, size_t i, VkImageMemoryBarrier *barrier)
{
	barrier->subresourceRange.baseMipLevel = i - 1;
	barrier->oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	barrier->newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
	barrier->srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
	barrier->dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
	vkCmdPipelineBarrier(command_buffer, VK_PIPELINE_STAGE_TRANSFER_BIT,
		VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, NULL, 0,
		NULL, 1, barrier);
}

static void		pipeline_barrier_after_blit_cmd(VkCommandBuffer
				command_buffer, VkImageMemoryBarrier *barrier)
{
	barrier->oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
	barrier->newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	barrier->srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
	barrier->dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
	vkCmdPipelineBarrier(command_buffer, VK_PIPELINE_STAGE_TRANSFER_BIT,
		VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, NULL, 0, NULL, 1,
		barrier);
}

/*
** in the loop, i should begin from 1. Thus i = 0
*/

void			vulkan_generate_mipmaps(t_cvulkan *app, t_image_info *info)
{
	VkCommandBuffer			command_buffer;
	VkImageMemoryBarrier	barrier;
	int32_t					mip_dimensions[2];
	size_t					i;

	validate_mipmap_format_properties(app, info);
	command_buffer = vulkan_begin_single_time_commands(app);
	set_pipeline_barrier_mipmap_base_info(info, &barrier);
	mip_dimensions[0] = info->width;
	mip_dimensions[1] = info->height;
	i = 0;
	while (++i < info->mip_levels)
	{
		pipeline_barrier_before_blit_cmd(command_buffer, i, &barrier);
		vulkan_create_blit_image_cmd(mip_dimensions, command_buffer, info, i);
		pipeline_barrier_after_blit_cmd(command_buffer, &barrier);
		if (mip_dimensions[0] > 1)
			mip_dimensions[0] /= 2;
		if (mip_dimensions[1] > 1)
			mip_dimensions[1] /= 2;
	}
	pipeline_barrier_after_mipmaps(command_buffer, info, &barrier);
	vulkan_end_single_time_commands(app, command_buffer);
}
