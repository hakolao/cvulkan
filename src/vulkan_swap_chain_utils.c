/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vulkan_swap_chain_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/09 21:54:38 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/09 21:56:33 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cvulkan.h"

void	vulkan_cleanup_swap_chain(t_cvulkan *app)
{
	vkDestroySwapchainKHR(app->vk_logical_device, app->vk_swap_chain, NULL);
}
