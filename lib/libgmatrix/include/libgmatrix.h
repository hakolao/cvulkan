/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libgmatrix.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/17 13:11:01 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/16 20:57:42 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBGMATRIX_H
# define LIBGMATRIX_H

# include <math.h>
# include "libft.h"

typedef enum		e_projection_type
{
	ML_ORTHOGRAPHIC = 1,
	ML_PERSPECTIVE = 2,
}					t_projection_type;

typedef float		t_vec2[2];
typedef float		t_vec3[3];
typedef float		t_vec4[4];
typedef t_vec4		t_mat4[4];

typedef struct		s_canvas
{
	float			width;
	float			height;
	float			near;
	float			far;
	float			fov;
}					t_canvas;

/*
** Vector
*/
void		ml_vector3_add(t_vec3 v1, t_vec3 v2, t_vec3 res);
void		ml_vector3_sub(t_vec3 v1, t_vec3 v2, t_vec3 res);
void		ml_vector3_mul(t_vec3 v1, float k, t_vec3 res);
void		ml_vector3_normalize(t_vec3 v, t_vec3 res);
float		ml_vector3_mag(t_vec3 v);
void		ml_vector3_forward(t_vec3 eye, t_vec3 target, t_vec3 res);
float		ml_vector3_dot(t_vec3 v1, t_vec3 v2);
void		ml_vector3_cross(t_vec3 v1, t_vec3 v2, t_vec3 res);
void		ml_vector3_to_vec4(t_vec3 v1, t_vec4 res);

void		ml_vector4_add(t_vec4 v1, t_vec4 v2, t_vec4 res);
void		ml_vector4_sub(t_vec4 v1, t_vec4 v2, t_vec4 res);
void		ml_vector4_mul(t_vec4 v1, float k, t_vec4 res);
void		ml_vector4_normalize(t_vec4 v, t_vec4 res);
float		ml_vector4_mag(t_vec4 v);
float		ml_vector4_dot(t_vec4 v1, t_vec4 v2);
void		ml_vector4_cross(t_vec4 v1, t_vec4 v2, t_vec4 res);

/*
** Matrix
*/
void		ml_matrix4_id(t_mat4 res);
void		ml_matrix4_inverse(t_mat4 m, t_mat4 res);
void		ml_matrix4_mul_vec3(t_mat4 m, t_vec3 v, t_vec4 res);
void		ml_matrix4_mul_vec4(t_mat4 m, t_vec3 v, t_vec4 res);
void		ml_matrix4_mul(t_mat4 m1, t_mat4 m2, t_mat4 res);
void		ml_matrix4_projection(t_projection_type type,
			t_canvas c, t_mat4 res);
void		ml_matrix4_lookat(t_vec3 eye, t_vec3 center, t_vec3 up, t_mat4 res);
void		ml_matrix4_fps(t_vec3 eye, float pitch, float yaw, t_mat4 res);
void		ml_matrix4_rotation_x(float rad, t_mat4 res);
void		ml_matrix4_rotation_y(float rad, t_mat4 res);
void		ml_matrix4_rotation_z(float rad, t_mat4 res);
void		ml_matrix4_rotation(float rad_x, float rad_y, float rad_z,
			t_mat4 res);
void		ml_matrix4_rotate_x(t_mat4 m, float rad, t_mat4 res);
void		ml_matrix4_rotate_y(t_mat4 m, float rad, t_mat4 res);
void		ml_matrix4_rotate_z(t_mat4 m, float rad, t_mat4 res);

/*
** Utils
*/
float		ml_rad(float angle);
void		ml_matrix4_print(t_mat4 v);
void		ml_vector3_print(t_vec3 v);
void		ml_vector4_print(t_vec3 v);
void		ml_vector4_to_str(t_vec3 v, char *res);
void		ml_matrix4_to_str(t_mat4 m, char *res);
void		ml_vector3_to_str(t_vec3 v, char *res);

#endif
