/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vulkan_draw.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/13 15:33:11 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/15 20:55:42 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cvulkan.h"
#include <time.h>

static void		vulkan_update_uniform_buffer(t_cvulkan *app,
				uint32_t currentImage)
{
	t_uniform_buffer_object		ubo;
	mat4						model;
	void						*data;
	static clock_t				start_time;
	clock_t						current_time;
	float						time_elapsed;

	start_time = start_time == 0 ? clock() : start_time;
	current_time = clock();
	time_elapsed = (double)(current_time - start_time) / CLOCKS_PER_SEC * 10.0f;
	glm_mat4_identity(model);
	glm_rotate_z(model, time_elapsed * glm_rad(90.0f), ubo.model);
	glm_lookat((vec3){2.0f, 2.0f, 2.0f}, (vec3){0.0f, 0.0f, 0.0f},
		(vec3){0.0f, 0.0f, 1.0f}, ubo.view);
	glm_perspective(glm_rad(45.0f), (float)app->vk_swap_chain_extent.width /
		(float)app->vk_swap_chain_extent.height, 0.1f, 10.0f, ubo.proj);
	ubo.proj[1][1] *= -1;
	vkMapMemory(app->vk_logical_device,
		 app->vk_uniform_buffers_memory[currentImage], 0,
		 	sizeof(ubo), 0, &data);
	ft_memcpy(data, &ubo, sizeof(ubo));
	vkUnmapMemory(app->vk_logical_device,
		app->vk_uniform_buffers_memory[currentImage]);


}

void			vulkan_draw(t_cvulkan *app)
{
	uint32_t			imageIndex;
	VkResult			res;
	VkSubmitInfo		submitInfo;
	VkPresentInfoKHR	presentInfo;

	vkWaitForFences(app->vk_logical_device, 1,
		&app->vk_in_flight_fences[app->vk_current_frame], VK_TRUE, UINT64_MAX);
	res = vkAcquireNextImageKHR(app->vk_logical_device, app->vk_swap_chain,
		UINT64_MAX, app->vk_img_available_semaphores[app->vk_current_frame],
		VK_NULL_HANDLE, &imageIndex);
	if (res == VK_ERROR_OUT_OF_DATE_KHR)
	{
		vulkan_recreate_swapchain(app);
		return ;
	}
	else
		error_check(res != VK_SUCCESS && res != VK_SUBOPTIMAL_KHR,
			"Failed to acquire swap chain image!");
	if (app->vk_images_in_flight[imageIndex] != VK_NULL_HANDLE)
		vkWaitForFences(app->vk_logical_device, 1,
			&app->vk_images_in_flight[imageIndex], VK_TRUE, UINT64_MAX);
	app->vk_images_in_flight[imageIndex] =
		app->vk_in_flight_fences[app->vk_current_frame];
	vulkan_update_uniform_buffer(app, imageIndex);
	ft_memset(&submitInfo, 0, sizeof(submitInfo));
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = (VkSemaphore[1]){
		app->vk_img_available_semaphores[app->vk_current_frame]};
	submitInfo.pWaitDstStageMask = (VkPipelineStageFlags[1]){
		VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &app->vk_command_buffers[imageIndex];
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = (VkSemaphore[1]){
		app->vk_rndr_finished_semaphores[app->vk_current_frame]};
	vkResetFences(app->vk_logical_device, 1,
		&app->vk_in_flight_fences[app->vk_current_frame]);
	error_check(vkQueueSubmit(app->vk_graphics_queue, 1, &submitInfo,
		app->vk_in_flight_fences[app->vk_current_frame]) != VK_SUCCESS,
			"Failed to submit draw command buffer!");
	ft_memset(&presentInfo, 0, sizeof(presentInfo));
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = submitInfo.pSignalSemaphores;
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = (VkSwapchainKHR[1]){app->vk_swap_chain};
	presentInfo.pImageIndices = &imageIndex;
	res = vkQueuePresentKHR(app->vk_present_queue, &presentInfo);
	if ((res == VK_ERROR_OUT_OF_DATE_KHR || res == VK_SUBOPTIMAL_KHR ||
		app->frame_buffer_resized) && !(app->frame_buffer_resized = false))
		vulkan_recreate_swapchain(app);
	else
		error_check(res != VK_SUCCESS, "Failed to present swap chain image!");
	app->vk_current_frame = (app->vk_current_frame + 1) % MAXFRAMESINFLIGHT;
}
