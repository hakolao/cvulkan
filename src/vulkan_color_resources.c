/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vulkan_color_resources.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/11 12:17:01 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/11 16:52:58 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cvulkan.h"

void	vulkan_create_color_resources(t_cvulkan *app)
{
	VkFormat				color_format;
	VkImageViewCreateInfo	view_info;

	color_format = app->vk_swap_chain_image_format;
	vulkan_create_image(app,
		&(t_image_info){.width = app->vk_swap_chain_extent.width,
			.height = app->vk_swap_chain_extent.height,
			.mip_levels = 1, .msaa_samples = app->vk_msaa_samples,
			.format = color_format, .tiling = VK_IMAGE_TILING_OPTIMAL,
			.usage = VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT |
					VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
			.properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			.image = &app->vk_color_image,
			.image_memory = &app->vk_color_image_memory});
	view_info = vulkan_create_image_image_view_info(
			app->vk_color_image, color_format,
			VK_IMAGE_ASPECT_COLOR_BIT, 1);
	app->vk_color_image_view =
		vulkan_create_image_view(app, &view_info);
}

