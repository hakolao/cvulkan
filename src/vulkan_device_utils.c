/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vulkan_device_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/09 19:47:51 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/09 20:02:30 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cvulkan.h"

VkSampleCountFlagBits			get_max_usable_sample_count(t_cvulkan *app)
{
	VkPhysicalDeviceProperties	physical_device_properties;
	VkSampleCountFlags			counts;

	vkGetPhysicalDeviceProperties(app->vk_physical_device,
		&physical_device_properties);
	counts = physical_device_properties.limits.framebufferColorSampleCounts &
		physical_device_properties.limits.framebufferDepthSampleCounts;
	if (counts & VK_SAMPLE_COUNT_64_BIT)
		return VK_SAMPLE_COUNT_64_BIT;
	if (counts & VK_SAMPLE_COUNT_32_BIT)
		return VK_SAMPLE_COUNT_32_BIT;
	if (counts & VK_SAMPLE_COUNT_16_BIT)
		return VK_SAMPLE_COUNT_16_BIT;
	if (counts & VK_SAMPLE_COUNT_8_BIT)
		return VK_SAMPLE_COUNT_8_BIT;
	if (counts & VK_SAMPLE_COUNT_4_BIT)
		return VK_SAMPLE_COUNT_4_BIT;
	if (counts & VK_SAMPLE_COUNT_2_BIT)
		return VK_SAMPLE_COUNT_2_BIT;
	return VK_SAMPLE_COUNT_1_BIT;
}

bool							check_device_extension_support(t_cvulkan *app,
								VkPhysicalDevice device)
{
	uint32_t				extension_count;
	VkExtensionProperties	available_extensions[64];
	size_t					i;
	size_t					j;
	size_t					required_extensions_checked;

	vkEnumerateDeviceExtensionProperties(device, NULL, &extension_count,
										 NULL);
	vkEnumerateDeviceExtensionProperties(device, NULL, &extension_count,
										 available_extensions);
	i = -1;
	required_extensions_checked = 0;
	while (++i < extension_count)
	{
		j = -1;
		while (++j < app->vk_required_extension_count)
			if (ft_strcmp(available_extensions[i].extensionName,
				app->vk_required_device_extensions[j]) == 0)
				required_extensions_checked++;
	}
	return required_extensions_checked == app->vk_required_extension_count;
}

void							query_swap_chain_support(t_cvulkan *app,
								VkPhysicalDevice device,
								t_swap_chain_support_details
								*details)
{
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, app->vk_surface,
		&details->capabilities);
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, app->vk_surface,
		&details->format_count, NULL);
	if (details->format_count != 0)
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, app->vk_surface,
			&details->format_count, details->formats);
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, app->vk_surface,
		&details->present_mode_count, NULL);
	if (details->present_mode_count != 0)
		vkGetPhysicalDeviceSurfacePresentModesKHR( device, app->vk_surface,
			&details->present_mode_count, details->present_modes);
}
