/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vulkan_descriptor_set.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/10 13:29:03 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/15 22:49:06 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cvulkan.h"

static void	populate_descriptor_writes(VkDescriptorBufferInfo *bufferInfo,
			VkDescriptorImageInfo *imageInfo,
			VkWriteDescriptorSet descriptorWrites[2])
{
	ft_memset(&descriptorWrites[0], 0, sizeof(descriptorWrites[0]));
	descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrites[0].dstBinding = 0;
	descriptorWrites[0].dstArrayElement = 0;
	descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	descriptorWrites[0].descriptorCount = 1;
	descriptorWrites[0].pBufferInfo = bufferInfo;
	ft_memset(&descriptorWrites[1], 0, sizeof(descriptorWrites[1]));
	descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrites[1].dstBinding = 1;
	descriptorWrites[1].dstArrayElement = 0;
	descriptorWrites[1].descriptorType =
		VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	descriptorWrites[1].descriptorCount = 1;
	descriptorWrites[1].pImageInfo = imageInfo;
}

static void	create_descriptor_set(t_vkrenderer *app, size_t i)
{
	VkDescriptorBufferInfo			bufferInfo;
	VkDescriptorImageInfo			imageInfo;
	VkWriteDescriptorSet			descriptorWrites[2];

	ft_memset(&bufferInfo, 0, sizeof(bufferInfo));
	bufferInfo.buffer = app->vk_uniform_buffers[i];
	bufferInfo.offset = 0;
	bufferInfo.range = sizeof(t_uniform_buffer_object);
	ft_memset(&imageInfo, 0, sizeof(imageInfo));
	imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	imageInfo.imageView = app->vk_texture_image_view;
	imageInfo.sampler = app->vk_texture_sampler;
	populate_descriptor_writes(&bufferInfo, &imageInfo, descriptorWrites);
	descriptorWrites[0].dstSet = app->vk_descriptor_sets[i];
	descriptorWrites[1].dstSet = app->vk_descriptor_sets[i];
	vkUpdateDescriptorSets(app->vk_logical_device, 2, descriptorWrites, 0,
		NULL);
}

void		vulkan_create_descriptor_set_layout(t_vkrenderer *app)
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

void		vulkan_create_descriptor_pool(t_vkrenderer *app)
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

void		vulkan_create_descriptor_sets(t_vkrenderer *app)
{
	VkDescriptorSetLayout			layouts[app->vk_swap_chain_images_count];
	size_t							i;
	VkDescriptorSetAllocateInfo 	allocInfo;

	i = -1;
	while (++i < app->vk_swap_chain_images_count)
		layouts[i] = app->vk_descriptor_set_layout;
	ft_memset(&allocInfo, 0, sizeof(allocInfo));
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = app->vk_descriptor_pool;
	allocInfo.descriptorSetCount = app->vk_swap_chain_images_count;
	allocInfo.pSetLayouts = layouts;
	error_check(vkAllocateDescriptorSets(app->vk_logical_device, &allocInfo,
		app->vk_descriptor_sets) != VK_SUCCESS,
			"Failed to allocate descriptor sets!");
	i = -1;
	while (++i < app->vk_swap_chain_images_count)
		create_descriptor_set(app, i);
}
