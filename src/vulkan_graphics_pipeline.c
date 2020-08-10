/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vulkan_graphics_pipeline.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/10 15:10:45 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/10 17:09:49 by ohakola          ###   ########.fr       */
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
	VkPipelineShaderStageCreateInfo			vertShaderStageInfo;
	VkPipelineShaderStageCreateInfo			fragShaderStageInfo;
	VkPipelineVertexInputStateCreateInfo	vertexInputInfo;
	VkVertexInputBindingDescription			bindingDescription;
	VkVertexInputAttributeDescription		attributeDescriptions[3];
	VkPipelineInputAssemblyStateCreateInfo	inputAssembly;
	VkPipelineMultisampleStateCreateInfo	multisampling;
	VkPipelineDepthStencilStateCreateInfo	depthStencil;
	VkPipelineColorBlendAttachmentState		colorBlendAttachment;
	VkPipelineColorBlendStateCreateInfo		colorBlending;
	VkPipelineLayoutCreateInfo				pipelineLayoutInfo;
	VkGraphicsPipelineCreateInfo			pipelineInfo;

	vertShaderModule = vulkan_create_shader_module(app,
		read_file("shaders/vert.spv"));
	fragShaderModule = vulkan_create_shader_module(app,
		read_file("shaders/frag.spv"));

	ft_memset(&vertShaderStageInfo, 0, sizeof(vertShaderStageInfo));
	vertShaderStageInfo.sType =
		VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertShaderStageInfo.module = vertShaderModule;
	vertShaderStageInfo.pName = "main";

	ft_memset(&fragShaderStageInfo, 0, sizeof(fragShaderStageInfo));
	fragShaderStageInfo.sType =
		VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = fragShaderModule;
	fragShaderStageInfo.pName = "main";

	ft_memset(&vertexInputInfo, 0, sizeof(vertexInputInfo));
	vertexInputInfo.sType =
		VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

	bindingDescription = vulkan_vertex_binding_description();
	vulkan_vertex_fill_attribute_descriptions(attributeDescriptions);

	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.vertexAttributeDescriptionCount = 3;
	vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions;

	ft_memset(&inputAssembly, 0, sizeof(inputAssembly));
	inputAssembly.sType =
		VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	ft_memset(&multisampling, 0, sizeof(multisampling));
	multisampling.sType =
		VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_TRUE;
	multisampling.minSampleShading = .2f;
	multisampling.rasterizationSamples = app->vk_msaa_samples;

	ft_memset(&depthStencil, 0, sizeof(depthStencil));
	depthStencil.sType =
		VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencil.depthTestEnable = VK_TRUE;
	depthStencil.depthWriteEnable = VK_TRUE;
	depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
	depthStencil.depthBoundsTestEnable = VK_FALSE;
	depthStencil.stencilTestEnable = VK_FALSE;

	ft_memset(&colorBlendAttachment, 0, sizeof(colorBlendAttachment));
	colorBlendAttachment.colorWriteMask =
		VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
		VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;

	ft_memset(&colorBlending, 0, sizeof(colorBlending));
	colorBlending.sType =
		VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = VK_LOGIC_OP_COPY;
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;
	colorBlending.blendConstants[0] = 0.0f;
	colorBlending.blendConstants[1] = 0.0f;
	colorBlending.blendConstants[2] = 0.0f;
	colorBlending.blendConstants[3] = 0.0f;

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
		vertShaderStageInfo, fragShaderStageInfo};
	// vulkan_pipeline_info_set_vertex_input();
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	// vulkan_pipeline_info_set_input_assembly();
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	// vulkan_pipeline_info_set_viewpor_state();

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
	// vulkan_pipeline_info_set_multisampling();
	pipelineInfo.pMultisampleState = &multisampling;
	// vulkan_pipeline_info_set_color_blending();
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
