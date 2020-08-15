/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_model.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/12 13:07:33 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/15 20:46:58 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cvulkan.h"
# include <tiny_obj_loader_c.h>

static void		get_file_data(const char *filename, char **data, size_t *len)
{
	t_file_contents	*contents;

	error_check(!(contents = read_file(filename)), "Failed to read file!");
	*(len) = contents->size;
	*(data) = contents->buf;
	free_file_contents(contents);
}

static void		set_face_vertices_and_indices(t_cvulkan *app,
				tinyobj_attrib_t attrib)
{
	size_t		i;
	size_t		f;
	size_t		k;
	size_t		vi;
	size_t		face_offset;

	i = -1;
	face_offset = 0;
	vi = 0;
	while (++i < attrib.num_face_num_verts)
	{
		f = -1;
		while (++f < (size_t)attrib.face_num_verts[i] / 3)
		{
			k = -1;
			while (++k < 3)
			{
				app->vertices[vi + k].pos[0] = attrib.vertices[3 *
					(size_t)attrib.faces[face_offset + 3 * f + k].v_idx + 0];
				app->vertices[vi + k].pos[1] = attrib.vertices[3 *
					(size_t)attrib.faces[face_offset + 3 * f + k].v_idx + 1];
				app->vertices[vi + k].pos[2] = attrib.vertices[3 *
					(size_t)attrib.faces[face_offset + 3 * f + k].v_idx + 2];
				app->vertices[vi + k].color[0] = 1.0f;
				app->vertices[vi + k].color[1] = 1.0f;
				app->vertices[vi + k].color[2] = 1.0f;
				app->vertices[vi + k].tex_coord[0] = attrib.texcoords[2 *
					(size_t)attrib.faces[face_offset + 3 * f + k].vt_idx + 0];
				app->vertices[vi + k].tex_coord[1] = 1.0f - attrib.texcoords[2 *
					(size_t)attrib.faces[face_offset + 3 * f + k].vt_idx + 1];
				app->indices[vi + k] = vi + k;
			}
			vi += 3;
		}
		face_offset += (size_t)attrib.face_num_verts[i];
	}
	app->num_vertices = vi;
	app->num_indices = app->num_vertices;
}

/*
** ToDo: Vertex dedup
*/

void			vulkan_load_model(t_cvulkan *app, const char *filename)
{

	tinyobj_attrib_t		attrib;
	tinyobj_shape_t			*shapes;
	tinyobj_material_t		*materials;
	size_t					num_shapes;
	size_t					num_materials;

	num_shapes = 0;
	num_materials = 0;
	shapes = NULL;
	materials = NULL;
	error_check(tinyobj_parse_obj(&attrib, &shapes, &num_shapes, &materials,
		&num_materials, filename, get_file_data, TINYOBJ_FLAG_TRIANGULATE),
		"Failed to parse model obj!");
	error_check(!(app->vertices = malloc(sizeof(*app->vertices) *
		attrib.num_face_num_verts * 3)), "Failed to malloc vertices");
	error_check(!(app->indices = malloc(sizeof(*app->indices) * 3 *
		attrib.num_face_num_verts * 3)), "Failed to malloc indices");
	set_face_vertices_and_indices(app, attrib);
	ft_printf("vertices: %d\n", app->num_vertices);
	ft_printf("num_indices: %d\n", app->num_indices);
	tinyobj_attrib_free(&attrib);
  	tinyobj_shapes_free(shapes, num_shapes);
 	tinyobj_materials_free(materials, num_materials);
}
