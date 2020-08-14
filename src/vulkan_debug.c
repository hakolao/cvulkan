/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vulkan_debug.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/07 17:35:54 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/14 20:18:07 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cvulkan.h"


static VKAPI_ATTR VkBool32 VKAPI_CALL	debug_callback(
	VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
	VkDebugUtilsMessageTypeFlagsEXT message_type,
	const VkDebugUtilsMessengerCallbackDataEXT *p_callback_data,
	void *p_user_data) {
	(void)message_type;
	(void)p_user_data;
	if (message_severity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)
		ft_dprintf(2, "Validation layer:\n%s\n", p_callback_data->pMessage);
	return (VK_FALSE);
}

static VkResult 						create_debug_utils_messenger_ext(
	VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *p_create_info,
	const VkAllocationCallbacks *p_allocator,
	VkDebugUtilsMessengerEXT *p_debug_messenger) {
	PFN_vkCreateDebugUtilsMessengerEXT func =
		(PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance,
		"vkCreateDebugUtilsMessengerEXT");
	if (func != NULL) {
		return func(instance,
			p_create_info, p_allocator, p_debug_messenger);
	} else {
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

void									vulkan_destroy_debug_utils_messenger_ext(
	VkInstance instance, VkDebugUtilsMessengerEXT p_debug_messenger,
	const VkAllocationCallbacks *p_allocator) {
	PFN_vkDestroyDebugUtilsMessengerEXT func =
		(PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
		instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func != NULL) {
		func(instance, p_debug_messenger, p_allocator);
	}
}

void									error_check(int test,
										const char *message)
{
	if (test)
	{
		ft_printf("%s\n", message);
		exit(EXIT_FAILURE);
	}
}

void									vulkan_populate_debug_messenger_create_info(
										VkDebugUtilsMessengerCreateInfoEXT
										*create_info)
{
	create_info->sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	create_info->messageSeverity =
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	create_info->messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
							 VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
							 VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	create_info->pfnUserCallback = debug_callback;
	create_info->flags = 0;
	create_info->pNext = NULL;
	create_info->pUserData = NULL;
}

void									vulkan_setup_debug_messenger(t_cvulkan
										*app)
{
	VkDebugUtilsMessengerCreateInfoEXT create_info;

	if (!ENABLE_VALIDATION_LAYERS) return;
	vulkan_populate_debug_messenger_create_info(&create_info);
	error_check(create_debug_utils_messenger_ext(app->vk_instance, &create_info,
		NULL, &app->vk_debug_utils_messenger) != VK_SUCCESS,
				"failed to set up debug messenger!");
}
