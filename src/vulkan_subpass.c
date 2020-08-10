/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vulkan_subpass.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/10 12:56:29 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/10 13:34:09 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cvulkan.h"

static void					set_to_zero(VkSubpassDescription *subpass,
							VkAttachmentReference *color_attachment_ref,
							VkAttachmentReference *color_attachment_resolve_ref,
							VkAttachmentReference *depth_attachment_ref)
{
	ft_memset(color_attachment_ref, 0, sizeof(*color_attachment_ref));
	ft_memset(color_attachment_resolve_ref, 0,
		sizeof(*color_attachment_resolve_ref));
	ft_memset(depth_attachment_ref, 0, sizeof(*depth_attachment_ref));
	ft_memset(subpass, 0, sizeof(*subpass));
}

static void					set_attachment_info(
							VkAttachmentReference *color_attachment_ref,
							VkAttachmentReference *color_attachment_resolve_ref,
							VkAttachmentReference *depth_attachment_ref)
{
	color_attachment_ref->attachment = 0;
	color_attachment_ref->layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	color_attachment_resolve_ref->attachment = 2;
	color_attachment_resolve_ref->layout =
		VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	depth_attachment_ref->attachment = 1;
	depth_attachment_ref->layout =
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
	VkAttachmentReference		*color_attachment_ref;
	VkAttachmentReference		*color_attachment_resolve_ref;
	VkAttachmentReference		*depth_attachment_ref;

	error_check(!(subpass = malloc(sizeof(*subpass))), "Failed to malloc");
	error_check(!(color_attachment_ref = malloc(sizeof(*color_attachment_ref))),
		"Failed to malloc");
	error_check(!(color_attachment_resolve_ref =
		malloc(sizeof(*color_attachment_resolve_ref))),"Failed to malloc");
	error_check(!(depth_attachment_ref = malloc(sizeof(*depth_attachment_ref))),
		"Failed to malloc");
	set_to_zero(subpass, color_attachment_ref,
		color_attachment_resolve_ref, depth_attachment_ref);
	set_attachment_info(color_attachment_ref, color_attachment_resolve_ref,
		depth_attachment_ref);
	subpass->pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass->colorAttachmentCount = 1;
	subpass->pColorAttachments = color_attachment_ref;
	subpass->pDepthStencilAttachment = depth_attachment_ref;
	subpass->pResolveAttachments = color_attachment_resolve_ref;
	return (subpass);
}
