/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vulkan_graphics_pipeline.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/10 15:10:45 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/10 22:44:49 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cvulkan.h"

static VkShaderModule 					vulkan_create_shader_module(t_cvulkan
										*app,
										t_file_contents *code)
{
	VkShaderModuleCreateInfo	create_info;
	VkShaderModule				shader_module;

	ft_memset(&create_info, 0, sizeof(create_info));
	create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	create_info.codeSize = code->size;
	create_info.pCode = (uint32_t*)code->buf;
	error_check(vkCreateShaderModule(app->vk_logical_device, &create_info, NULL,
		&shader_module) != VK_SUCCESS, "Failed to create shader module!");
	free_file_contents(code);
	return (shader_module);
}

static void								create_pipeline_layout(t_cvulkan *app)
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

VkPipelineColorBlendStateCreateInfo		*create_color_blend_create_info()
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

VkPipelineDepthStencilStateCreateInfo	*create_depth_stencil_create_info()
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

VkPipelineMultisampleStateCreateInfo	*create_multisample_create_info(
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

VkPipelineRasterizationStateCreateInfo	*create_rasterization_create_info()
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

VkPipelineViewportStateCreateInfo		*create_viewport_create_info(t_cvulkan
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

void									free_viewport_create_info(
										const VkPipelineViewportStateCreateInfo
										*create_info)
{
	free((void*)create_info->pScissors);
	free((void*)create_info->pViewports);
	free((void*)create_info);
}

void									vulkan_create_graphics_pipeline(
										t_cvulkan *app)
{
	VkShaderModule							vertShaderModule;
	VkShaderModule							fragShaderModule;
	VkVertexInputBindingDescription			bindingDescription;
	VkVertexInputAttributeDescription		attributeDescriptions[3];
	VkGraphicsPipelineCreateInfo			pipelineInfo;

	create_pipeline_layout(app);
	vertShaderModule = vulkan_create_shader_module(app,
		read_file("shaders/vert.spv"));
	fragShaderModule = vulkan_create_shader_module(app,
		read_file("shaders/frag.spv"));

	ft_memset(&pipelineInfo, 0, sizeof(pipelineInfo));
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;

	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = (VkPipelineShaderStageCreateInfo[2]){
		(VkPipelineShaderStageCreateInfo){
		.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
		.flags = 0, .pNext = NULL, .pSpecializationInfo = NULL, .pName = "main",
		.module = vertShaderModule, .stage = VK_SHADER_STAGE_VERTEX_BIT},
		(VkPipelineShaderStageCreateInfo){
		.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
		.flags = 0, .pNext = NULL, .pSpecializationInfo = NULL, .pName = "main",
		.module = fragShaderModule, .stage = VK_SHADER_STAGE_FRAGMENT_BIT}};

	vulkan_vertex_fill_attribute_descriptions(attributeDescriptions);
	bindingDescription = vulkan_vertex_binding_description();
	pipelineInfo.pVertexInputState = &(VkPipelineVertexInputStateCreateInfo){
		.pNext = NULL, .flags = 0,
		.pVertexAttributeDescriptions = attributeDescriptions,
		.pVertexBindingDescriptions = &bindingDescription,
		.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
		.vertexBindingDescriptionCount = 1,
		.vertexAttributeDescriptionCount = 3};

	pipelineInfo.pInputAssemblyState =
		&(VkPipelineInputAssemblyStateCreateInfo){.flags = 0,
		.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
		.primitiveRestartEnable = VK_FALSE,
		.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, .pNext = NULL};

	pipelineInfo.pViewportState = create_viewport_create_info(app);
	pipelineInfo.pRasterizationState = create_rasterization_create_info();
	pipelineInfo.pMultisampleState =
		create_multisample_create_info(app->vk_msaa_samples);
	pipelineInfo.pColorBlendState = create_color_blend_create_info();
	pipelineInfo.layout = app->vk_pipeline_layout;
	pipelineInfo.renderPass = app->vk_render_pass;
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
	pipelineInfo.pDepthStencilState = create_depth_stencil_create_info();
	error_check(vkCreateGraphicsPipelines(app->vk_logical_device,
		VK_NULL_HANDLE, 1, &pipelineInfo, NULL, &app->vk_graphics_pipeline)
			!= VK_SUCCESS, "Failed to create graphics pipeline!");
	free_color_blend_create_info(pipelineInfo.pColorBlendState);
	free_viewport_create_info(pipelineInfo.pViewportState);
	free((void*)pipelineInfo.pDepthStencilState);
	free((void*)pipelineInfo.pMultisampleState);
	free((void*)pipelineInfo.pRasterizationState);
	vkDestroyShaderModule(app->vk_logical_device, fragShaderModule, NULL);
	vkDestroyShaderModule(app->vk_logical_device, vertShaderModule, NULL);
}
