/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vulkan_vertex.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/10 15:30:13 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/10 16:13:23 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cvulkan.h"

VkVertexInputBindingDescription		vulkan_vertex_binding_description()
{
	VkVertexInputBindingDescription bindingDescription;

	ft_memset(&bindingDescription, 0, sizeof(bindingDescription));
	bindingDescription.binding = 0;
	bindingDescription.stride = sizeof(t_vulkan_vertex);
	bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
	return bindingDescription;
}

void								vulkan_vertex_fill_attribute_descriptions(
									VkVertexInputAttributeDescription
									attributeDescriptions[3])
{
	t_vulkan_vertex						vtx;

	attributeDescriptions[0].binding = 0;
	attributeDescriptions[0].location = 0;
	attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[0].offset = 0;
	attributeDescriptions[1].binding = 0;
	attributeDescriptions[1].location = 1;
	attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[1].offset = sizeof(vtx.pos);
	attributeDescriptions[2].binding = 0;
	attributeDescriptions[2].location = 2;
	attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
	attributeDescriptions[2].offset = sizeof(vtx.pos) + sizeof(vtx.color);
}
