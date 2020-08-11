/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vulkan_image_views.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/10 11:50:07 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/11 22:19:02 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cvulkan.h"

VkImageViewCreateInfo		vulkan_create_image_image_view_info(VkImage image,
							VkFormat format, VkImageAspectFlags aspect_flags,
							uint32_t mip_levels)
{
	VkImageViewCreateInfo	view_info;

	ft_memset(&view_info, 0, sizeof(view_info));
	view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	view_info.image = image;
	view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
	view_info.format = format;
	view_info.subresourceRange.aspectMask = aspect_flags;
	view_info.subresourceRange.baseMipLevel = 0;
	view_info.subresourceRange.levelCount = mip_levels;
	view_info.subresourceRange.baseArrayLayer = 0;
	view_info.subresourceRange.layerCount = 1;
	return (view_info);
}

VkImageView					vulkan_create_image_view(t_cvulkan *app,
							VkImageViewCreateInfo *view_info)
{
	VkImageView				image_view;

	error_check(vkCreateImageView(app->vk_logical_device, view_info, NULL,
		&image_view) != VK_SUCCESS, "Failed to create texture image view!");
	return image_view;
}

void						vulkan_create_image_views(t_cvulkan *app)
{
	size_t					i;
	VkImageViewCreateInfo	view_info;

	i = -1;
	while (++i < app->vk_swap_chain_images_count)
	{
		view_info = vulkan_create_image_image_view_info(
			app->vk_swap_chain_images[i], app->vk_swap_chain_image_format,
			VK_IMAGE_ASPECT_COLOR_BIT, 1);
		app->vk_swap_chain_image_views[i] =
			vulkan_create_image_view(app, &view_info);
	}
}

void						vulkan_create_texture_image_view(t_cvulkan *app)
{
	VkImageViewCreateInfo	view_info;

	view_info = vulkan_create_image_image_view_info(
		app->vk_texture_image, VK_FORMAT_B8G8R8A8_SRGB,
		VK_IMAGE_ASPECT_COLOR_BIT, app->vk_mip_levels);
	app->vk_texture_image_view = vulkan_create_image_view(app, &view_info);
}
