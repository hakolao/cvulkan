/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vulkan_render_pass_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/10 12:54:50 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/10 12:55:47 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cvulkan.h"

static VkFormat				find_supported_format(t_cvulkan *app,
							VkFormat *candidates, VkImageTiling tiling,
							VkFormatFeatureFlags features)
{
	size_t				i;
	VkFormatProperties	props;

	i = 0;
	while (candidates[i] != (VkFormat)-1)
	{
		vkGetPhysicalDeviceFormatProperties(app->vk_physical_device,
			candidates[i], &props);
		if (tiling == VK_IMAGE_TILING_LINEAR &&
			(props.linearTilingFeatures & features) == features) {
			return (candidates[i]);
		} else if (tiling == VK_IMAGE_TILING_OPTIMAL &&
				   (props.optimalTilingFeatures & features) == features) {
			return (candidates[i]);
		}
		i++;
	}
	error_check(true, "Failed to find supported format!");
	return (VK_FORMAT_UNDEFINED);
}

VkFormat					vulkan_find_depth_format(t_cvulkan *app)
{
	return (find_supported_format(
		app, (VkFormat[4]){VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT,
		 VK_FORMAT_D24_UNORM_S8_UINT, -1}, VK_IMAGE_TILING_OPTIMAL,
		VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT));
}
