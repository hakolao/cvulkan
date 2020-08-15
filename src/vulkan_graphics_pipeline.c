/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vulkan_graphics_pipeline.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/10 15:10:45 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/15 22:49:06 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cvulkan.h"

static void								create_pipeline_layout(t_vkrenderer *app)
{
	VkPipelineLayoutCreateInfo				pipelineLayoutInfo;

	ft_memset(&pipelineLayoutInfo, 0, sizeof(pipelineLayoutInfo));
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 1;
	pipelineLayoutInfo.pSetLayouts = &app->vk_descriptor_set_layout;
	pipelineLayoutInfo.pushConstantRangeCount = 0;
	error_check(vkCreatePipelineLayout(app->vk_logical_device,
		&pipelineLayoutInfo, NULL, &app->vk_pipeline_layout) != VK_SUCCESS,
				"Failed to create pipeline layout!");
}

static void								free_graphics_pipeline_info(
										t_vkrenderer *app,
										VkGraphicsPipelineCreateInfo
										*pipeline_info)
{
	free_color_blend_create_info(pipeline_info->pColorBlendState);
	vulkan_free_viewport_create_info(pipeline_info->pViewportState);
	vulkan_free_vertex_input_create_info(pipeline_info->pVertexInputState);
	free((void*)pipeline_info->pDepthStencilState);
	free((void*)pipeline_info->pMultisampleState);
	free((void*)pipeline_info->pRasterizationState);
	free((void*)pipeline_info->pInputAssemblyState);
	vulkan_destroy_shader_modules(app,
		pipeline_info->stageCount, pipeline_info->pStages);
}

void									vulkan_create_graphics_pipeline(
										t_vkrenderer *app)
{
	VkGraphicsPipelineCreateInfo			pipeline_info;

	create_pipeline_layout(app);
	ft_memset(&pipeline_info, 0, sizeof(pipeline_info));
	pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipeline_info.pStages = vulkan_create_shader_stages_info(app,
		&pipeline_info.stageCount);
	pipeline_info.pVertexInputState = vulkan_create_vertex_input_create_info();
	pipeline_info.pInputAssemblyState = create_input_assembly_create_info();
	pipeline_info.pViewportState = vulkan_create_viewport_create_info(app);
	pipeline_info.pRasterizationState = vulkan_create_rasterization_create_info();
	pipeline_info.pMultisampleState =
		vulkan_create_multisample_create_info(app->vk_msaa_samples);
	pipeline_info.pColorBlendState = vulkan_create_color_blend_create_info();
	pipeline_info.layout = app->vk_pipeline_layout;
	pipeline_info.renderPass = app->vk_render_pass;
	pipeline_info.subpass = 0;
	pipeline_info.basePipelineHandle = VK_NULL_HANDLE;
	pipeline_info.pDepthStencilState = vulkan_create_depth_stencil_create_info();
	error_check(vkCreateGraphicsPipelines(app->vk_logical_device,
		VK_NULL_HANDLE, 1, &pipeline_info, NULL, &app->vk_graphics_pipeline)
			!= VK_SUCCESS, "Failed to create graphics pipeline!");
	free_graphics_pipeline_info(app, &pipeline_info);
}
