/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_model.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/12 13:07:33 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/12 16:56:44 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cvulkan.h"
# include <tiny_obj_loader_c.h>

static void	get_file_data(const char *filename, char **data, size_t *len)
{
	t_file_contents	*contents;

	error_check(!(contents = read_file(filename)), "Failed to read file!");
	*(len) = contents->size;
	*(data) = contents->buf;
	free_file_contents(contents);
}

void		vulkan_load_model(t_cvulkan *app, const char *filename)
{

	tinyobj_attrib_t		attrib;
	tinyobj_shape_t			*shapes;
	tinyobj_material_t		*materials;
	size_t					num_shapes;
	size_t					num_materials;
	size_t					i;
	size_t					f;

	num_shapes = 0;
	num_materials = 0;
	shapes = NULL;
	materials = NULL;
	error_check(tinyobj_parse_obj(&attrib, &shapes, &num_shapes, &materials,
		&num_materials, filename, get_file_data, TINYOBJ_FLAG_TRIANGULATE),
		"Failed to parse model obj!");
	error_check(!(app->vertices = malloc(sizeof(*app->vertices) * 3 *
		attrib.num_face_num_verts)), "Failed to malloc vertices");
	error_check(!(app->indices = malloc(sizeof(*app->indices) * 3 *
		attrib.num_face_num_verts)), "Failed to malloc indices");
	i = -1;
	while (++i < attrib.num_face_num_verts)
	{
		error_check(attrib.face_num_verts[i] % 3 != 0,
			"Face num verts % 3 != 0");
		f = -1;
		while (++f < (size_t)attrib.face_num_verts[i] / 3)
		{
			app->vertices[i + f].pos[0] = attrib.vertices[3 *
				(size_t)(attrib.faces[i + 3 * f + 0].v_idx) + 0];
			app->vertices[i + f].pos[1] = attrib.vertices[3 *
				(size_t)(attrib.faces[i + 3 * f + 1].v_idx) + 1];
			app->vertices[i + f].pos[2] = attrib.vertices[3 *
				(size_t)(attrib.faces[i + 3 * f + 2].v_idx) + 2];
			app->vertices[i + f].color[0] = 1.0f;
			app->vertices[i + f].color[1] = 1.0f;
			app->vertices[i + f].color[2] = 1.0f;
			app->vertices[i + f].tex_coord[0] = attrib.texcoords[2 *
				(size_t)(attrib.faces[i + 3 * f].vt_idx) + 0];
			app->vertices[i + f].tex_coord[1] = 1.0f - attrib.texcoords[2 *
				(size_t)(attrib.faces[i + 3 * f].vt_idx) + 1];
			app->indices[i + f] = i + f;
		}
	}
	tinyobj_attrib_free(&attrib);
  	tinyobj_shapes_free(shapes, num_shapes);
 	tinyobj_materials_free(materials, num_materials);
}
