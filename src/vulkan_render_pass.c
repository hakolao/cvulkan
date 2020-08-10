/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vulkan_render_pass.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/10 12:12:47 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/10 12:36:43 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cvulkan.h"

static VkFormat	find_supported_format(t_cvulkan *app,
	VkFormat *candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
{
	size_t				i;
	VkFormatProperties	props;

	i = 0;
	while (candidates[i] != (VkFormat)-1)
	{
		vkGetPhysicalDeviceFormatProperties(app->vk_physical_device,
			candidates[i], &props);
		if (tiling == VK_IMAGE_TILING_LINEAR &&
			(props.linearTilingFeatures & features) == features) {
			return (candidates[i]);
		} else if (tiling == VK_IMAGE_TILING_OPTIMAL &&
				   (props.optimalTilingFeatures & features) == features) {
			return (candidates[i]);
		}
		i++;
	}
	error_check(true, "Failed to find supported format!");
	return (VK_FORMAT_UNDEFINED);
}

static VkFormat	find_depth_format(t_cvulkan *app)
{
	return (find_supported_format(
		app, (VkFormat[4]){VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT,
		 VK_FORMAT_D24_UNORM_S8_UINT, -1}, VK_IMAGE_TILING_OPTIMAL,
		VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT));
}

void			vulkan_create_render_pass(t_cvulkan *app)
{
	VkAttachmentDescription		colorAttachment;
	VkAttachmentDescription		colorAttachmentResolve;
	VkAttachmentReference		colorAttachmentRef;
	VkAttachmentReference		colorAttachmentResolveRef;
	VkAttachmentDescription		depthAttachment;
	VkAttachmentReference		depthAttachmentRef;
	VkSubpassDescription		subpass;
	VkSubpassDependency			dependency;
	VkRenderPassCreateInfo		renderPassInfo;

	ft_memset(&colorAttachment, 0, sizeof(colorAttachment));
	colorAttachment.format = app->vk_swap_chain_image_format;
	colorAttachment.samples = app->vk_msaa_samples;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	ft_memset(&colorAttachmentResolve, 0, sizeof(colorAttachmentResolve));
	colorAttachmentResolve.format = app->vk_swap_chain_image_format;
	colorAttachmentResolve.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachmentResolve.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachmentResolve.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachmentResolve.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachmentResolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachmentResolve.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachmentResolve.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	ft_memset(&colorAttachmentRef, 0, sizeof(colorAttachmentRef));
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	ft_memset(&colorAttachmentResolveRef, 0, sizeof(colorAttachmentResolveRef));
	colorAttachmentResolveRef.attachment = 2;
	colorAttachmentResolveRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	ft_memset(&depthAttachment, 0, sizeof(depthAttachment));
	depthAttachment.format = find_depth_format(app);
	depthAttachment.samples = app->vk_msaa_samples;
	depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	depthAttachment.finalLayout =
		VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	ft_memset(&depthAttachmentRef, 0, sizeof(depthAttachmentRef));
	depthAttachmentRef.attachment = 1;
	depthAttachmentRef.layout =
		VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	ft_memset(&subpass, 0, sizeof(subpass));
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;
	subpass.pDepthStencilAttachment = &depthAttachmentRef;
	subpass.pResolveAttachments = &colorAttachmentResolveRef;

	ft_memset(&dependency, 0, sizeof(dependency));
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	ft_memset(&renderPassInfo, 0, sizeof(renderPassInfo));
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = 3;
	renderPassInfo.pAttachments = (VkAttachmentDescription[3]){colorAttachment,
		depthAttachment, colorAttachmentResolve};
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = 1;
	renderPassInfo.pDependencies = &dependency;

	error_check(vkCreateRenderPass(app->vk_logical_device,
		&renderPassInfo, NULL, &app->vk_render_pass) != VK_SUCCESS,
				"Failed to create render pass!");
}
