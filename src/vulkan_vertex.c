/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vulkan_vertex.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/10 15:30:13 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/10 23:26:26 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cvulkan.h"

VkVertexInputBindingDescription		*vulkan_create_vertex_binding_description()
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

VkVertexInputAttributeDescription	*vulkan_create_vtx_attribute_descriptions(
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
