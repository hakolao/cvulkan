/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vulkan_graphics_pipeline_info2.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/10 23:35:07 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/10 23:58:00 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cvulkan.h"


VkPipelineViewportStateCreateInfo		*vulkan_create_viewport_create_info(t_cvulkan
										*app)
{
	VkPipelineViewportStateCreateInfo	*viewport_state;
	VkRect2D							*scissor;
	VkViewport							*viewport;

	error_check(!(viewport_state = malloc(sizeof(*viewport_state))),
		"Failed to malloc!");
	error_check(!(scissor = malloc(sizeof(*scissor))), "Failed to malloc!");
	error_check(!(viewport = malloc(sizeof(*viewport))), "Failed to malloc!");
	ft_memset(viewport_state, 0, sizeof(*viewport_state));
	ft_memset(scissor, 0, sizeof(*scissor));
	ft_memset(viewport, 0, sizeof(*viewport));
	viewport->x = 0.0f;
	viewport->y = 0.0f;
	viewport->width = (float)app->vk_swap_chain_extent.width;
	viewport->height = (float)app->vk_swap_chain_extent.height;
	viewport->minDepth = 0.0f;
	viewport->maxDepth = 1.0f;
	scissor->offset = (VkOffset2D){0, 0};
	scissor->extent = app->vk_swap_chain_extent;
	viewport_state->sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewport_state->viewportCount = 1;
	viewport_state->pViewports = viewport;
	viewport_state->scissorCount = 1;
	viewport_state->pScissors = scissor;
	return (viewport_state);
}

void									vulkan_free_viewport_create_info(
										const VkPipelineViewportStateCreateInfo
										*create_info)
{
	free((void*)create_info->pScissors);
	free((void*)create_info->pViewports);
	free((void*)create_info);
}

VkPipelineInputAssemblyStateCreateInfo	*create_input_assembly_create_info()
{
	VkPipelineInputAssemblyStateCreateInfo	*input_assembly;

	error_check(!(input_assembly = malloc(sizeof(*input_assembly))),
		"Failed to malloc!");
	ft_memset(input_assembly, 0, sizeof(*input_assembly));
	input_assembly->sType =
		VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	input_assembly->topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	input_assembly->primitiveRestartEnable = VK_FALSE;
	return (input_assembly);
}
