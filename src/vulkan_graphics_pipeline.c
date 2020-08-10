/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vulkan_graphics_pipeline.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/10 15:10:45 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/10 17:40:56 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cvulkan.h"

static VkShaderModule 		vulkan_create_shader_module(t_cvulkan *app,
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

void						vulkan_create_graphics_pipeline(t_cvulkan *app)
{
	VkShaderModule							vertShaderModule;
	VkShaderModule							fragShaderModule;
	VkVertexInputBindingDescription			bindingDescription;
	VkPipelineColorBlendStateCreateInfo		colorBlending;
	VkVertexInputAttributeDescription		attributeDescriptions[3];
	VkPipelineDepthStencilStateCreateInfo	depthStencil;
	VkPipelineLayoutCreateInfo				pipelineLayoutInfo;
	VkGraphicsPipelineCreateInfo			pipelineInfo;

	vertShaderModule = vulkan_create_shader_module(app,
		read_file("shaders/vert.spv"));
	fragShaderModule = vulkan_create_shader_module(app,
		read_file("shaders/frag.spv"));

	ft_memset(&depthStencil, 0, sizeof(depthStencil));
	depthStencil.sType =
		VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencil.depthTestEnable = VK_TRUE;
	depthStencil.depthWriteEnable = VK_TRUE;
	depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
	depthStencil.depthBoundsTestEnable = VK_FALSE;
	depthStencil.stencilTestEnable = VK_FALSE;

	ft_memset(&pipelineLayoutInfo, 0, sizeof(pipelineLayoutInfo));
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 1;
	pipelineLayoutInfo.pSetLayouts = &app->vk_descriptor_set_layout;
	pipelineLayoutInfo.pushConstantRangeCount = 0;

	error_check(vkCreatePipelineLayout(app->vk_logical_device,
		&pipelineLayoutInfo, NULL, &app->vk_pipeline_layout) != VK_SUCCESS,
				"Failed to create pipeline layout!");

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
	pipelineInfo.pVertexInputState = &(VkPipelineVertexInputStateCreateInfo){.pNext = NULL, .flags = 0,
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
	pipelineInfo.pViewportState = &(VkPipelineViewportStateCreateInfo){
		.flags = 0, .pNext = NULL,
		.pScissors = &(VkRect2D){.offset = (VkOffset2D){0, 0}},
		.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
		.viewportCount = 1, .pViewports = &(VkViewport){.x = 0.0f, .y = 0.0f,
		.width = (float)app->vk_swap_chain_extent.width,
		.height = (float)app->vk_swap_chain_extent.height,
		.minDepth = 0.0f, .maxDepth = 0.0f}, .scissorCount = 1};
	pipelineInfo.pRasterizationState =
		&(VkPipelineRasterizationStateCreateInfo){.flags = 0, .pNext = NULL,
		.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
		.depthBiasSlopeFactor = 0.0f,
		.depthBiasConstantFactor = 0.0f, .depthBiasClamp = 0.0f,
		.depthClampEnable = VK_FALSE, .depthBiasEnable = VK_FALSE,
		.lineWidth = 1.0f, .cullMode = VK_CULL_MODE_BACK_BIT,
		.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE,
		.rasterizerDiscardEnable = VK_FALSE,
		.polygonMode = VK_POLYGON_MODE_FILL};
	pipelineInfo.pMultisampleState = &(VkPipelineMultisampleStateCreateInfo){
		.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
		.pNext = NULL,  .flags = 0, .pSampleMask = NULL,
		.alphaToCoverageEnable = VK_FALSE, .alphaToOneEnable = VK_FALSE,
		.sampleShadingEnable = VK_TRUE, .minSampleShading = .2f,
		.rasterizationSamples = app->vk_msaa_samples};
	colorBlending = (VkPipelineColorBlendStateCreateInfo){
		.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
		.pNext = NULL, .flags = 0, .attachmentCount = 1,
		.logicOpEnable = VK_FALSE, .logicOp = VK_LOGIC_OP_COPY,
		.pAttachments = &(VkPipelineColorBlendAttachmentState){
			.blendEnable = VK_FALSE, .alphaBlendOp = 0, .colorBlendOp = 0,
			.colorWriteMask =
				VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
				VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
			.dstAlphaBlendFactor = 0, .dstColorBlendFactor = 0,
			.srcAlphaBlendFactor = 0, .srcColorBlendFactor = 0
	}};
	colorBlending.blendConstants[0] = 0.0f;
	colorBlending.blendConstants[1] = 0.0f;
	colorBlending.blendConstants[2] = 0.0f;
	colorBlending.blendConstants[3] = 0.0f;
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.layout = app->vk_pipeline_layout;
	pipelineInfo.renderPass = app->vk_render_pass;
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
	pipelineInfo.pDepthStencilState = &depthStencil;

	error_check(
		vkCreateGraphicsPipelines(app->vk_logical_device, VK_NULL_HANDLE, 1,
			&pipelineInfo, NULL, &app->vk_graphics_pipeline) != VK_SUCCESS,
		"Failed to create graphics pipeline!");

	vkDestroyShaderModule(app->vk_logical_device, fragShaderModule, NULL);
	vkDestroyShaderModule(app->vk_logical_device, vertShaderModule, NULL);
}
