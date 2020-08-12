/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cvulkan.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/07 16:54:33 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/12 23:19:54 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CVULKAN_H

# define FILE_READ_BUF 1024

# define WIDTH 1280
# define HEIGHT 720
# define NAME "CVulkan"

# define EXIT_FAILURE 1
# define EXIT_SUCCESS 0

# define ENABLE_VALIDATION_LAYERS 1

# include <SDL.h>
# include <SDL_image.h>
# include <SDL_vulkan.h>
# include <vulkan/vulkan.h>
# include <math.h>
# include "libft.h"

/*
** Tiny object loader
** ToDo: Use own functions
** See load_model.c
*/
# define TINYOBJ_LOADER_C_IMPLEMENTATION

/*
** cGLM
** ToDo: Don't use cglm, but own library...
*/
# include <cglm/cglm.h>

# define MODEL_PATH "models/viking_room.obj"
# define TEXTURE_PATH "textures/viking_room.png"

typedef struct						s_image_info
{
	uint32_t						width;
	uint32_t						height;
	uint32_t						mip_levels;
	VkSampleCountFlagBits			msaa_samples;
	VkFormat						format;
	VkImageTiling					tiling;
	VkImageUsageFlags				usage;
	VkMemoryPropertyFlags			properties;
	VkImage							*image;
	VkDeviceMemory					*image_memory;
}									t_image_info;

typedef struct						s_buffer_info
{
VkDeviceSize						size;
VkBufferUsageFlags					usage;
VkMemoryPropertyFlags				properties;
VkBuffer							*buffer;
VkDeviceMemory						*buffer_memory;
}									t_buffer_info;

typedef struct						s_vulkan_vertex
{
	vec3				pos;
	vec3				color;
	vec2				tex_coord;
}									t_vulkan_vertex;

typedef struct						s_uniform_buffer_object
{
	mat4				model;
	mat4				view;
	mat4				proj;
}									t_uniform_buffer_object;

typedef struct						s_file_contents
{
	void				*buf;
	uint32_t			size;
}									t_file_contents;

typedef struct						s_queue_family_indices
{
	uint32_t			graphics_family;
	uint32_t			present_family;
}									t_queue_family_indices;

typedef struct						s_swap_chain_support_details {
	VkSurfaceCapabilitiesKHR	capabilities;
	VkSurfaceFormatKHR			formats[64];
	uint32_t					format_count;
	VkPresentModeKHR			present_modes[64];
	uint32_t					present_mode_count;
}									t_swap_chain_support_details;

typedef struct						s_window_info {
	uint32_t		window_id;
	bool			is_hidden;
	void			*parent;
}									t_window_info;

# define MAX_SWAPCHAIN_SIZE 8

typedef struct						s_cvulkan {
	bool						is_running;
	bool						frame_buffer_resized;
	SDL_Window					*window;
	t_window_info				window_info;
	VkInstance					vk_instance;
	VkSurfaceKHR				vk_surface;
	VkPhysicalDevice			vk_physical_device;
	VkDevice					vk_logical_device;
	VkQueue						vk_graphics_queue;
	VkQueue						vk_present_queue;
	VkSwapchainKHR				vk_swap_chain;
	VkImage						vk_swap_chain_images[MAX_SWAPCHAIN_SIZE];
	VkFormat					vk_swap_chain_image_format;
	VkExtent2D					vk_swap_chain_extent;
	VkImageView					vk_swap_chain_image_views[MAX_SWAPCHAIN_SIZE];
	VkFramebuffer				vk_swap_chain_frame_buffers[MAX_SWAPCHAIN_SIZE];
	uint32_t					vk_swap_chain_images_count;
	VkSampleCountFlagBits		vk_msaa_samples;
	uint32_t					vk_enabled_extension_count;
	uint32_t					vk_enabled_layer_count;
	uint32_t					vk_required_extension_count;
	VkDebugUtilsMessengerEXT	vk_debug_utils_messenger;
	char						*vk_extension_names[64];
	char						*vk_enabled_layers[64];
	char						*vk_required_device_extensions[64];
	VkRenderPass				vk_render_pass;
	VkDescriptorSetLayout		vk_descriptor_set_layout;
	VkPipelineLayout			vk_pipeline_layout;
	VkPipeline					vk_graphics_pipeline;
	VkCommandPool				vk_command_pool;
	VkImage						vk_color_image;
	VkDeviceMemory				vk_color_image_memory;
	VkImageView					vk_color_image_view;
	VkImage						vk_depth_image;
	VkDeviceMemory				vk_depth_image_memory;
	VkImageView					vk_depth_image_view;
	uint32_t					vk_mip_levels;
	VkImage						vk_texture_image;
	VkDeviceMemory				vk_texture_image_memory;
	VkImageView					vk_texture_image_view;
	VkSampler					vk_texture_sampler;
	t_vulkan_vertex				*vertices;
	uint32_t					*indices;
	uint32_t					num_vertices;
	uint32_t					num_indices;
	VkBuffer					vk_vertex_buffer;
	VkDeviceMemory				vk_vertex_buffer_memory;
	VkBuffer					vk_index_buffer;
	VkDeviceMemory				vk_index_buffer_memory;
	VkBuffer					vk_uniform_buffers[MAX_SWAPCHAIN_SIZE];
	VkDeviceMemory				vk_uniform_buffers_memory[MAX_SWAPCHAIN_SIZE];
	VkDescriptorPool			vk_descriptor_pool;
	VkDescriptorSet				vk_descriptor_sets[MAX_SWAPCHAIN_SIZE];
	VkCommandBuffer				vk_command_buffers[MAX_SWAPCHAIN_SIZE];
}									t_cvulkan;

/*
** App
*/
void								app_run(t_cvulkan *app);

/*
** File reading
*/
t_file_contents						*read_file(const char *filename);
void								free_file_contents(t_file_contents
									*contents);

/*
** Window
*/
void								window_init(t_cvulkan *app);

/*
** Model loading
*/
void								vulkan_load_model(t_cvulkan *app,
									const char *filename);

/*
** Debugging
*/
void								error_check(int test, const char *message);
void								vulkan_populate_debug_messenger_create_info(
									VkDebugUtilsMessengerCreateInfoEXT
									*create_info);
void								vulkan_setup_debug_messenger(t_cvulkan
									*app);
void								vulkan_destroy_debug_utils_messenger_ext(
									VkInstance instance,
									VkDebugUtilsMessengerEXT p_debug_messenger,
									const VkAllocationCallbacks *p_allocator);

/*
** Vulkan instance
*/
void								vulkan_create_instance(t_cvulkan *app);
void								vulkan_populate_instance_create_info(
									t_cvulkan *app, VkApplicationInfo *app_info,
									VkDebugUtilsMessengerCreateInfoEXT
									*debug_create_info,
									VkInstanceCreateInfo *create_info);

/*
** Vulkan surface
*/
void								vulkan_create_surface(t_cvulkan *app);


/*
** Vulkan device
*/
void								vulkan_query_swap_chain_support(
									t_cvulkan *app,
									VkPhysicalDevice device,
									t_swap_chain_support_details *details);
void								vulkan_pick_physical_device(t_cvulkan *app);
bool								vulkan_check_device_extension_support(
									t_cvulkan *app,
									VkPhysicalDevice device);
VkSampleCountFlagBits				vulkan_get_max_usable_sample_count(t_cvulkan
									*app);
void								vulkan_find_queue_families(t_cvulkan *app,
									VkPhysicalDevice device,
									t_queue_family_indices *indices);

/*
** Vulkan logical device
*/
void								vulkan_create_logical_device(t_cvulkan *app);

/*
** Vulkan swap chain
*/
void								vulkan_create_swap_chain(t_cvulkan *app);
void								vulkan_cleanup_swap_chain(t_cvulkan *app);
void								vulkan_choose_swap_extent(t_cvulkan *app,
									VkSurfaceCapabilitiesKHR *capabilities,
									VkExtent2D *actualExtent);
void								vulkan_choose_swap_present_mode(
									t_swap_chain_support_details *details,
									VkPresentModeKHR *present_mode);
void								vulkan_choose_swap_surface_format(
									t_swap_chain_support_details *details,
									VkSurfaceFormatKHR *format);

/*
** Vulkan image views
*/
void								vulkan_create_image_views(t_cvulkan *app);
VkImageView							vulkan_create_image_view(t_cvulkan *app,
									VkImageViewCreateInfo *view_info);
VkImageViewCreateInfo				vulkan_create_image_image_view_info(
									VkImage image, VkFormat format,
									VkImageAspectFlags aspect_flags,
									uint32_t mip_levels);
void								vulkan_create_texture_image_view(t_cvulkan
									*app);

/*
** Vulkan render pass
*/
void								vulkan_create_render_pass(t_cvulkan *app);
VkFormat							vulkan_find_depth_format(t_cvulkan *app);
VkSubpassDescription				*vulkan_create_subpass_description();
void								vulkan_free_subpass_description(
									VkSubpassDescription *subpass);

/*
** Vulkan descriptor set
*/
void								vulkan_create_descriptor_set_layout(
									t_cvulkan *app);
void								vulkan_create_descriptor_pool(t_cvulkan
									*app);
void								vulkan_create_descriptor_sets(t_cvulkan
									*app);

/*
** Vulkan vertex
*/
void								vulkan_free_vertex_input_create_info(const
									VkPipelineVertexInputStateCreateInfo
									*create_info);
VkPipelineVertexInputStateCreateInfo*vulkan_create_vertex_input_create_info();

/*
** Vulkan graphics pipeline
*/
void								vulkan_create_graphics_pipeline(t_cvulkan
									*app);
VkPipelineColorBlendStateCreateInfo	*vulkan_create_color_blend_create_info();
void								free_color_blend_create_info(const
									VkPipelineColorBlendStateCreateInfo
									*color_blending);
VkPipelineDepthStencilStateCreateInfo
									*vulkan_create_depth_stencil_create_info();
VkPipelineMultisampleStateCreateInfo
									*vulkan_create_multisample_create_info(
									VkSampleCountFlagBits msaa_samples);
VkPipelineRasterizationStateCreateInfo
									*vulkan_create_rasterization_create_info();

VkPipelineViewportStateCreateInfo	*vulkan_create_viewport_create_info(
									t_cvulkan *app);
void								vulkan_free_viewport_create_info(const
									VkPipelineViewportStateCreateInfo
									*create_info);
VkPipelineInputAssemblyStateCreateInfo
									*create_input_assembly_create_info();
VkPipelineShaderStageCreateInfo		*vulkan_create_shader_stages_info(
									t_cvulkan *app,
									uint32_t *stage_count);
void								vulkan_destroy_shader_modules(
									t_cvulkan *app,
									uint32_t stage_count,
									const VkPipelineShaderStageCreateInfo
									*shader_stages);


/*
** Vulkan command pool
*/
void								vulkan_create_command_pool(t_cvulkan *app);

/*
** Vulkan memory
*/
uint32_t							vulkan_find_memory_type(t_cvulkan *app,
									uint32_t type_filter,
									VkMemoryPropertyFlags properties);

/*
** Vulkan image
*/
void								vulkan_create_image(t_cvulkan *app,
									t_image_info *info);
void								vulkan_create_texture_image(t_cvulkan *app);
bool								vulkan_has_stencil_component(VkFormat
									format);
void								vulkan_allocate_image_memory(t_cvulkan *app,
									t_image_info *info);
void								vulkan_copy_buffer_to_image(t_cvulkan *app,
									VkBuffer buffer, t_image_info *info);
void								vulkan_transition_image_layout(t_cvulkan
									*app, t_image_info *info,
									VkImageLayout oldLayout,
									VkImageLayout newLayout);
 void								vulkan_set_barrier_subresource_range(
	 								VkImageLayout new_layout,
									t_image_info *info,
									VkImageMemoryBarrier *barrier);
void								vulkan_set_barrier_layout_transition(
									VkImageLayout layouts[2],
									VkPipelineStageFlags *src_stage,
									VkPipelineStageFlags *dst_stage,
									VkImageMemoryBarrier *barrier);
void								vulkan_generate_mipmaps(t_cvulkan *app,
									t_image_info *info);
void								vulkan_create_blit_image_cmd(int32_t
									dimensions[2],
									VkCommandBuffer command_buffer,
									t_image_info *info, size_t i);
void								vulkan_create_texture_sampler(t_cvulkan
									*app);

/*
** Vulkan color resources
*/
void								vulkan_create_color_resources(t_cvulkan
									*app);

/*
** Vulkan depth resources
*/
void								vulkan_create_depth_resources(t_cvulkan
									*app);

/*
** Vulkan  frame buffers
*/
void								vulkan_create_frame_buffers(t_cvulkan *app);

/*
** Vulkan command buffer
*/
VkCommandBuffer						vulkan_begin_single_time_commands(t_cvulkan
									*app);
void								vulkan_end_single_time_commands(t_cvulkan
									*app, VkCommandBuffer command_buffer);
void								vulkan_create_buffer(t_cvulkan *app,
									t_buffer_info *info);
void								vulkan_copy_buffer(t_cvulkan *app,
									VkBuffer src_buffer, VkBuffer dst_buffer,
									VkDeviceSize size);
void								vulkan_create_command_buffers(t_cvulkan
									*app);

/*
** Vulkan vertex & index buffers
*/
void								vulkan_create_vertex_buffer(t_cvulkan *app);
void								vulkan_create_index_buffer(t_cvulkan *app);

/*
** Vulkan uniform buffers
*/
void								vulkan_create_uniform_buffers(t_cvulkan
									*app);

#endif
