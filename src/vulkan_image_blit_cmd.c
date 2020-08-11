/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vulkan_image_blit_cmd.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/11 17:57:29 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/11 18:12:41 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cvulkan.h"

void			vulkan_create_blit_image_cmd(int32_t dimensions[2],
				VkCommandBuffer command_buffer,
				t_image_info *info, size_t i)
{
	VkImageBlit				blit;
	int32_t					mip_width;
	int32_t					mip_height;

	mip_width = dimensions[0];
	mip_height = dimensions[1];
	ft_memset(&blit, 0, sizeof(blit));
	blit.srcOffsets[0] = (VkOffset3D){0, 0, 0};
	blit.srcOffsets[1] = (VkOffset3D){mip_width, mip_height, 1};
	blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	blit.srcSubresource.mipLevel = i - 1;
	blit.srcSubresource.baseArrayLayer = 0;
	blit.srcSubresource.layerCount = 1;
	blit.dstOffsets[0] = (VkOffset3D){0, 0, 0};
	blit.dstOffsets[1] = (VkOffset3D){mip_width > 1 ? mip_width / 2 : 1,
		mip_height > 1 ? mip_height / 2 : 1, 1};
	blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	blit.dstSubresource.mipLevel = i;
	blit.dstSubresource.baseArrayLayer = 0;
	blit.dstSubresource.layerCount = 1;
	vkCmdBlitImage(command_buffer, *info->image,
		VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, *info->image,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &blit, VK_FILTER_LINEAR);
}
