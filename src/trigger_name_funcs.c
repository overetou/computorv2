/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trigger_name_funcs.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: overetou <overetou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/19 16:14:02 by overetou          #+#    #+#             */
/*   Updated: 2019/10/21 19:37:12 by overetou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "computor.h"

void	prepare_var_def(t_master *m, char *s)
{
	m->to_define = s;
	m->equal_defined = 1;
}

BOOL	char_is_valid_var_name_material(const char c)
{
	return (int_is_comprised(c, 'a', 'z') || int_is_comprised(c, 'A', 'Z') || c == '_');
}

BOOL	is_sep(const char c)
{
	return (c == ' ' || c == '\t');
}

//We already check at each equal if there is no double.
char	alpha_exec(t_buf *b, void *m)
{
	char	*s;
	int		value;

	s = read_word(b, char_is_valid_var_name_material);
	if (((t_master*)m)->equal_defined == 0)
	{
		read_till_false(b, is_sep);
		if (b->str[b->pos] == '=' && read_smart_inc(b) && is_sep(b->str[b->pos]))
			prepare_var_def(m, s);//Dont forget to signal the = in the struct.
		else
			add_value(m);
	}
	else
	{
		if (get_item_value(&value, s))
			add_value(m, value);
		else
			handle_line_error(m, "Definition of a variable was not found.");
	}
	return (1);
}

char	equal_exec(t_buf *b, void *m)
{
	if (((t_master*)m)->equal_defined)
		handle_line_error(m, "At least two '=' are present on the line.");
	else if (((t_master*)m)->prev != NOTHING)//If we have a variable to define, we keep prev to nothing.
		handle_line_error(m, "'=' was not used to simply define a variable.");
	((t_master*)m)->prev = EQUAL;//Not sure if this is useful.
	(void)b;
	return (1);
}

//When reaching this point, we have the certitude that the line was ok.
char	endline_exec(t_buf *b, void *m)
{
	t_simple	*var;
	int			*content;

	if (((t_master*)m)->to_define)//This is a string with the name of the variable to define.
	{
		var = get_item(((t_master*)m)->to_define);
		if (var)
		{
			content = (int*)(&(var->content));
			*content = get_addition_result(&(((t_master*)m)->exec_tracks));
		}
		else
			track_add(((t_master*)m)->vars, t_simple_init((void*)get_addition_result(m->exec_tracks)));
	}
	put_int(get_addition_result(&(((t_master*)m)->exec_tracks)));
	prepare_new_line(m);
	(void)b;
	return (1);
}
