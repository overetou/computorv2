/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trigger_sign_funcs.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: overetou <overetou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/18 18:18:55 by overetou          #+#    #+#             */
/*   Updated: 2019/11/16 19:10:26 by overetou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "computor.h"
#include <stdio.h>

BOOL	fetch_power_data(t_buf *b, int *power)
{
	read_till_false(b, is_sep);
	if (is_digit(b->str[b->pos]))
		return (read_int(b, power));
	return (0);
}

BOOL	apply_power(t_content *c, char *info, int power)
{
	if (*info == RATIONNAL)
		return (float_simple_power(&(c->flt), power));
	return (0);
}

BOOL	get_power(t_buf *b, t_content *c, char *info)
{
	int	power;

	read_till_false(b, is_sep);
	if (b->str[b->pos] == '^')
	{
		read_smart_inc(b);
		power = 1;
		if (fetch_power_data(b, &power) == 0)
			return (0);
		//printf("get_power: power = %d\n", power);
		return (apply_power(c, info, power));
	}
	return (1);
}

char	num_store(t_buf *b, void *m)
{
	t_content	value;
	char		info;
	
	if (prev(m) == VALUE)
	{
		handle_line_error(m, "Two values were consecutively defined.");
		return (1);
	}
	info = RATIONNAL;
	value.flt = (prev(m) == MINUS || int_is_comprised(prev(m), MINUS_PLUS, MINUS_MODULO) ? -1 : 1);
	if (!read_float(b, &(value.flt)))
	{
		handle_line_error(m, "Problem while parsing a number.");
		return (1);
	}
	if (get_power(b, &value, &info) == 0)
	{
		handle_line_error(m, "Problem while parsing a power detected.");
		return (1);
	}
	printf("num_store: value read = %f\n", value.flt);
	if (((t_master*)m)->equal_defined == DEFINE_FUNC)
	{
		mix_in_value(m, value, info);
		*(prev_adr(m)) = VALUE;
	}
	else
		inject_value(m, value, info);
	return (1);
}

char	star_exec(t_buf *b, void *m)
{
	read_smart_inc(b);
	if (b->str[b->pos] == '*')
		handle_line_error(m, "Forbidden instruction '**'.");
	if (prev(m) != VALUE)
		handle_line_error(m, "Expected a value before a '*'.");
	else
	{
		*(prev_adr(m)) = MULT;
		if (((t_master*)m)->equal_defined == DEFINE_FUNC)
		{
			putendl("mult added in the function definition.");
			mix_in_value(m, (t_content)NULL, MULT);
		}
	}
	return (1);
}

char	div_exec(t_buf *b, void *m)
{
	if (prev(m) != VALUE)
	{
		handle_line_error(m, "No value found behind '/'.");
		return (1);
	}
	*(prev_adr(m)) = DIV;
	read_smart_inc(b);
	if (((t_master*)m)->equal_defined == DEFINE_FUNC)
		mix_in_value(m, (t_content)NULL, DIV);
	return (1);
}

char	modulo_exec(t_buf *b, void *m)
{
	if (prev(m) != VALUE)
	{
		handle_line_error(m, "No value found behind '%'.");
		return (1);
	}
	*(prev_adr(m)) = MODULO;
	read_smart_inc(b);
	if (((t_master*)m)->equal_defined == DEFINE_FUNC)
		mix_in_value(m, (t_content)NULL, MODULO);
	return (1);
}

char	power_exec(t_buf *b, void *m)
{
	(void)b;
	handle_line_error(m, "A '^' was found in a strange place.");
	if (((t_master*)m)->equal_defined == DEFINE_FUNC)
		mix_in_value(m, (t_content)NULL, POWER);
	return (1);
}

char	minus_exec(t_buf *b, void *m)
{
	if (int_is_comprised(prev(m), MINUS, MODULO))
		*(prev_adr(m)) += 5;
	else if (prev(m) == NOTHING || prev(m) == VALUE)
	{
		//putendl("prev = minus.");
		*(prev_adr(m)) = MINUS;
	}
	else
	{
		handle_line_error(m, "'-' was not preceded by a compatible element.");
		return (1);
	}
	read_smart_inc(b);
	if (((t_master*)m)->equal_defined == DEFINE_FUNC)
		mix_in_value(m, (t_content)NULL, MINUS);
	return (1);
}

char	plus_exec(t_buf *b, void *m)
{
	if (prev(m) != VALUE)
	{
		handle_line_error(m, "'+' was not preceded by a value.");
		return (1);
	}
	else
		*(prev_adr(m)) = PLUS;
	read_smart_inc(b);
	if (((t_master*)m)->equal_defined == DEFINE_FUNC)
		mix_in_value(m, (t_content)NULL, PLUS);
	return (1);
}
