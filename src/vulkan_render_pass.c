/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vulkan_render_pass.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/10 12:12:47 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/10 16:31:50 by ohakola          ###   ########.fr       */
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
	VkAttachmentDescription		depth_attachment;
	ft_memset(&depth_attachment, 0, sizeof(depth_attachment));
	depth_attachment.format = vulkan_find_depth_format(app);
	depth_attachment.samples = app->vk_msaa_samples;
	depth_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depth_attachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depth_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	depth_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depth_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	depth_attachment.finalLayout =
		VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	return (depth_attachment);
}

static VkAttachmentDescription	color_attachment_description(t_cvulkan *app)
{
	VkAttachmentDescription		color_attachment;

	ft_memset(&color_attachment, 0, sizeof(color_attachment));
	color_attachment.format = app->vk_swap_chain_image_format;
	color_attachment.samples = app->vk_msaa_samples;
	color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	color_attachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	return (color_attachment);
}

static VkAttachmentDescription	color_attachment_resolve_description(t_cvulkan
								*app)
{
	VkAttachmentDescription		color_attachment_resolve;

	ft_memset(&color_attachment_resolve, 0, sizeof(color_attachment_resolve));
	color_attachment_resolve.format = app->vk_swap_chain_image_format;
	color_attachment_resolve.samples = VK_SAMPLE_COUNT_1_BIT;
	color_attachment_resolve.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	color_attachment_resolve.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	color_attachment_resolve.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	color_attachment_resolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	color_attachment_resolve.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	color_attachment_resolve.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	return (color_attachment_resolve);
}

void							vulkan_create_render_pass(t_cvulkan *app)
{
	VkAttachmentDescription		color_attachment;
	VkAttachmentDescription		color_attachment_resolve;
	VkSubpassDependency			dependency;
	VkRenderPassCreateInfo		render_pass_info;
	VkSubpassDescription		*subpass;

	color_attachment = color_attachment_description(app);
	color_attachment_resolve = color_attachment_resolve_description(app);
	subpass = vulkan_create_subpass_description();
	ft_memset(&render_pass_info, 0, sizeof(render_pass_info));
	render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	render_pass_info.attachmentCount = 3;
	render_pass_info.pAttachments = (VkAttachmentDescription[3]){
		color_attachment, depth_attachment_description(app),
		color_attachment_resolve};
	render_pass_info.subpassCount = 1;
	render_pass_info.pSubpasses = subpass;
	render_pass_info.dependencyCount = 1;
	dependency = subpass_dependency();
	render_pass_info.pDependencies = &dependency;
	error_check(vkCreateRenderPass(app->vk_logical_device,
		&render_pass_info, NULL, &app->vk_render_pass) != VK_SUCCESS,
				"Failed to create render pass!");
	vulkan_free_subpass_description(subpass);
}
