/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cvulkan.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/07 16:54:33 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/07 17:58:21 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CVULKAN_H

#define WIDTH 1280
#define HEIGHT 720
#define NAME "CVulkan"

# define EXIT_FAILURE 1
# define EXIT_SUCCESS 0

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_vulkan.h>
#include <vulkan/vulkan.h>
# include "libft.h"

typedef struct		s_window_info {
	uint32_t		window_id;
	bool			is_hidden;
	void			*parent;
}					t_window_info;

typedef struct		s_cvulkan {
	bool			is_running;
	bool			frame_buffer_resized;
	SDL_Window		*window;
	t_window_info	window_info;
}					t_cvulkan;

/*
** Error checking
*/
void				error_check(int test, const char *message);

/*
** App
*/
void				app_run(t_cvulkan *app);

/*
** Window
*/
void				window_init(t_cvulkan *app);

#endif
