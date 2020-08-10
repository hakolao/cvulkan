/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vulkan_graphics_pipeline_utils.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/10 16:32:08 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/10 23:39:12 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cvulkan.h"

VkPipelineColorBlendStateCreateInfo		*vulkan_create_color_blend_create_info()
{
	VkPipelineColorBlendStateCreateInfo	*color_blending;
	VkPipelineColorBlendAttachmentState	*color_blend_attachment;

	error_check(!(color_blending = malloc(sizeof(*color_blending))),
		"Failed to malloc!");
	error_check(!(color_blend_attachment =
		malloc(sizeof(*color_blend_attachment))), "Failed to malloc!");
	ft_memset(color_blend_attachment, 0, sizeof(*color_blend_attachment));
	color_blend_attachment->colorWriteMask =
		VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
		VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	color_blend_attachment->blendEnable = VK_FALSE;
	ft_memset(color_blending, 0, sizeof(*color_blending));
	color_blending->sType =
		VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	color_blending->logicOpEnable = VK_FALSE;
	color_blending->logicOp = VK_LOGIC_OP_COPY;
	color_blending->attachmentCount = 1;
	color_blending->pAttachments = color_blend_attachment;
	color_blending->blendConstants[0] = 0.0f;
	color_blending->blendConstants[1] = 0.0f;
	color_blending->blendConstants[2] = 0.0f;
	color_blending->blendConstants[3] = 0.0f;
	return (color_blending);
}

void									free_color_blend_create_info(
										const
										VkPipelineColorBlendStateCreateInfo
										*color_blending)
{
	free((void*)color_blending->pAttachments);
	free((void*)color_blending);
}

VkPipelineDepthStencilStateCreateInfo	*vulkan_create_depth_stencil_create_info()
{
	VkPipelineDepthStencilStateCreateInfo	*depth_stencil_state;
	error_check(!(depth_stencil_state = malloc(sizeof(*depth_stencil_state))),
		"Failed to malloc!");
	ft_memset(depth_stencil_state, 0, sizeof(*depth_stencil_state));
	depth_stencil_state->sType =
		VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depth_stencil_state->depthTestEnable = VK_TRUE;
	depth_stencil_state->depthWriteEnable = VK_TRUE;
	depth_stencil_state->depthCompareOp = VK_COMPARE_OP_LESS;
	depth_stencil_state->depthBoundsTestEnable = VK_FALSE;
	depth_stencil_state->stencilTestEnable = VK_FALSE;
	return (depth_stencil_state);
}

VkPipelineMultisampleStateCreateInfo	*vulkan_create_multisample_create_info(
										VkSampleCountFlagBits msaa_samples)
{
	VkPipelineMultisampleStateCreateInfo	*create_info;

	error_check(!(create_info = malloc(sizeof(*create_info))),
		"Failed to malloc!");
	ft_memset(create_info, 0, sizeof(*create_info));
	create_info->sType =
		VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	create_info->sampleShadingEnable = VK_TRUE;
	create_info->minSampleShading = .2f;
	create_info->rasterizationSamples = msaa_samples;
	return (create_info);
}

VkPipelineRasterizationStateCreateInfo	*vulkan_create_rasterization_create_info()
{
	VkPipelineRasterizationStateCreateInfo	*rasterizer;

	error_check(!(rasterizer = malloc(sizeof(*rasterizer))),
		"Failed to malloc!");
	ft_memset(rasterizer, 0, sizeof(*rasterizer));
	rasterizer->sType =
		VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer->depthClampEnable = VK_FALSE;
	rasterizer->rasterizerDiscardEnable = VK_FALSE;
	rasterizer->polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer->lineWidth = 1.0f;
	rasterizer->cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizer->frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	rasterizer->depthBiasEnable = VK_FALSE;
	return (rasterizer);
}
