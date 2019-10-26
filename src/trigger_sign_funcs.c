/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trigger_sign_funcs.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: overetou <overetou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/18 18:18:55 by overetou          #+#    #+#             */
/*   Updated: 2019/10/26 18:20:44 by overetou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "computor.h"
#include <stdio.h>

char	num_store(t_buf *b, void *m)
{
	int	value;
	
	if (prev(m) == VALUE)
	{
		handle_line_error(m, "Two values were consecutively defined.");//This func must read till endline, call prepare new line and putendl the given string.
		return (1);
	}
	value = (prev(m) == MINUS || int_is_comprised(prev(m), MINUS_PLUS, MINUS_MODULO) ? -1 : 1);
	//printf("NUMSTORE: starter value = %d\n", value);
	if (!read_int(b, &value))
	{
		handle_line_error(m, "Integer overflow detected.");
		return (1);
	}
	//printf("NUMSTORE: final value = %d\n", value);
	inject_value(m, value);
	*(prev_adr(m)) = VALUE;
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
		*(prev_adr(m)) = MULT;
	return (1);
}

char	div_exec(t_buf *b, void *m)
{
	if (prev(m) != VALUE)
		handle_line_error(m, "No value found behind '/'.");
	*(prev_adr(m)) = DIV;
	read_smart_inc(b);
	return (1);
}

char	modulo_exec(t_buf *b, void *m)
{
	if (prev(m) != VALUE)
		handle_line_error(m, "No value found behind '%'.");
	*(prev_adr(m)) = MODULO;
	read_smart_inc(b);
	return (1);
}

char	minus_exec(t_buf *b, void *m)
{
	if (int_is_comprised(prev(m), MINUS, MODULO))
		*(prev_adr(m)) += 5;
	else if (prev(m) == NOTHING || prev(m) == VALUE)
		*(prev_adr(m)) = MINUS;
	else
	{
		print_track_values(m);
		handle_line_error(m, "'-' was not preceded by a compatible element.");
		return (1);
	}
	read_smart_inc(b);
	return (1);
}

char	plus_exec(t_buf *b, void *m)
{
	if (prev(m) != VALUE)
		handle_line_error(m, "'+' was not preceded by a value.");
	else
		*(prev_adr(m)) = PLUS;
	read_smart_inc(b);
	return (1);
}
