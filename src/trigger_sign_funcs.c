/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trigger_sign_funcs.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: overetou <overetou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/18 18:18:55 by overetou          #+#    #+#             */
/*   Updated: 2019/10/19 16:16:43 by overetou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "computor.h"

char	num_store(t_buf *b, t_master *m)
{
	int	value;
	
	if (m->prev == VALUE)
	{
		handle_line_error(m, "Two values were consecutively defined.");//This func must read till endline, call prepare new line and putendl the given string.
		return (1);
	}
	value = (m->prev == MINUS || m->prev = MULT_MINUS ? -1 : 1);
	if (!read_int(b, &value))
	{
		handle_line_error(m, "Integer overflow detected.");
		return (1);
	}
	if (!exec_cell_if_prior(m->prev_expr, m->prev))
		track_add(m->exec_tracks, t_simple_init((void*)value));
	m->prev = VALUE;
	return (1);
}

char	star_exec(t_buf *b, t_master *m)
{
	read_smart_inc(b);
	if (b->str[b->pos] == "*")
		handle_line_error(m, "Forbidden instruction '**'.");
	if (m->prev != VALUE)
		handle_line_error(m, "Expected a value behind a '*'.");
	else
		m->prev = MULT;
	return (1);
}

char	div_exec(t_buf *b, t_master *m)
{
	if (m->prev != VALUE)
		handle_line_error(m, "No value found behind '/'.");
	m->prev = DIV;
	return (1);
}

char	modulo_exec(t_buf *b, t_master *m)
{
	if (m->prev != VALUE)
		handle_line_error(m, "No value found behind '%'.");
	m->prev = MODULO;
	return (1);
}

char	minus_exec(t_buf *b, t_master *m)
{
	if (m->prev est dans l'interval des operations)
		m->prev += augmentation vers une operation suivie d'un moins.
	else if (m->prev != VALUE)
		handle_line_error(m, "'-' was preceded itself more than one time or was not preceded by a value.");
	else
		m->prev = MINUS;
	return (1);
}

char	plus_exec(t_buf *b, t_master *m)
{
	if (m->prev != VALUE)
		handle_line_error(m, "'+' was not preceded by a value.");
	else
		m->prev = PLUS;
	return (1);
}
