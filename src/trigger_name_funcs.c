/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trigger_name_funcs.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: overetou <overetou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/19 16:14:02 by overetou          #+#    #+#             */
/*   Updated: 2019/10/29 18:38:33 by overetou         ###   ########.fr       */
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
	//printf("t_var_init: new->name = %s\n", new->name);
	new->content = (((t_expr*)((t_link_track*)(m->exec_tracks.last))->first))->content;
	new->info = (((t_expr*)((t_link_track*)(m->exec_tracks.last))->first))->info;
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

void	*t_expr_init(t_content content, char info)
{
	t_expr	*new;

	new = t_expr_create();
	new->content = content;
	new->info = info;
	return (new);
}

//Writes directly on the first time, then add a cell.
void	mix_in_value(t_master *m, t_content content, char info)
{
	if (EXEC_TRACK_LAST_AS_LINK_TRACK->first == NULL)
		link_track_init(EXEC_TRACK_LAST_AS_LINK_TRACK, (t_link*)t_expr_init(content, info));
	else
		link_track_add(EXEC_TRACK_LAST_AS_LINK_TRACK, (t_link*)t_expr_init(content, info));
}

void	inject_value(t_master *m, t_content content, char info)
{
	if (!exec_cell_if_prior((t_master*)m, content, info))
		mix_in_value(m, content, info);
	*(prev_adr(m)) = VALUE;
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
	//putendl("get_item: one member left.");
	//printf("get_item: name = %s, content = %s\n", name, p->name);
	if (str_perfect_match(name, p->name))
		return ((void*)p);
	return (NULL);
}

char	get_item_value(t_content *n, t_master *m, const char *name)
{
	t_var	*v;

	v = get_item(m, name);
	if (v == NULL)
		return (-1);
	*n = v->content;
	return (v->info);
}

void	inject_var_value(t_master *m, char *name)
{
	t_content	value;
	char		info;

	info = get_item_value(&value, m, name);
	if (info >= 0)
		inject_value(m, value, info);
	else
	{
		m->to_define = NULL;
		handle_line_error(m, "Definition of a variable was not found.");
	}
	free(name);
}

//We already check at each equal if there is no double.
char	alpha_exec(t_buf *b, void *m)
{
	char	*s;
	char	save;

	s = read_word(b, char_is_valid_var_name_material);
	//printf("alpha_exec: word read = %s\n", s);
	if (((t_master*)m)->equal_defined == 0)
	{
		//putendl("alpha_exec: no equal defined.");
		save = b->str[b->pos];
		read_till_false(b, is_sep);
		if (b->str[b->pos] == '=') 
		{
			//putendl("alpha_exec: equal detected.");
			if (is_inside_parenthesis(m))
			{
				handle_line_error(m, "= was defined inside a parenthesis.");
				return (1);
			}
			if (read_smart_inc(b))
			{
				//putendl("alpha_exec: equal confirmed.");
				prepare_var_def(m, s);//Dont forget to signal the = in the struct.
			}
			else
				handle_line_error(m, "= is followed by nothing.");
			*(prev_adr(m)) = EQUAL;
			return (1);
		}
	}
	*(prev_adr(m)) = VALUE;
	inject_var_value((t_master*)m, s);
	return (1);
}

char	equal_exec(t_buf *b, void *m)
{
	if (((t_master*)m)->equal_defined)
		handle_line_error(m, "At least two '=' are present on the line.");
	else if (prev(m) != NOTHING)//If we have a variable to define, we keep prev to nothing.
		handle_line_error(m, "'=' was not used to simply define a variable.");
	*(prev_adr(m)) = EQUAL;
	read_smart_inc(b);
	return (1);
}

//When reaching this point, we have the certitude that the line was ok.
char	endline_exec(t_buf *b, void *m)
{
	t_expr	*var;

	if (((t_master*)m)->to_define)//This is a string with the name of the variable to define.
	{
		if (((t_master*)m)->vars.first == NULL)
			track_init(&(((t_master*)m)->vars), t_var_init(m));
		else
		{
			var = get_item(m, ((t_master*)m)->to_define);
			if (var)
			{
				if (condense_last_track(m))
				{
					var->content = ((t_expr*)(((t_link_track*)(((t_master*)m)->exec_tracks.last))->first));
					var->info = ((t_link_track*)(((t_master*)m)->exec_tracks.last))->info;
				}
				else
				{
					handle_line_error(m, "An error appened during the addiditon of expressions destined to a variable definition.");
					return (1);
				}
			}
			else
				track_add(&(((t_master*)m)->vars), t_var_init(m));
		}
	}
	//print_track_values(m);
	display_last_expr(m);
	putchr('\n');
	prepare_new_line(m);
	read_smart_inc(b);
	return (1);
}
