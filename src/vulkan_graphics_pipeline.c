/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vulkan_graphics_pipeline.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/10 15:10:45 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/10 16:14:31 by ohakola          ###   ########.fr       */
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
	return (shader_module);
}

void						vulkan_create_graphics_pipeline(t_cvulkan *app)
{
	t_file_contents *vertShaderCode;
	t_file_contents *fragShaderCode;

 	vertShaderCode = read_file("shaders/vert.spv");
	fragShaderCode = read_file("shaders/frag.spv");
	VkShaderModule vertShaderModule =
		vulkan_create_shader_module(app, vertShaderCode);
	VkShaderModule fragShaderModule =
		vulkan_create_shader_module(app, fragShaderCode);
	free_file_contents(vertShaderCode);
	free_file_contents(fragShaderCode);

	VkPipelineShaderStageCreateInfo vertShaderStageInfo;
	ft_memset(&vertShaderStageInfo, 0, sizeof(vertShaderStageInfo));
	vertShaderStageInfo.sType =
		VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertShaderStageInfo.module = vertShaderModule;
	vertShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo fragShaderStageInfo;
	ft_memset(&fragShaderStageInfo, 0, sizeof(fragShaderStageInfo));
	fragShaderStageInfo.sType =
		VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = fragShaderModule;
	fragShaderStageInfo.pName = "main";

	VkPipelineVertexInputStateCreateInfo vertexInputInfo;
	ft_memset(&vertexInputInfo, 0, sizeof(vertexInputInfo));
	vertexInputInfo.sType =
		VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

	VkVertexInputBindingDescription bindingDescription =
		vulkan_vertex_binding_description();
	VkVertexInputAttributeDescription	attributeDescriptions[3];
	vulkan_vertex_fill_attribute_descriptions(attributeDescriptions);

	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.vertexAttributeDescriptionCount = 3;
	vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions;

	VkPipelineInputAssemblyStateCreateInfo inputAssembly;
	ft_memset(&inputAssembly, 0, sizeof(inputAssembly));
	inputAssembly.sType =
		VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	VkViewport viewport;
	ft_memset(&viewport, 0, sizeof(viewport));
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)app->vk_swap_chain_extent.width;
	viewport.height = (float)app->vk_swap_chain_extent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor;
	ft_memset(&scissor, 0, sizeof(scissor));
	scissor.offset = (VkOffset2D){0, 0};
	scissor.extent = app->vk_swap_chain_extent;

	VkPipelineViewportStateCreateInfo viewportState;
	ft_memset(&viewportState, 0, sizeof(viewportState));
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.pViewports = &viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &scissor;

	VkPipelineRasterizationStateCreateInfo rasterizer;
	ft_memset(&rasterizer, 0, sizeof(rasterizer));
	rasterizer.sType =
		VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;

	VkPipelineMultisampleStateCreateInfo multisampling;
	ft_memset(&multisampling, 0, sizeof(multisampling));
	multisampling.sType =
		VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_TRUE;
	multisampling.minSampleShading = .2f;
	multisampling.rasterizationSamples = app->vk_msaa_samples;

	VkPipelineDepthStencilStateCreateInfo depthStencil;
	ft_memset(&depthStencil, 0, sizeof(depthStencil));
	depthStencil.sType =
		VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencil.depthTestEnable = VK_TRUE;
	depthStencil.depthWriteEnable = VK_TRUE;
	depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
	depthStencil.depthBoundsTestEnable = VK_FALSE;
	depthStencil.stencilTestEnable = VK_FALSE;

	VkPipelineColorBlendAttachmentState colorBlendAttachment;
	ft_memset(&colorBlendAttachment, 0, sizeof(colorBlendAttachment));
	colorBlendAttachment.colorWriteMask =
		VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
		VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;

	VkPipelineColorBlendStateCreateInfo colorBlending;
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

	VkPipelineLayoutCreateInfo pipelineLayoutInfo;
	ft_memset(&pipelineLayoutInfo, 0, sizeof(pipelineLayoutInfo));
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 1;
	pipelineLayoutInfo.pSetLayouts = &app->vk_descriptor_set_layout;
	pipelineLayoutInfo.pushConstantRangeCount = 0;

	error_check(vkCreatePipelineLayout(app->vk_logical_device,
		&pipelineLayoutInfo, NULL, &app->vk_pipeline_layout) != VK_SUCCESS,
				"Failed to create pipeline layout!");

	VkGraphicsPipelineCreateInfo pipelineInfo;
	ft_memset(&pipelineInfo, 0, sizeof(pipelineInfo));
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = (VkPipelineShaderStageCreateInfo[2]){
		vertShaderStageInfo, fragShaderStageInfo};
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
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
