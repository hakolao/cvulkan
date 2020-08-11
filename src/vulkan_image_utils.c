/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vulkan_image_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/11 16:58:47 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/11 17:01:06 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cvulkan.h"

bool			vulkan_has_stencil_component(VkFormat format) {
	return (format == VK_FORMAT_D32_SFLOAT_S8_UINT ||
			format == VK_FORMAT_D24_UNORM_S8_UINT);
}

void			vulkan_allocate_image_memory(t_cvulkan *app,
				t_image_info *info)
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

void			vulkan_copy_buffer_to_image(t_cvulkan *app,
				VkBuffer buffer, t_image_info *info)
{
	VkCommandBuffer		command_buffer;
	VkBufferImageCopy	region;

	command_buffer = vulkan_begin_single_time_commands(app);
	ft_memset(&region, 0, sizeof(region));
	region.bufferOffset = 0;
	region.bufferRowLength = 0;
	region.bufferImageHeight = 0;
	region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	region.imageSubresource.mipLevel = 0;
	region.imageSubresource.baseArrayLayer = 0;
	region.imageSubresource.layerCount = 1;
	region.imageOffset = (VkOffset3D){0, 0, 0};
	region.imageExtent = (VkExtent3D){info->width, info->height, 1};
	vkCmdCopyBufferToImage(command_buffer, buffer, *info->image,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
	vulkan_end_single_time_commands(app, command_buffer);
}
