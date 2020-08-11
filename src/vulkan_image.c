/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vulkan_image.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/11 12:23:55 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/11 22:15:46 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cvulkan.h"

static void			map_sdl_image_to_buffers(t_cvulkan *app,
					uint32_t dimensions[2], VkBuffer *stagingBuffer,
					VkDeviceMemory *stagingBufferMemory)
{
	void				*data;
	VkDeviceSize		imageSize;
	SDL_Surface			*image;

	error_check(!(image = IMG_Load(TEXTURE_PATH)),
		"Failed to load texture image!");
	dimensions[0] = image->w;
	dimensions[1] = image->h;
	imageSize = dimensions[0] * dimensions[1] * 4;
	app->vk_mip_levels = (uint32_t)floor(log2(ft_max_int((int[2]){
		dimensions[0], dimensions[1]}, 2))) + 1;
	vulkan_create_buffer(app, &(t_buffer_info){.size = imageSize,
		.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		.properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
			VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		.buffer = stagingBuffer, .buffer_memory = stagingBufferMemory});
	vkMapMemory(app->vk_logical_device, *stagingBufferMemory,
		0, imageSize, 0, &data);
	ft_memcpy(data, image->pixels, (size_t)(imageSize));
	vkUnmapMemory(app->vk_logical_device, *stagingBufferMemory);
	SDL_FreeSurface(image);
}

void				vulkan_create_texture_image(t_cvulkan *app)
{
	uint32_t			dimensions[2];
	VkBuffer			stagingBuffer;
	VkDeviceMemory		stagingBufferMemory;
	t_image_info		image_info;

	dimensions[0] = 0;
	dimensions[1] = 0;
	map_sdl_image_to_buffers(app, dimensions,
		&stagingBuffer, &stagingBufferMemory);
	image_info = (t_image_info){.width = dimensions[0], .height = dimensions[1],
		.mip_levels = app->vk_mip_levels, .msaa_samples = VK_SAMPLE_COUNT_1_BIT,
		.format = VK_FORMAT_B8G8R8A8_SRGB, .tiling = VK_IMAGE_TILING_OPTIMAL,
		.usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT |
			VK_IMAGE_USAGE_TRANSFER_DST_BIT |
			VK_IMAGE_USAGE_SAMPLED_BIT,
		.properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		.image = &app->vk_texture_image,
		.image_memory = &app->vk_texture_image_memory};
	vulkan_create_image(app, &image_info);
	vulkan_transition_image_layout(app, &image_info, VK_IMAGE_LAYOUT_UNDEFINED,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	vulkan_copy_buffer_to_image(app, stagingBuffer, &image_info);
	vkDestroyBuffer(app->vk_logical_device, stagingBuffer, NULL);
	vkFreeMemory(app->vk_logical_device, stagingBufferMemory, NULL);
	vulkan_generate_mipmaps(app, &image_info);
}

void				vulkan_create_image(t_cvulkan *app, t_image_info *info)
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
