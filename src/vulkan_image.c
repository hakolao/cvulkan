/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vulkan_image.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/11 12:23:55 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/11 12:54:15 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cvulkan.h"

static void		allocate_image_memory(t_cvulkan *app,
				t_create_image_info *info)
{
	VkMemoryAllocateInfo	alloc_info;
	VkMemoryRequirements	mem_requirements;

	vkGetImageMemoryRequirements(app->vk_logical_device, *info->image,
		&mem_requirements);
	ft_memset(&alloc_info, 0, sizeof(alloc_info));
	alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	alloc_info.allocationSize = mem_requirements.size;
	alloc_info.memoryTypeIndex = vulkan_find_memory_type(app,
		mem_requirements.memoryTypeBits, info->properties);
	error_check(vkAllocateMemory(app->vk_logical_device, &alloc_info, NULL,
		info->image_memory) != VK_SUCCESS, "Failed to allocate image memory!");
}

void			vulkan_create_image(t_cvulkan *app, t_create_image_info *info)
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
	allocate_image_memory(app, info);
	vkBindImageMemory(app->vk_logical_device, *info->image,
		*info->image_memory, 0);
}
