/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   functions.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: overetou <overetou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/11 18:22:15 by overetou          #+#    #+#             */
/*   Updated: 2019/11/13 19:57:09 by overetou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "computor.h"
#include <stdio.h>

BOOL	is_definition(t_master *m, t_buf *b)
{
	if (m->equal_defined == 0)
	{
		read_till_false(b, is_sep);
		if (b->str[b->pos] == '=') 
			return (1);
	}
	return (0);
}

t_expr	*get_argument(t_master *m, t_buf *b)
{
	t_expr	*e;

	read_till_false(b, is_sep);
	if (int_is_comprised(b->str[b->pos], '0', '9') || b->str[b->pos] == '-')
	{
		e = int_expr_from_buf;
		if (b->str[b->pos] == ')')
			return (e);
	}
	else if (is_alpha(b->str[b->pos]))
	{

		e = get_variable;
		if (b->str[b->pos] == ')')
			return (e);
	}
	return (NULL);
}

t_expr	*extract_track_and_push_it(t_master *m, char info)
{
	t_expr result;

	result.content.expr = refine_addition_result(m);
	result.info = info;
	...
}

t_expr	*compute_func(t_master *m, t_expr *argument, char *func_name)
{
	t_track *func;
	t_expr	*cur;
	t_expr	*next;

	func = get_func(m, func_name);
	cur = func->first;
	add_level(m);
	while ((t_link*)cur != func->last)
	{
		next = cur->next;
		inject_expr(cur);
		cur = next;
		if ((t_link*)cur != func->last)
		{
			(*prev_adr(m)) = e->info;
			next = cur->next;
			//free_expr(cur);
			cur = next;
		}
	}
	return (extract_func_result(m));
}

char	handle_func(t_master *m, t_buf *b, char *s)
{
	t_expr	*e;

	if (is_definition(m, b))
		return (prepare_func_definition(m, b, s));
	else
	{
		e = compute_func(m, get_argument(m, b));
		if (e)
			inject_expr(m, e);
		else
		{
			handle_line_error(m, "Problem detected while computing a function.");
			return (0);
		}
	}
	return (1);
}

char	prepare_func_definition(t_master *m, t_buf *b, char *s)
{
	read_till_false(b, is_sep);
	if (b->str[b->pos] == ')')
		m->func_var_name = NULL;
	else
	{
		if (!char_is_valid_var_name_material(b->str[b->pos]))
			return (0);
		m->func_var_name = read_word(b, char_is_valid_var_name_material);
	}
	m->equal_defined = DEFINE_FUNC;
	m->to_define = s;
	return (1);
}
