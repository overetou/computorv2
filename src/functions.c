/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   functions.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: overetou <overetou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/11 18:22:15 by overetou          #+#    #+#             */
/*   Updated: 2019/11/11 21:22:45 by overetou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "computor.h"
#include <stdio.h>

char	handle_func()
{
	t_expr	*e;

	if (is_definition(b))
		return (prepare_func_definition(m, b, s));
	else
	{
		e = compute_func(get_argument);
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
