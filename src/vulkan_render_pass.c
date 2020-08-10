/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vulkan_render_pass.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/10 12:12:47 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/10 13:22:45 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cvulkan.h"

static VkSubpassDependency		subpass_dependency()
{
	VkSubpassDependency	dependency;

	ft_memset(&dependency, 0, sizeof(dependency));
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	return (dependency);
}

static VkAttachmentDescription	depth_attachment_description(t_cvulkan *app)
{
	VkAttachmentDescription		depthAttachment;
	ft_memset(&depthAttachment, 0, sizeof(depthAttachment));
	depthAttachment.format = vulkan_find_depth_format(app);
	depthAttachment.samples = app->vk_msaa_samples;
	depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	depthAttachment.finalLayout =
		VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	return (depthAttachment);
}

static VkAttachmentDescription	color_attachment_description(t_cvulkan *app)
{
	VkAttachmentDescription		colorAttachment;

	ft_memset(&colorAttachment, 0, sizeof(colorAttachment));
	colorAttachment.format = app->vk_swap_chain_image_format;
	colorAttachment.samples = app->vk_msaa_samples;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	return (colorAttachment);
}

static VkAttachmentDescription	color_attachment_resolve(t_cvulkan *app)
{
	VkAttachmentDescription		colorAttachmentResolve;

	ft_memset(&colorAttachmentResolve, 0, sizeof(colorAttachmentResolve));
	colorAttachmentResolve.format = app->vk_swap_chain_image_format;
	colorAttachmentResolve.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachmentResolve.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachmentResolve.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachmentResolve.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachmentResolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachmentResolve.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachmentResolve.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	return (colorAttachmentResolve);
}

void							vulkan_create_render_pass(t_cvulkan *app)
{
	VkAttachmentDescription		colorAttachment;
	VkAttachmentDescription		colorAttachmentResolve;
	VkSubpassDependency			dependency;
	VkRenderPassCreateInfo		renderPassInfo;
	VkSubpassDescription		*subpass;

	colorAttachment = color_attachment_description(app);
	colorAttachmentResolve = color_attachment_resolve(app);
	subpass = vulkan_create_subpass_description();
	ft_memset(&renderPassInfo, 0, sizeof(renderPassInfo));
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = 3;
	renderPassInfo.pAttachments = (VkAttachmentDescription[3]){colorAttachment,
		depth_attachment_description(app), colorAttachmentResolve};
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = subpass;
	renderPassInfo.dependencyCount = 1;
	dependency = subpass_dependency();
	renderPassInfo.pDependencies = &dependency;
	error_check(vkCreateRenderPass(app->vk_logical_device,
		&renderPassInfo, NULL, &app->vk_render_pass) != VK_SUCCESS,
				"Failed to create render pass!");
	vulkan_free_subpass_description(subpass);
}
