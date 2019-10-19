/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trigger_name_funcs.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: overetou <overetou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/19 16:14:02 by overetou          #+#    #+#             */
/*   Updated: 2019/10/19 17:40:19 by overetou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "computor.h"

void	prepare_var_def(t_master *m, char *s)
{
	m->to_define = s;
	m->equal_defined = 1;
}

//We already check at each equal if there is no double.
char	alpha_exec(t_buf *b, t_master *m)
{
	char	*s;
	int		value;

	s = read_word(b, " \t");
	if (m->equal_defined == 0)
	{
		if (str_perfect_match("=", read_next_word(b, " \t")))
			prepare_var_def(s);//Dont forget to signal the = in the struct.
		else
			lancer la fonction au dessus de parse_trigger pour ne pas incrementer et perdre le char actuel. Attention a ne pas avoir de cas d'erreur due a EOF.
	}
	else
	{
		if (get_item_value(&value, s))
			add_value(m, value);
		else
			handle_line_error("Definition of a variable was not found.");
	}
	return (1);
}

char	equal_exec(t_buf *b, t_master *m)
{
	if (m->equal_defined)
		handle_line_error("At least two '=' are present on the line.");
	else if (m->prev != NOTHING)//If we have a variable to define, we keep prev to nothing.
		handle_line_error("'=' was not used to simply define a variable.");
	m->prev = EQUAL;//Not sure if this is useful.
}

//When reaching this point, we have the certitude that the line was ok.
char	endline_exec(t_buf *b, t_master *m)
{
	t_simple	*var;

	if (m->to_define)//This is a string with the name of the variable to define.
	{
		var = get_item(m->to_define);
		if (var)
			var->content = (void*)get_addition_result(m->exec_tracks);
		else
			track_add(m->vars, t_simple_init((void*)get_addition_result(m->exec_tracks)));
	}
	put_int((void*)get_addition_result(m->exec_tracks));
	prepare_new_line(m);
	return (1);
}
