/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vulkan_graphics_pipeline_info3.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/10 23:57:37 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/15 22:49:06 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cvulkan.h"

static VkShaderModule 					vulkan_create_shader_module(t_vkrenderer
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

VkPipelineShaderStageCreateInfo			*vulkan_create_shader_stages_info(
										t_vkrenderer *app,
										uint32_t *stage_count)
{
	VkPipelineShaderStageCreateInfo	*shader_stages;

	*stage_count = 2;
	error_check(!(shader_stages =
		malloc(sizeof(*shader_stages) * *stage_count)), "Failed to malloc!");
	ft_memset(shader_stages, 0,
		sizeof(*shader_stages) * *stage_count);
	shader_stages[0].sType =
		VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shader_stages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
	shader_stages[0].module = vulkan_create_shader_module(app,
		read_file("shaders/vert.spv"));
	shader_stages[0].pName = "main";
	shader_stages[1].sType =
		VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shader_stages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	shader_stages[1].module = vulkan_create_shader_module(app,
		read_file("shaders/frag.spv"));
	shader_stages[1].pName = "main";
	return (shader_stages);
}

void									vulkan_destroy_shader_modules(
										t_vkrenderer *app,
										uint32_t stage_count,
										const VkPipelineShaderStageCreateInfo
										*shader_stages)
{
	size_t		i;

	i = -1;
	while (++i < stage_count)
		vkDestroyShaderModule(app->vk_logical_device,
			shader_stages[i].module, NULL);
	free((void*)shader_stages);
}
