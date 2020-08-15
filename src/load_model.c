/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_model.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/12 13:07:33 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/15 20:11:55 by ohakola          ###   ########.fr       */
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
	size_t		vi;
	size_t		face_offset;

	i = -1; (void)app;
	face_offset = 0;
	vi = 0;
	while (++i < attrib.num_face_num_verts)
	{
		f = -1;
		while (++f < (size_t)attrib.face_num_verts[i] / 3)
		{
			tinyobj_vertex_index_t idx0 = attrib.faces[face_offset + 3 * f + 0];
			tinyobj_vertex_index_t idx1 = attrib.faces[face_offset + 3 * f + 1];
			tinyobj_vertex_index_t idx2 = attrib.faces[face_offset + 3 * f + 2];
			// for (k = 0; k < 3; k++) {
			int f0 = idx0.v_idx;
			int f1 = idx1.v_idx;
			int f2 = idx2.v_idx;
			app->vertices[vi + 0].pos[0] =
				attrib.vertices[3 * (size_t)f0 + 0];
			app->vertices[vi + 0].pos[1] =
				attrib.vertices[3 * (size_t)f0 + 1];
			app->vertices[vi + 0].pos[2] =
				attrib.vertices[3 * (size_t)f0 + 2];
			app->vertices[vi + 1].pos[0] =
				attrib.vertices[3 * (size_t)f1 + 0];
			app->vertices[vi + 1].pos[1] =
				attrib.vertices[3 * (size_t)f1 + 1];
			app->vertices[vi + 1].pos[2] =
				attrib.vertices[3 * (size_t)f1 + 2];
			app->vertices[vi + 2].pos[0] =
				attrib.vertices[3 * (size_t)f2 + 0];
			app->vertices[vi + 2].pos[1] =
				attrib.vertices[3 * (size_t)f2 + 1];
			app->vertices[vi + 2].pos[2] =
				attrib.vertices[3 * (size_t)f2 + 2];
				vi += 3;
			// }
		}
		face_offset += (size_t)attrib.face_num_verts[i];
	}
	app->num_vertices = vi;
	app->num_indices = app->num_vertices;
	for (size_t i = 0; i < vi; i++)
	{
		ft_printf("%d: %f %f %f\n", i,
			app->vertices[i].pos[0],
			app->vertices[i].pos[1],
			app->vertices[i].pos[2]);
	}

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
