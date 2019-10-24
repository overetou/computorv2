/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trigger_name_funcs.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: overetou <overetou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/19 16:14:02 by overetou          #+#    #+#             */
/*   Updated: 2019/10/24 17:20:01 by overetou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "computor.h"
#include <stdlib.h>

void	*t_var_init(t_master *m)
{
	t_var	*new;

	new = malloc(sizeof(t_var));
	new->name = m->to_define;
	m->to_define = NULL;
	new->content.integ = get_addition_result(&(((t_master*)m)->exec_tracks));
	return (new);
}

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

void	*t_expr_create(void)
{
	return (malloc(sizeof(t_expr)));
}

void	*t_expr_init(const int value)
{
	t_expr	*new;

	new = t_expr_create();
	new->content.integ = value;
	return (new);
}

//Puts the given value inside the current exec_track cell. Then create another.
void	mix_in_value_init(t_master *m, int value)
{
	((t_expr*)(m->exec_tracks.last))->content.integ = value;
}

//Create another cell and put the value in it.
void	mix_in_value(t_master *m, int value)
{
	track_add(&(m->exec_tracks), (t_link*)t_expr_create());
	((t_expr*)(m->exec_tracks.last))->content.integ = value;
}

void	*get_item(t_master *m, const char *name)
{
	t_var	*p;

	p = (t_var*)(m->vars.first);
	if (p == NULL)
		return (NULL);
	while (p != (t_var*)(m->vars.last))
	{
		if (str_perfect_match(name, p->name))
			return ((void*)p);
		p = p->next;
	}
	if (str_perfect_match(name, p->name))
		return ((void*)p);
	return (NULL);
}

BOOL	get_item_value(int *n, t_master *m, const char *name)
{
	t_var	*v;

	v = get_item(m, name);
	if (v == NULL)
		return (0);
	*n = v->content.integ;
	return (1);
}

void	mix_in_var_value_init(t_master *m, char *name)
{
	int		value;

	if (get_item_value(&value, m, name))
		mix_in_value_init(m, value);
	else
		handle_line_error(m, "Definition of a variable was not found.");
	free(name);
}

void	mix_in_var_value(t_master *m, char *name)
{
	int		value;

	if (get_item_value(&value, m, name))
		mix_in_value(m, value);
	else
		handle_line_error(m, "Definition of a variable was not found.");
	free(name);
}

//We already check at each equal if there is no double.
char	alpha_exec_init(t_buf *b, void *m)
{
	char	*s;
	char	save;

	s = read_word(b, char_is_valid_var_name_material);
	printf("alpha_exec_init: word read = %s\n", s);
	if (((t_master*)m)->equal_defined == 0)
	{
		putendl("alpha_exec_init: no equal defined.");
		save = b->str[b->pos];
		if (read_till_false(b, is_sep))
		{
			if (b->str[b->pos] == '=') 
			{
				putendl("alpha_exec_init: equal detected.");
				if (read_smart_inc(b))
				{
					putendl("alpha_exec_init: equal confirmed.");
					prepare_var_def(m, s);//Dont forget to signal the = in the struct.
				}
				else
					handle_line_error(m, "= is followed by nothing.");
				return (1);
			}
			else if (save == b->str[b->pos] && b->str[b->pos] != '\n')
			{
				handle_line_error(m, "Invalid variable name found.");
				return (1);
			}
		}
	}
	mix_in_var_value_init((t_master*)m, s);
	((t_simple*)get_link_by_index(((t_master*)m)->trigger_funcs.first, 7))->content = alpha_exec;
	return (1);
}

//We already check at each equal if there is no double.
char	alpha_exec(t_buf *b, void *m)
{
	char	*s;
	char	save;

	s = read_word(b, char_is_valid_var_name_material);
	if (((t_master*)m)->equal_defined == 0)
	{
		save = b->str[b->pos];
		if (read_till_false(b, is_sep))
		{
			if (b->str[b->pos] == '=') 
			{
				if (read_smart_inc(b) && is_sep(b->str[b->pos]))
					prepare_var_def(m, s);//Dont forget to signal the = in the struct.
				else
					handle_line_error(m, "= is followed by nothing.");
				return (1);
			}
			else if (save == b->str[b->pos])
			{
				handle_line_error(m, "Invalid variable name found.");
				return (1);
			}
		}
	}
	mix_in_var_value((t_master*)m, s);
	return (1);
}

char	equal_exec(t_buf *b, void *m)
{
	if (((t_master*)m)->equal_defined)
		handle_line_error(m, "At least two '=' are present on the line.");
	else if (((t_master*)m)->prev != NOTHING)//If we have a variable to define, we keep prev to nothing.
		handle_line_error(m, "'=' was not used to simply define a variable.");
	((t_master*)m)->prev = EQUAL;//Not sure if this is useful.
	read_smart_inc(b);
	return (1);
}

//When reaching this point, we have the certitude that the line was ok.
char	endline_exec(t_buf *b, void *m)
{
	t_expr	*var;

	if (((t_master*)m)->to_define)//This is a string with the name of the variable to define.
	{
		var = get_item(m, ((t_master*)m)->to_define);
		putendl("hello !");
		if (var)
			var->content.integ = get_addition_result(&(((t_master*)m)->exec_tracks));
		else
		{
			if (var == NULL)
				track_init(&(((t_master*)m)->vars), t_var_init(m));
			track_add(&(((t_master*)m)->vars), t_var_init(m));
		}
	}
	quick_putnb(get_addition_result(&(((t_master*)m)->exec_tracks)));
	putchr('\n');
	prepare_new_line(m);
	read_smart_inc(b);
	return (1);
}
