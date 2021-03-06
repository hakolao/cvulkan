/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vulkan_surface.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/09 18:34:37 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/15 22:49:06 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cvulkan.h"

void		vulkan_create_surface(t_vkrenderer *app)
{
	error_check(!SDL_Vulkan_CreateSurface(app->window, app->vk_instance,
		&app->vk_surface), "Failed to create a Vulkan surface");
}
