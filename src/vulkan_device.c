/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vulkan_device.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/09 18:39:46 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/09 19:50:25 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cvulkan.h"

static void						find_queue_families(t_cvulkan *app,
								VkPhysicalDevice device,
								t_queue_family_indices *indices)
{
	uint32_t				queue_family_count;
	VkQueueFamilyProperties	queue_families[64];
	size_t					i;
	VkBool32				present_support;

	queue_family_count = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, NULL);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count,
		queue_families);
	i = 0;
	indices->graphics_family = -1;
	indices->present_family = -1;
	while (i  < queue_family_count)
	{
		if (queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
			indices->graphics_family = i;
		if (indices->graphics_family != -1 && indices->present_family != -1)
			break;
		present_support = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, app->vk_surface,
											 &present_support);
		if (present_support)
			indices->present_family = i;
		i++;
	}
}

static int 						rate_device_suitability(t_cvulkan *app,
								VkPhysicalDevice device)
{
	VkPhysicalDeviceProperties		deviceProperties;
	VkPhysicalDeviceFeatures		deviceFeatures;
	t_queue_family_indices			indices;
	int								score;
	bool							swap_chain_adequate;
	t_swap_chain_support_details	swap_chain_support;

	vkGetPhysicalDeviceProperties(device, &deviceProperties);
	vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
	score = 0;
	if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
		score += 1000;
	score += deviceProperties.limits.maxImageDimension2D;
	if (!deviceFeatures.samplerAnisotropy)
		return 0;
	find_queue_families(device, device, &indices);
	if (indices.graphics_family == -1 || indices.present_family == -1)
		return 0;
	if (!check_device_extension_support(app, device))
		return 0;
	swap_chain_adequate = false;
	swap_chain_support.format_count = 0;
	swap_chain_support.present_mode_count = 0;
	query_swap_chain_support(app, device, &swap_chain_support);
	if (!(swap_chain_support.format_count > 0 &&
			swap_chain_support.present_mode_count > 0))
		return 0;
	return score;
}

void							vulkan_pick_physical_device(t_cvulkan *app)
{
	uint32_t			device_count;
	VkPhysicalDevice	devices[64];
	VkPhysicalDevice	best;
	size_t				i;
	int					score;
	int					tmp;

	device_count = 0;
	app->vk_required_extension_count = 1;
	app->vk_required_device_extensions[0] = VK_KHR_SWAPCHAIN_EXTENSION_NAME;
	vkEnumeratePhysicalDevices(app->vk_instance, &device_count, NULL);
	error_check(device_count == 0, "Failed to find GPUs with Vulkan support!");
	vkEnumeratePhysicalDevices(app->vk_instance, &device_count, devices);
	i = -1;
	score = 0;
	best = devices[0];
	while (++i < device_count)
	{
		tmp = rate_device_suitability(devices[i]);
		if (tmp > score)
		{
			score = tmp;
			best = devices[i];
		}
	}
	if (score > 0)
	{
		app->vk_physical_device = best;
		app->vk_msaa_samples = get_max_usable_sample_count(app);
	}
	else
		error_check(true, "Failed to find a suitable GPU!");
}
