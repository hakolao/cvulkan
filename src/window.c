/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/07 17:32:05 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/07 17:58:01 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cvulkan.h"

static int	frame_buffer_resize_callback(void *data, SDL_Event *event)
{
	if (event->type == SDL_WINDOWEVENT &&
		(event->window.event == SDL_WINDOWEVENT_RESIZED ||
		 event->window.event == SDL_WINDOWEVENT_MINIMIZED ||
		 event->window.event == SDL_WINDOWEVENT_MAXIMIZED ||
		 event->window.event == SDL_WINDOWEVENT_SHOWN ||
		 event->window.event == SDL_WINDOWEVENT_HIDDEN))
		{
		t_window_info *window_info = (t_window_info*)data;
		if (event->window.windowID == window_info->window_id)
		{
			t_cvulkan *app = (t_cvulkan*)(window_info->parent);
			app->frame_buffer_resized = true;
			if (event->window.event == SDL_WINDOWEVENT_HIDDEN)
				window_info->is_hidden = true;
			else if (event->window.event == SDL_WINDOWEVENT_SHOWN)
				window_info->is_hidden = false;
		}
	}
	return 0;
}

void		window_init(t_cvulkan *app)
{
	app->window = SDL_CreateWindow(NAME, SDL_WINDOWPOS_CENTERED,
							  SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT,
							  SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);
	error_check(app->window == NULL, SDL_GetError());
	app->window_info.window_id = SDL_GetWindowID(app->window);
	app->window_info.parent = app->window;
	app->window_info.is_hidden = false;
	SDL_AddEventWatch(frame_buffer_resize_callback, &app->window_info);
}
