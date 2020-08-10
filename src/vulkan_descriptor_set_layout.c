/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vulkan_descriptor_set_layout.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/10 13:29:03 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/10 13:38:55 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cvulkan.h"

void	vulkan_create_descriptor_set_layout(t_cvulkan *app)
{
	VkDescriptorSetLayoutBinding	ubo_layout_binding;
	VkDescriptorSetLayoutBinding	sampler_layout_binding;
	VkDescriptorSetLayoutCreateInfo	layout_info;

	ft_memset(&ubo_layout_binding, 0, sizeof(ubo_layout_binding));
	ft_memset(&sampler_layout_binding, 0, sizeof(sampler_layout_binding));
	ft_memset(&layout_info, 0, sizeof(layout_info));
	ubo_layout_binding.binding = 0;
	ubo_layout_binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	ubo_layout_binding.descriptorCount = 1;
	ubo_layout_binding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	sampler_layout_binding.binding = 1;
	sampler_layout_binding.descriptorCount = 1;
	sampler_layout_binding.descriptorType =
		VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	sampler_layout_binding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	layout_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layout_info.bindingCount = 2;
	layout_info.pBindings =
		(VkDescriptorSetLayoutBinding[2]){ubo_layout_binding,
		sampler_layout_binding};
	error_check(vkCreateDescriptorSetLayout(app->vk_logical_device,
		&layout_info, NULL, &app->vk_descriptor_set_layout) != VK_SUCCESS,
				"Failed to create descriptor set layout!");
}
