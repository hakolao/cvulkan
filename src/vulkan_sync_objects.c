/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vulkan_sync_objects.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/13 14:32:18 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/13 15:34:39 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cvulkan.h"

void	vulkan_create_sync_objects(t_cvulkan *app)
{
	size_t					i;
	VkSemaphoreCreateInfo	semaphore_info;
	VkFenceCreateInfo		fence_info;

	i = -1;
	app->vk_current_frame = 0;
	while (++i < app->vk_swap_chain_images_count)
		app->vk_images_in_flight[i] = VK_NULL_HANDLE;
	ft_memset(&semaphore_info, 0, sizeof(semaphore_info));
	semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	ft_memset(&fence_info, 0, sizeof(fence_info));
	fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;
	for (size_t i = 0; i < MAXFRAMESINFLIGHT; i++) {
		error_check(vkCreateSemaphore(app->vk_logical_device, &semaphore_info,
			NULL, &app->vk_img_available_semaphores[i]) != VK_SUCCESS ||
				vkCreateSemaphore(app->vk_logical_device, &semaphore_info, NULL,
					&app->vk_rndr_finished_semaphores[i]) != VK_SUCCESS ||
					vkCreateFence(app->vk_logical_device, &fence_info, NULL,
				 		&app->vk_in_flight_fences[i]) != VK_SUCCESS,
			"Failed to create synchronization objects for a frame!");
	}
}
