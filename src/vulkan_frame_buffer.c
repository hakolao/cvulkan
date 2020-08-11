/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vulkan_frame_buffer.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/11 13:22:20 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/11 13:31:18 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cvulkan.h"

void	vulkan_create_frame_buffers(t_cvulkan *app)
{
	size_t					i;
	VkImageView				*attachments;
	VkFramebufferCreateInfo	frame_buffer_info;

	i = -1;
	while (++i < app->vk_swap_chain_images_count)
	{
		attachments = (VkImageView[3]){app->vk_color_image_view,
			app->vk_depth_image_view,
			app->vk_swap_chain_image_views[i]};
		ft_memset(&frame_buffer_info, 0, sizeof(frame_buffer_info));
		frame_buffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		frame_buffer_info.renderPass = app->vk_render_pass;
		frame_buffer_info.attachmentCount = 3;
		frame_buffer_info.pAttachments = attachments;
		frame_buffer_info.width = app->vk_swap_chain_extent.width;
		frame_buffer_info.height = app->vk_swap_chain_extent.height;
		frame_buffer_info.layers = 1;
		error_check(vkCreateFramebuffer(app->vk_logical_device,
			&frame_buffer_info, NULL, &app->vk_swap_chain_frame_buffers[i]) !=
				VK_SUCCESS, "Failed to create framebuffer");
	}
}
