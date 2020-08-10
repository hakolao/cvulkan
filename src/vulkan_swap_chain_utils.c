/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vulkan_swap_chain_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/09 21:54:38 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/10 11:39:44 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cvulkan.h"

void					vulkan_cleanup_swap_chain(t_cvulkan *app)
{
	vkDestroySwapchainKHR(app->vk_logical_device, app->vk_swap_chain, NULL);
}

void					choose_swap_extent(t_cvulkan *app,
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

void					choose_swap_present_mode(
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

void					choose_swap_surface_format(
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
