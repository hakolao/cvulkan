/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vulkan_draw.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/13 15:33:11 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/17 00:56:50 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cvulkan.h"

static void		vulkan_update_uniform_buffer(t_vkrenderer *app,
				uint32_t current_image)
{
	t_uniform_buffer_object		ubo;
	t_mat4						model;
	void						*data;

	ml_matrix4_id(model);
	ml_matrix4_rotate_z(model, ml_rad(app->rotation_angle), ubo.model);
	ml_matrix4_lookat((t_vec3){2.0f, 2.0f, 2.0f}, (t_vec3){0.0f, 0.0f, 0.0f},
		(t_vec3){0.0f, 0.0f, 1.0f}, ubo.view);
	ml_matrix4_projection(ML_PERSPECTIVE, (t_canvas){.fov = 45.0f,
		.width = (float)app->vk_swap_chain_extent.width,
		.height = (float)app->vk_swap_chain_extent.height,
		.near = 0.1f, .far = 10.0f}, ubo.proj);
	ubo.proj[1][1] *= -1;
	vkMapMemory(app->vk_logical_device,
		 app->vk_uniform_buffers_memory[current_image], 0,
		 	sizeof(ubo), 0, &data);
	ft_memcpy(data, &ubo, sizeof(ubo));
	vkUnmapMemory(app->vk_logical_device,
		app->vk_uniform_buffers_memory[current_image]);
}

static void		submit_draw_queue(t_vkrenderer *app, uint32_t image_index,
				VkSubmitInfo *submit_info)
{
	ft_memset(submit_info, 0, sizeof(*submit_info));
	submit_info->sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submit_info->waitSemaphoreCount = 1;
	submit_info->pWaitSemaphores = (VkSemaphore[1]){
		app->vk_img_available_semaphores[app->vk_current_frame]};
	submit_info->pWaitDstStageMask = (VkPipelineStageFlags[1]){
		VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
	submit_info->commandBufferCount = 1;
	submit_info->pCommandBuffers = &app->vk_command_buffers[image_index];
	submit_info->signalSemaphoreCount = 1;
	submit_info->pSignalSemaphores = (VkSemaphore[1]){
		app->vk_rndr_finished_semaphores[app->vk_current_frame]};
	vkResetFences(app->vk_logical_device, 1,
		&app->vk_in_flight_fences[app->vk_current_frame]);
	error_check(vkQueueSubmit(app->vk_graphics_queue, 1, submit_info,
		app->vk_in_flight_fences[app->vk_current_frame]) != VK_SUCCESS,
			"Failed to submit draw command buffer!");
}

static int		acquire_swap_chain_image(t_vkrenderer *app,
				uint32_t *image_index)
{
	VkResult			res;

	res = vkAcquireNextImageKHR(app->vk_logical_device, app->vk_swap_chain,
		UINT64_MAX, app->vk_img_available_semaphores[app->vk_current_frame],
		VK_NULL_HANDLE, image_index);
	if (res == VK_ERROR_OUT_OF_DATE_KHR)
	{
		vulkan_recreate_swapchain(app);
		return (1);
	}
	else
		error_check(res != VK_SUCCESS && res != VK_SUBOPTIMAL_KHR,
			"Failed to acquire swap chain image!");
	return (0);
}

static void		present_image(t_vkrenderer *app, uint32_t image_index)
{
	VkPresentInfoKHR	presentInfo;
	VkResult			res;

	ft_memset(&presentInfo, 0, sizeof(presentInfo));
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = (VkSemaphore[1]){
		app->vk_rndr_finished_semaphores[app->vk_current_frame]};
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = (VkSwapchainKHR[1]){app->vk_swap_chain};
	presentInfo.pImageIndices = &image_index;
	res = vkQueuePresentKHR(app->vk_present_queue, &presentInfo);
	if ((res == VK_ERROR_OUT_OF_DATE_KHR || res == VK_SUBOPTIMAL_KHR ||
		app->frame_buffer_resized) && !(app->frame_buffer_resized = false))
		vulkan_recreate_swapchain(app);
	else
		error_check(res != VK_SUCCESS, "Failed to present swap chain image!");
}

void			vulkan_draw_frame(t_vkrenderer *app)
{
	uint32_t			image_index;
	VkSubmitInfo		submit_info;

	vkWaitForFences(app->vk_logical_device, 1,
		&app->vk_in_flight_fences[app->vk_current_frame], VK_TRUE, UINT64_MAX);
	if (acquire_swap_chain_image(app, &image_index))
		return ;
	if (app->vk_images_in_flight[image_index] != VK_NULL_HANDLE)
		vkWaitForFences(app->vk_logical_device, 1,
			&app->vk_images_in_flight[image_index], VK_TRUE, UINT64_MAX);
	app->vk_images_in_flight[image_index] =
		app->vk_in_flight_fences[app->vk_current_frame];
	vulkan_update_uniform_buffer(app, image_index);
	submit_draw_queue(app, image_index, &submit_info);
	present_image(app, image_index);
	app->vk_current_frame = (app->vk_current_frame + 1) % MAXFRAMESINFLIGHT;
}
