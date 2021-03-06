/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   format_precision.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/10 13:50:00 by ohakola           #+#    #+#             */
/*   Updated: 2020/06/25 17:02:27 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static char				*handle_zerox(t_printf *data, char *res)
{
	if (data->c == 'o')
	{
		if (res[0] == '0')
			return (res);
		res = add_str_to_beg(res, "0", data->var_len, 1);
		data->var_len += 1;
	}
	else if (data->c == 'x' || data->c == 'X' || data->c == 'p')
	{
		if (data->is_zero_res)
			return (res);
		res = add_str_to_beg(res, "0x", data->var_len, 2);
		data->var_len += 2;
	}
	else if (data->c == 'f' && data->precision == 0)
	{
		if (!(res = extend_str(res, data->var_len, 1)))
			return (NULL);
		res[data->var_len] = '.';
		data->var_len += 1;
	}
	return (res);
}

char					*handle_int_precision(t_printf *data, char *res)
{
	int		len;
	int		add_size;
	int		new_var_len;

	len = data->var_len;
	if (data->has_precision && data->precision > 0)
	{
		data->pad_zeros = FALSE;
		new_var_len = data->is_negative || data->show_sign ?
				data->precision + 1 : data->precision;
		if (new_var_len > len)
		{
			data->var_len = new_var_len;
			add_size = data->var_len - len;
			if (!(res = extend_str(res, len, add_size)))
				return (NULL);
			res = add_chars_to_str_begin(res, len, data->var_len, '0');
			swap_sign_after_precision(data, res, add_size);
		}
	}
	return (res);
}

char					*handle_number_precision(t_printf *data, char *res)
{
	if (data->show_sign && !data->is_negative && data->c != 'b')
	{
		res = add_str_to_beg(res, "+", 1, data->var_len);
		data->var_len += 1;
	}
	if ((is_int_specifier(data->c) || data->c == 'p') && data->c != 'b')
		res = handle_int_precision(data, res);
	if (data->c == 'b' && !data->is_zero_res)
	{
		res = add_str_to_beg(res,
			data->is_negative ? "1" : "0", 1, data->var_len);
		data->var_len += 1;
	}
	if (data->blank_space)
		res = handle_blank(data, res);
	if (data->zerox || data->c == 'p')
		res = handle_zerox(data, res);
	if (data->c == 'X')
		ft_capitalize(res);
	return (res);
}

char					*handle_string_precision(t_printf *data, char *res)
{
	if (data->has_precision && res[0] && data->precision >= 0)
		data->var_len =
			data->var_len > data->precision ? data->precision : data->var_len;
	return (res);
}
