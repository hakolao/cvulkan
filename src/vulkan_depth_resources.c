/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vulkan_depth_resources.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/11 13:05:47 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/15 22:49:06 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cvulkan.h"

static VkFormat		find_supported_format(t_vkrenderer *app,
					VkFormat *candidates, VkImageTiling tiling,
					VkFormatFeatureFlags features)
{
	size_t				i;
	VkFormatProperties	props;

	i = -1;
	while (++i < 3)
	{
		vkGetPhysicalDeviceFormatProperties(app->vk_physical_device,
			candidates[i], &props);
		if (tiling == VK_IMAGE_TILING_LINEAR &&
			(props.linearTilingFeatures & features) == features)
			return (candidates[i]);
		else if (tiling == VK_IMAGE_TILING_OPTIMAL &&
					(props.optimalTilingFeatures & features) == features)
			return (candidates[i]);
	}
	error_check(true, "Failed to find supported format!");
	return (0);
}

static VkFormat		find_depth_format(t_vkrenderer *app)
{
	return (find_supported_format(app,
		(uint32_t[3]){VK_FORMAT_D32_SFLOAT,
		VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
		VK_IMAGE_TILING_OPTIMAL,
		VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT));
}

void				vulkan_create_depth_resources(t_vkrenderer *app)
{
	VkFormat				depth_format;
	VkImageViewCreateInfo	view_info;

	depth_format = find_depth_format(app);
	vulkan_create_image(app,
		&(t_image_info){.width = app->vk_swap_chain_extent.width,
			.height = app->vk_swap_chain_extent.height,
			.mip_levels = 1, .msaa_samples = app->vk_msaa_samples,
			.format = depth_format, .tiling = VK_IMAGE_TILING_OPTIMAL,
			.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
			.properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			.image = &app->vk_depth_image,
			.image_memory = &app->vk_depth_image_memory});
	view_info = vulkan_create_image_image_view_info(app->vk_depth_image,
			depth_format, VK_IMAGE_ASPECT_DEPTH_BIT, 1);
	app->vk_depth_image_view = vulkan_create_image_view(app, &view_info);
}
