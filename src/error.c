/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/07 17:35:54 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/07 17:58:17 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cvulkan.h"

void	error_check(int test, const char *message)
{
	if (test)
	{
		ft_printf(message);
		exit(EXIT_FAILURE);
	}
}
