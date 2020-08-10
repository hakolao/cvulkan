/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vulkan_subpass.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/10 12:56:29 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/10 13:24:54 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cvulkan.h"

static void					set_to_zero(VkSubpassDescription *subpass,
							VkAttachmentReference *colorAttachmentRef,
							VkAttachmentReference *colorAttachmentResolveRef,
							VkAttachmentReference *depthAttachmentRef)
{
	ft_memset(colorAttachmentRef, 0, sizeof(*colorAttachmentRef));
	ft_memset(colorAttachmentResolveRef, 0, sizeof(*colorAttachmentResolveRef));
	ft_memset(depthAttachmentRef, 0, sizeof(*depthAttachmentRef));
	ft_memset(subpass, 0, sizeof(*subpass));
}

static void					set_attachment_info(
							VkAttachmentReference *colorAttachmentRef,
							VkAttachmentReference *colorAttachmentResolveRef,
							VkAttachmentReference *depthAttachmentRef)
{
	colorAttachmentRef->attachment = 0;
	colorAttachmentRef->layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	colorAttachmentResolveRef->attachment = 2;
	colorAttachmentResolveRef->layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	depthAttachmentRef->attachment = 1;
	depthAttachmentRef->layout =
		VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
}

void						vulkan_free_subpass_description(
							VkSubpassDescription *subpass)
{
	free((void*)subpass->pColorAttachments);
	free((void*)subpass->pDepthStencilAttachment);
	free((void*)subpass->pResolveAttachments);
	free((void*)subpass);
}

VkSubpassDescription		*vulkan_create_subpass_description()
{
	VkSubpassDescription		*subpass;
	VkAttachmentReference		*colorAttachmentRef;
	VkAttachmentReference		*colorAttachmentResolveRef;
	VkAttachmentReference		*depthAttachmentRef;

	error_check(!(subpass = malloc(sizeof(*subpass))), "Failed to malloc");
	error_check(!(colorAttachmentRef = malloc(sizeof(*colorAttachmentRef))),
		"Failed to malloc");
	error_check(!(colorAttachmentResolveRef =
		malloc(sizeof(*colorAttachmentResolveRef))),"Failed to malloc");
	error_check(!(depthAttachmentRef = malloc(sizeof(*depthAttachmentRef))),
		"Failed to malloc");
	set_to_zero(subpass, colorAttachmentRef,
		colorAttachmentResolveRef, depthAttachmentRef);
	set_attachment_info(colorAttachmentRef, colorAttachmentResolveRef,
		depthAttachmentRef);
	subpass->pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass->colorAttachmentCount = 1;
	subpass->pColorAttachments = colorAttachmentRef;
	subpass->pDepthStencilAttachment = depthAttachmentRef;
	subpass->pResolveAttachments = colorAttachmentResolveRef;
	return (subpass);
}
