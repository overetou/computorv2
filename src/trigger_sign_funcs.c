/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trigger_sign_funcs.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: overetou <overetou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/18 18:18:55 by overetou          #+#    #+#             */
/*   Updated: 2019/10/22 18:59:35 by overetou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "computor.h"

char	num_store(t_buf *b, void *m)
{
	int	value;
	
	if (((t_master*)m)->prev == VALUE)
	{
		handle_line_error(m, "Two values were consecutively defined.");//This func must read till endline, call prepare new line and putendl the given string.
		return (1);
	}
	value = (((t_master*)m)->prev == MINUS || int_is_comprised(((t_master*)m)->prev, MINUS_MULT, MINUS_MODULO) ? -1 : 1);
	if (!read_int(b, &value))
	{
		handle_line_error(m, "Integer overflow detected.");
		return (1);
	}
	if (!exec_cell_if_prior((t_master*)m, value))
		mix_in_value((t_master*)m, value);
	((t_master*)m)->prev = VALUE;
	return (1);
}

char	star_exec(t_buf *b, void *m)
{
	read_smart_inc(b);
	if (b->str[b->pos] == '*')
		handle_line_error(m, "Forbidden instruction '**'.");
	if (((t_master*)m)->prev != VALUE)
		handle_line_error(m, "Expected a value behind a '*'.");
	else
		((t_master*)m)->prev = MULT;
	return (1);
}

char	div_exec(t_buf *b, void *m)
{
	if (((t_master*)m)->prev != VALUE)
		handle_line_error(m, "No value found behind '/'.");
	((t_master*)m)->prev = DIV;
	(void)b;
	return (1);
}

char	modulo_exec(t_buf *b, void *m)
{
	if (((t_master*)m)->prev != VALUE)
		handle_line_error(m, "No value found behind '%'.");
	((t_master*)m)->prev = MODULO;
	(void)b;
	return (1);
}

char	minus_exec(t_buf *b, void *m)
{
	if (int_is_comprised(((t_master*)m)->prev, PLUS, MODULO))
		((t_master*)m)->prev += 5;
	else if (((t_master*)m)->prev != VALUE)
		handle_line_error(m, "'-' was preceded itself more than one time or was not preceded by a value.");
	else
		((t_master*)m)->prev = MINUS;
	(void)b;
	return (1);
}

char	plus_exec(t_buf *b, void *m)
{
	if (((t_master*)m)->prev != VALUE)
		handle_line_error(m, "'+' was not preceded by a value.");
	else
		((t_master*)m)->prev = PLUS;
	(void)b;
	return (1);
}
