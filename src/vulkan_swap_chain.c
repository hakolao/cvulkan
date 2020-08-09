/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vulkan_swap_chain.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/09 21:01:12 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/09 22:06:27 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cvulkan.h"

static void					choose_swap_extent(t_cvulkan *app,
							VkSurfaceCapabilitiesKHR *capabilities,
							VkExtent2D *actualExtent)
{
	int			width;
	int			height;

	if (capabilities->currentExtent.width != UINT32_MAX)
		*actualExtent = capabilities->currentExtent;
	else
	{
		SDL_GL_GetDrawableSize(app->window, &width, &height);
		actualExtent->width = ft_max_int((int[2]){
			capabilities->minImageExtent.width,
			ft_min_int((int[2]){capabilities->maxImageExtent.width,
				width}, 2)}, 2);
		actualExtent->height = ft_max_int((int[2]){
			capabilities->minImageExtent.height,
			ft_min_int((int[2]){capabilities->maxImageExtent.height,
				height}, 2)}, 2);
	}
}

static void					choose_swap_present_mode(
							t_swap_chain_support_details *details,
							VkPresentModeKHR *present_mode)
{
	size_t			i;

	i = -1;
	while (++i < details->present_mode_count)
	{
		if (details->present_modes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
		{
			*present_mode = details->present_modes[i];
			return ;
		}
	}
	*present_mode = VK_PRESENT_MODE_FIFO_KHR;
}

static void					choose_swap_surface_format(
							t_swap_chain_support_details *details,
							VkSurfaceFormatKHR *format)
{
	size_t			i;

	i = -1;
	while (++i < details->format_count)
	{
		if (details->formats[i].format == VK_FORMAT_B8G8R8A8_SRGB &&
			details->formats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
		{
			*format = details->formats[i];
			return ;
		}
	}
	*format = details->formats[0];
}

static void					populate_swap_chain_create_info(t_cvulkan *app,
							uint32_t *image_count,
							t_swap_chain_support_details *swap_chain_support,
							VkSwapchainCreateInfoKHR *create_info)
{
	t_queue_family_indices			indices;
	uint32_t						queue_family_indices[2];
	VkSurfaceFormatKHR				surface_format;
	VkPresentModeKHR				present_mode;

	choose_swap_surface_format(swap_chain_support, &surface_format);
	app->vk_swap_chain_image_format = surface_format.format;
	choose_swap_present_mode(swap_chain_support, &present_mode);
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
	find_queue_families(app, app->vk_physical_device, &indices);
	queue_family_indices[0] = indices.graphics_family,
	queue_family_indices[1] = indices.present_family;
	if (indices.graphics_family != indices.present_family) {
		create_info->imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		create_info->queueFamilyIndexCount = 2;
		create_info->pQueueFamilyIndices = queue_family_indices;
	} else {
		create_info->imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	}
	create_info->preTransform =
		swap_chain_support->capabilities.currentTransform;
	create_info->compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	create_info->presentMode = present_mode;
	create_info->clipped = VK_TRUE;
}

void						vulkan_create_swap_chain(t_cvulkan *app)
{
	VkSwapchainCreateInfoKHR		create_info;
	uint32_t						image_count;
	t_swap_chain_support_details	swap_chain_support;

	query_swap_chain_support(app, app->vk_physical_device, &swap_chain_support);
	choose_swap_extent(app, &swap_chain_support.capabilities,
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
}
