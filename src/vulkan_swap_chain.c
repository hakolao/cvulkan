/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vulkan_swap_chain.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/09 21:01:12 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/15 22:49:06 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cvulkan.h"

static void					populate_queue_family_dependent_info(t_vkrenderer *app,
							VkSwapchainCreateInfoKHR *create_info)
{
	t_queue_family_indices			indices;
	uint32_t						queue_family_indices[2];

	vulkan_find_queue_families(app, app->vk_physical_device, &indices);
	queue_family_indices[0] = indices.graphics_family,
	queue_family_indices[1] = indices.present_family;
	if (indices.graphics_family != indices.present_family) {
		create_info->imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		create_info->queueFamilyIndexCount = 2;
		create_info->pQueueFamilyIndices = queue_family_indices;
	} else {
		create_info->imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	}
}

static void					populate_swap_chain_create_info(t_vkrenderer *app,
							uint32_t *image_count,
							t_swap_chain_support_details *swap_chain_support,
							VkSwapchainCreateInfoKHR *create_info)
{
	VkSurfaceFormatKHR				surface_format;
	VkPresentModeKHR				present_mode;

	vulkan_choose_swap_surface_format(swap_chain_support, &surface_format);
	app->vk_swap_chain_image_format = surface_format.format;
	vulkan_choose_swap_present_mode(swap_chain_support, &present_mode);
	*image_count = swap_chain_support->capabilities.minImageCount + 1;
	if (swap_chain_support->capabilities.maxImageCount > 0 &&
		*image_count > swap_chain_support->capabilities.maxImageCount)
		*image_count = swap_chain_support->capabilities.maxImageCount;
	ft_memset(create_info, 0, sizeof(*create_info));
	create_info->sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	create_info->surface = app->vk_surface;
	create_info->minImageCount = *image_count;
	create_info->imageFormat = surface_format.format;
	create_info->imageColorSpace = surface_format.colorSpace;
	create_info->imageExtent = app->vk_swap_chain_extent;
	create_info->imageArrayLayers = 1;
	create_info->imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	populate_queue_family_dependent_info(app, create_info);
	create_info->preTransform =
		swap_chain_support->capabilities.currentTransform;
	create_info->compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	create_info->presentMode = present_mode;
	create_info->clipped = VK_TRUE;
}

void						vulkan_create_swap_chain(t_vkrenderer *app)
{
	VkSwapchainCreateInfoKHR		create_info;
	uint32_t						image_count;
	t_swap_chain_support_details	swap_chain_support;

	vulkan_query_swap_chain_support(app, app->vk_physical_device, &swap_chain_support);
	vulkan_choose_swap_extent(app, &swap_chain_support.capabilities,
		&app->vk_swap_chain_extent);
	populate_swap_chain_create_info(app, &image_count, &swap_chain_support,
		&create_info);
	error_check(vkCreateSwapchainKHR(app->vk_logical_device, &create_info,
		NULL, &app->vk_swap_chain) != VK_SUCCESS,
				"Failed to create swap chain!");
	vkGetSwapchainImagesKHR(app->vk_logical_device, app->vk_swap_chain,
		&image_count, NULL);
	vkGetSwapchainImagesKHR(app->vk_logical_device, app->vk_swap_chain,
		&image_count, app->vk_swap_chain_images);
	app->vk_swap_chain_images_count = image_count;
}

void						vulkan_recreate_swapchain(t_vkrenderer *app)
{
	while (app->window_info.is_hidden)
		SDL_PollEvent(NULL);
	vkDeviceWaitIdle(app->vk_logical_device);
	vulkan_cleanup_swap_chain(app);
	vulkan_create_swap_chain(app);
	vulkan_create_image_views(app);
	vulkan_create_render_pass(app);
	vulkan_create_graphics_pipeline(app);
	vulkan_create_color_resources(app);
	vulkan_create_depth_resources(app);
	vulkan_create_frame_buffers(app);
	vulkan_create_uniform_buffers(app);
	vulkan_create_descriptor_pool(app);
	vulkan_create_descriptor_sets(app);
	vulkan_create_command_buffers(app);
}
