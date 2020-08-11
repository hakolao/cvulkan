/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vulkan_command_pool.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/11 11:56:26 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/11 12:16:14 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cvulkan.h"

void	vulkan_create_command_pool(t_cvulkan *app)
{
	t_queue_family_indices	queue_family_indices;
	VkCommandPoolCreateInfo	pool_info;

	vulkan_find_queue_families(app, app->vk_physical_device,
		&queue_family_indices);
	ft_memset(&pool_info, 0, sizeof(pool_info));
	pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	pool_info.queueFamilyIndex = queue_family_indices.graphics_family;
	error_check(vkCreateCommandPool(app->vk_logical_device, &pool_info,
		NULL, &app->vk_command_pool) !=	VK_SUCCESS,
			"Failed to create command pool!");
}
