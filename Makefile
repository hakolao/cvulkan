CC = gcc
NAME=cvulkan
DIR_SRC = ./src
DIR_OBJ = temp
LIBVULKAN = ./lib/vulkan
LIBSDL2 = ./lib/SDL2
LIBFT = ./lib/libft

# USE THESE AT SCHOOL
# VULKAN_SDK_PATH = /Users/ohakola/goinfre/vulkan/macOS
# -I$(BREW_INCLUDES)
# -I$(VULKAN_SDK_PATH)/include
# ===

LIBFTFLAGS = -L$(LIBFT) -lft

VULKAN_FLAGS =  -rpath $(LIBVULKAN) \
				-framework vulkan -F$(LIBVULKAN)/

SDL_FLAGS = -rpath $(LIBSDL2) \
				-framework SDL2 -F$(LIBSDL2)/ \
				-framework SDL2_image -F$(LIBSDL2)/ \
				-framework SDL2_ttf -F$(LIBSDL2)/

INCL = -I ./include \
		-I$(LIBFT)/incl \
		-I$(LIBVULKAN)/vulkan.framework/Headers \
		-I$(LIBSDL2)/SDL2.framework/Headers \
		-I$(LIBSDL2)/SDL2_image.framework/Headers \
		-I$(LIBSDL2)/SDL2_ttf.framework/Headers

FLAGS = -Wall -Wextra -Werror -O3
SOURCES = main.c \
			app.c \
			window.c \
			read_file.c \
			vulkan_debug.c \
			vulkan_device.c \
			vulkan_device_utils.c \
			vulkan_surface.c \
			vulkan_instance.c \
			vulkan_instance_info.c \
			vulkan_logical_device.c \
			vulkan_swap_chain.c \
			vulkan_swap_chain_utils.c \
			vulkan_image_views.c \
			vulkan_render_pass.c \
			vulkan_subpass.c \
			vulkan_render_pass_utils.c \
			vulkan_descriptor_set_layout.c \
			vulkan_graphics_pipeline.c \
			vulkan_graphics_pipeline_info1.c \
			vulkan_graphics_pipeline_info2.c \
			vulkan_graphics_pipeline_info3.c \
			vulkan_vertex.c \
			vulkan_command_pool.c \
			vulkan_color_resources.c \
			vulkan_depth_resources.c \
			vulkan_image.c \
			vulkan_memory.c \
			vulkan_frame_buffer.c \
			vulkan_command_buffer.c

SRCS = $(addprefix $(DIR_SRC)/,$(SOURCES))
OBJS = $(addprefix $(DIR_OBJ)/,$(SOURCES:.c=.o))

all: $(DIR_OBJ) $(NAME)

$(NAME): $(OBJS)
	@printf "\033[32;1mCompiling libs...\n\033[0m"
	make -C $(LIBFT)
	@printf "\033[32;1mCompiling shaders...\n\033[0m"
	cd shaders && /bin/sh compile.sh && cd ..
	@printf "\033[32;1mCompiling app...\n\033[0m"
	$(CC) $(FLAGS) $(LIBFTFLAGS) $(VULKAN_FLAGS) $(SDL_FLAGS) -o $@ $^
	@printf "\033[32;1mDone. Run: ./$(NAME)\n\033[0m"

$(DIR_OBJ):
	@mkdir -p temp

$(DIR_OBJ)/%.o: $(DIR_SRC)/%.c
	@$(CC) $(FLAGS) $(INCL) -c -o $@ $<

clean:
	@make -C $(LIBFT) clean
	@/bin/rm -f $(OBJS)
	@/bin/rm -rf $(DIR_OBJ)
	@/bin/rm -f shaders/*.spv

fclean: clean
	@make -C $(LIBFT) fclean
	@/bin/rm -f $(NAME)

re: fclean all

norm: norminette $(DIR_SRC) ./include

.PHONY: all, $(DIR_OBJ), clean, fclean
