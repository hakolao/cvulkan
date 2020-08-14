/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vulkan_vertex.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/10 15:30:13 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/14 20:47:47 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cvulkan.h"

static VkVertexInputBindingDescription		*create_vertex_binding_description()
{
	VkVertexInputBindingDescription *binding_description;

	error_check(!(binding_description = malloc(sizeof(*binding_description))),
		"Failed to malloc!");
	ft_memset(binding_description, 0, sizeof(*binding_description));
	binding_description->binding = 0;
	binding_description->stride = sizeof(t_vulkan_vertex);
	binding_description->inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
	return (binding_description);
}

static VkVertexInputAttributeDescription	*create_vtx_attribute_descriptions(
											uint32_t *count)
{
	t_vulkan_vertex						vtx;
	VkVertexInputAttributeDescription	*attribute_descriptions;

	error_check(!(attribute_descriptions =
		malloc(sizeof(*attribute_descriptions) * 3)), "Failed to malloc!");
	ft_memset(attribute_descriptions, 0, sizeof(*attribute_descriptions) * 3);
	attribute_descriptions[0].binding = 0;
	attribute_descriptions[0].location = 0;
	attribute_descriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
	attribute_descriptions[0].offset = 0;
	attribute_descriptions[1].binding = 0;
	attribute_descriptions[1].location = 1;
	attribute_descriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
	attribute_descriptions[1].offset = sizeof(vtx.pos);
	attribute_descriptions[2].binding = 0;
	attribute_descriptions[2].location = 2;
	attribute_descriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
	attribute_descriptions[2].offset = sizeof(vtx.pos) + sizeof(vtx.color);
	*count = 3;
	return (attribute_descriptions);
}

VkPipelineVertexInputStateCreateInfo		*vulkan_create_vertex_input_create_info()
{
	VkPipelineVertexInputStateCreateInfo	*vertex_input_info;
	uint32_t								attribute_count;

	error_check(!(vertex_input_info = malloc(sizeof(*vertex_input_info))),
		"Failed to malloc!");
	ft_memset(vertex_input_info, 0, sizeof(*vertex_input_info));
	vertex_input_info->sType =
		VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertex_input_info->vertexBindingDescriptionCount = 1;
	vertex_input_info->pVertexBindingDescriptions =
		create_vertex_binding_description();
	vertex_input_info->pVertexAttributeDescriptions =
		create_vtx_attribute_descriptions(&attribute_count);
	vertex_input_info->vertexAttributeDescriptionCount = attribute_count;
	return (vertex_input_info);
}

void										vulkan_free_vertex_input_create_info(
											const
											VkPipelineVertexInputStateCreateInfo
											*create_info)
{
	free((void*)create_info->pVertexAttributeDescriptions);
	free((void*)create_info->pVertexBindingDescriptions);
	free((void*)create_info);
}
