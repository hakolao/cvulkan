/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vulkan_image.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/11 12:23:55 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/15 22:49:06 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cvulkan.h"

static void			sdl_image_to_buffers_and_image_info(t_vkrenderer *app,
					t_image_info *image_info, VkBuffer *staging_buffer,
					VkDeviceMemory *staging_buffer_memory)
{
	void				*data;
	VkDeviceSize		image_size;
	SDL_Surface			*image;

	error_check(!(image = IMG_Load(image_info->path)),
		"Failed to load texture image!");
	image_info->width = image->w;
	image_info->height = image->h;
	image_size = image_info->width * image_info->height * 4;
	app->vk_mip_levels = (uint32_t)floor(log2(ft_max_int((int[2]){
		image_info->width, image_info->height}, 2))) + 1;
	image_info->mip_levels = app->vk_mip_levels;
	vulkan_create_buffer(app, &(t_buffer_info){.size = image_size,
		.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		.properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
			VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		.buffer = staging_buffer, .buffer_memory = staging_buffer_memory});
	vkMapMemory(app->vk_logical_device, *staging_buffer_memory,
		0, image_size, 0, &data);
	ft_memcpy(data, image->pixels, (size_t)(image_size));
	vkUnmapMemory(app->vk_logical_device, *staging_buffer_memory);
	SDL_FreeSurface(image);
}

void				vulkan_create_texture_sampler(t_vkrenderer *app)
{
	VkSamplerCreateInfo	sampler_info;

	ft_memset(&sampler_info, 0, sizeof(sampler_info));
	sampler_info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	sampler_info.magFilter = VK_FILTER_LINEAR;
	sampler_info.minFilter = VK_FILTER_LINEAR;
	sampler_info.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	sampler_info.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	sampler_info.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	sampler_info.anisotropyEnable = VK_TRUE;
	sampler_info.maxAnisotropy = 16.0f;
	sampler_info.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	sampler_info.unnormalizedCoordinates = VK_FALSE;
	sampler_info.compareEnable = VK_FALSE;
	sampler_info.compareOp = VK_COMPARE_OP_ALWAYS;
	sampler_info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	sampler_info.minLod = 0.0f;
	sampler_info.maxLod = (float)app->vk_mip_levels;
	sampler_info.mipLodBias = 0.0f;
	error_check(vkCreateSampler(app->vk_logical_device, &sampler_info, NULL,
		&app->vk_texture_sampler) != VK_SUCCESS,
		"Failed to create texture sampler!");
}

void				vulkan_create_texture_image(t_vkrenderer *app,
					const char *filename)
{
	VkBuffer			staging_buffer;
	VkDeviceMemory		staging_buffer_memory;
	t_image_info		image_info;

	image_info = (t_image_info){.width = 0, .height = 0, .mip_levels = 0,
		.msaa_samples = VK_SAMPLE_COUNT_1_BIT,
		.format = VK_FORMAT_B8G8R8A8_SRGB, .tiling = VK_IMAGE_TILING_OPTIMAL,
		.usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT |
			VK_IMAGE_USAGE_TRANSFER_DST_BIT |
			VK_IMAGE_USAGE_SAMPLED_BIT,
		.properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		.image = &app->vk_texture_image,
		.image_memory = &app->vk_texture_image_memory,
		.path = filename};
	sdl_image_to_buffers_and_image_info(app, &image_info, &staging_buffer,
		&staging_buffer_memory);
	vulkan_create_image(app, &image_info);
	vulkan_transition_image_layout(app, &image_info, VK_IMAGE_LAYOUT_UNDEFINED,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	vulkan_copy_buffer_to_image(app, staging_buffer, &image_info);
	vkDestroyBuffer(app->vk_logical_device, staging_buffer, NULL);
	vkFreeMemory(app->vk_logical_device, staging_buffer_memory, NULL);
	vulkan_generate_mipmaps(app, &image_info);
}

void				vulkan_create_image(t_vkrenderer *app, t_image_info *info)
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
