/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vulkan_image.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/11 12:23:55 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/11 17:01:39 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cvulkan.h"

static void		transition_image_layout(t_cvulkan *app,
				t_image_info *info,
				VkImageLayout oldLayout,
				VkImageLayout newLayout)
{
	VkCommandBuffer			commandBuffer;
	VkImageMemoryBarrier	barrier;
	VkPipelineStageFlags	sourceStage;
	VkPipelineStageFlags	destinationStage;

	sourceStage = 0;
	destinationStage = 0;
	commandBuffer = vulkan_begin_single_time_commands(app);
	ft_memset(&barrier, 0, sizeof(barrier));
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout = oldLayout;
	barrier.newLayout = newLayout;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.image = *info->image;
	if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
	{
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
		if (vulkan_has_stencil_component(info->format))
			barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
	}
	else
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = info->mip_levels;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;
	if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED &&
		newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	}
	else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL &&
			   newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
		sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	}
	else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED &&
			   newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT |
								VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
		sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	}
	else
		error_check(true, "Unsupported layout transition!");
	vkCmdPipelineBarrier(commandBuffer, sourceStage, destinationStage, 0, 0,
		NULL, 0, NULL, 1, &barrier);
	vulkan_end_single_time_commands(app, commandBuffer);
}

void			vulkan_create_image(t_cvulkan *app, t_image_info *info)
{
	VkImageCreateInfo		image_info;

	ft_memset(&image_info, 0, sizeof(image_info));
	image_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	image_info.imageType = VK_IMAGE_TYPE_2D;
	image_info.extent.width = info->width;
	image_info.extent.height = info->height;
	image_info.extent.depth = 1;
	image_info.mipLevels = info->mip_levels;
	image_info.arrayLayers = 1;
	image_info.format = info->format;
	image_info.tiling = info->tiling;
	image_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	image_info.usage = info->usage;
	image_info.samples = info->msaa_samples;
	image_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	error_check(vkCreateImage(app->vk_logical_device, &image_info, NULL,
			info->image) != VK_SUCCESS, "Failed to create image!");
	vulkan_allocate_image_memory(app, info);
	vkBindImageMemory(app->vk_logical_device, *info->image,
		*info->image_memory, 0);
}

void			generate_mipmaps(t_cvulkan *app, t_image_info *info)
{
	VkFormatProperties		formatProperties;
	VkCommandBuffer			commandBuffer;
	VkImageMemoryBarrier	barrier;
	int32_t					mipWidth;
	int32_t					mipHeight;
	size_t					i;
	VkImageBlit				blit;

	vkGetPhysicalDeviceFormatProperties(app->vk_physical_device,
		info->format, &formatProperties);
	error_check(!(formatProperties.optimalTilingFeatures &
		VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT),
		"Texture image format does not support linear blitting!");
	commandBuffer = vulkan_begin_single_time_commands(app);
	ft_memset(&barrier, 0, sizeof(barrier));
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.image = *info->image;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;
	barrier.subresourceRange.levelCount = 1;
	mipWidth = info->width;
	mipHeight = info->height;
	i = 0;
	while (++i < info->mip_levels)
	{
		barrier.subresourceRange.baseMipLevel = i - 1;
		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
		vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT,
			VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, NULL, 0,
			NULL, 1, &barrier);
		ft_memset(&blit, 0, sizeof(blit));
		blit.srcOffsets[0] = (VkOffset3D){0, 0, 0};
		blit.srcOffsets[1] = (VkOffset3D){mipWidth, mipHeight, 1};
		blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		blit.srcSubresource.mipLevel = i - 1;
		blit.srcSubresource.baseArrayLayer = 0;
		blit.srcSubresource.layerCount = 1;
		blit.dstOffsets[0] = (VkOffset3D){0, 0, 0};
		blit.dstOffsets[1] = (VkOffset3D){mipWidth > 1 ? mipWidth / 2 : 1,
			mipHeight > 1 ? mipHeight / 2 : 1, 1};
		blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		blit.dstSubresource.mipLevel = i;
		blit.dstSubresource.baseArrayLayer = 0;
		blit.dstSubresource.layerCount = 1;
		vkCmdBlitImage(
			commandBuffer, *info->image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
			*info->image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &blit,
			VK_FILTER_LINEAR);
		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
		vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT,
							 VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0,
							 NULL, 0, NULL, 1, &barrier);
		if (mipWidth > 1)
			mipWidth /= 2;
		if (mipHeight > 1)
			mipHeight /= 2;
	}
	barrier.subresourceRange.baseMipLevel = info->mip_levels - 1;
	barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
	barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
	vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT,
		VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, NULL,
		0, NULL, 1, &barrier);
	vulkan_end_single_time_commands(app, commandBuffer);
}

void			vulkan_create_texture_image(t_cvulkan *app)
{
	SDL_Surface			*image;
	uint32_t			texWidth;
	uint32_t			texHeight;
	VkDeviceSize		imageSize;
	VkBuffer			stagingBuffer;
	VkDeviceMemory		stagingBufferMemory;
	void				*data;
	t_image_info		image_info;

	stagingBuffer = NULL;
	stagingBufferMemory = NULL;
	error_check(!(image = IMG_Load(TEXTURE_PATH)),
		"Failed to load texture image!");
	texWidth = image->w;
	texHeight = image->h;
	imageSize = texWidth * texHeight * 4;
	app->vk_mip_levels = (uint32_t)floor(log2(ft_max_int((int[2]){texWidth,
		texHeight}, 2))) + 1;
	vulkan_create_buffer(app, &(t_buffer_info){.size = imageSize,
		.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		.properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
			VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		.buffer = &stagingBuffer, .buffer_memory = &stagingBufferMemory});
	vkMapMemory(app->vk_logical_device, stagingBufferMemory,
		0, imageSize, 0, &data);
	ft_memcpy(data, image->pixels, (size_t)(imageSize));
	vkUnmapMemory(app->vk_logical_device, stagingBufferMemory);
	SDL_FreeSurface(image);
	image_info = (t_image_info){.width = texWidth, .height = texHeight,
		.mip_levels = app->vk_mip_levels, .msaa_samples = VK_SAMPLE_COUNT_1_BIT,
		.format = VK_FORMAT_B8G8R8A8_SRGB, .tiling = VK_IMAGE_TILING_OPTIMAL,
		.usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT |
			VK_IMAGE_USAGE_TRANSFER_DST_BIT |
			VK_IMAGE_USAGE_SAMPLED_BIT,
		.properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		.image = &app->vk_texture_image,
		.image_memory = &app->vk_texture_image_memory};
	vulkan_create_image(app, &image_info);
	transition_image_layout(app, &image_info, VK_IMAGE_LAYOUT_UNDEFINED,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	vulkan_copy_buffer_to_image(app, stagingBuffer, &image_info);
	vkDestroyBuffer(app->vk_logical_device, stagingBuffer, NULL);
	vkFreeMemory(app->vk_logical_device, stagingBufferMemory, NULL);
	generate_mipmaps(app, &image_info);
}
