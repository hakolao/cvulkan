/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vulkan_descriptor_set.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/10 13:29:03 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/12 18:38:10 by ohakola          ###   ########.fr       */
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

void	vulkan_create_descriptor_pool(t_cvulkan *app)
{
	VkDescriptorPoolSize		poolSizes[2];
	VkDescriptorPoolCreateInfo	poolInfo;

	poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSizes[0].descriptorCount = app->vk_swap_chain_images_count;
	poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	poolSizes[1].descriptorCount = app->vk_swap_chain_images_count;
	ft_memset(&poolInfo, 0, sizeof(poolInfo));
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = 2;
	poolInfo.pPoolSizes = poolSizes;
	poolInfo.maxSets = app->vk_swap_chain_images_count;
	error_check(vkCreateDescriptorPool(app->vk_logical_device, &poolInfo, NULL,
		&app->vk_descriptor_pool) != VK_SUCCESS,
			"Failed to create descriptor pool!");
}
