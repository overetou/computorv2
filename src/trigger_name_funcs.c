/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trigger_name_funcs.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: overetou <overetou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/19 16:14:02 by overetou          #+#    #+#             */
/*   Updated: 2019/10/31 16:59:21 by overetou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "computor.h"
#include <stdlib.h>

void	*t_var_init(t_master *m)
{
	t_var	*new;

	new = malloc(sizeof(t_var));
	new->name = m->to_define;
	//printf("t_var_init: new->name = %s\n", new->name);
	new->content = (((t_expr*)((t_link_track*)(m->exec_tracks.last))->first))->content;
	new->info = (((t_expr*)((t_link_track*)(m->exec_tracks.last))->first))->info;
	return (new);
}

void	t_var_update(t_expr *v, t_master *m)
{
	v->content = (((t_expr*)((t_link_track*)(m->exec_tracks.last))->first))->content;
	v->info = (((t_expr*)((t_link_track*)(m->exec_tracks.last))->first))->info;
	free(m->to_define);
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

void	convert_to_irationnal(t_expr *e)
{
	if (e->info == RATIONNAL)
		e->info = IRATIONNAL;
	if (e->info == IRATIONNAL)
	{
		e->content.flt = -(e->content.flt);
		e->info = RATIONNAL;
	}
}

BOOL	apply_i(t_master *m)
{
	t_content	c;

	if (prev(m) == VALUE || prev(m) == MULT)
		convert_to_irationnal(get_last_last_expr(m));
	else if (prev(m) == PLUS)
	{
		c.flt = 1;
		mix_in_value(m, c, IRATIONNAL);
	}
	else if (prev(m) == MINUS)
	{
		c.flt = -1;
		mix_in_value(m, c, IRATIONNAL);
	}
	else
		handle_line_error(m, "'i' was used in a non supported scenario.");
	return (1);
}

//We already check at each equal if there is no double.
char	alpha_exec(t_buf *b, void *m)
{
	char	*s;

	s = read_word(b, char_is_valid_var_name_material);
	if (str_perfect_match(s, "i"))
		return (apply_i(m));
	if (((t_master*)m)->equal_defined == 0)
	{
		read_till_false(b, is_sep);
		if (b->str[b->pos] == '=') 
		{
			if (is_inside_parenthesis(m))
				return (handle_line_error(m, "= was defined inside a parenthesis."));
			if (read_smart_inc(b))
				prepare_var_def(m, s);//Dont forget to signal the = in the struct.
			else
				return (handle_line_error(m, "= is followed by nothing."));
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

void	define_variable(t_master *m)
{
	t_expr	*var;

	if (((t_master*)m)->vars.first == NULL)
		track_init(&(((t_master*)m)->vars), t_var_init(m));
	else
	{
		var = get_item(m, ((t_master*)m)->to_define);
		if (var)
			t_var_update(var, m);
		else
			track_add(&(((t_master*)m)->vars), t_var_init(m));
	}
	m->to_define = NULL;
}

char	endline_exec(t_buf *b, void *m)
{
	if (condense_last_track(m) == 0)
		handle_line_error(m, "A problem happended while doing additions.");
	if (((t_master*)m)->to_define)
		define_variable(m);
	display_last_expr(m);
	putchr('\n');
	prepare_new_line(m);
	read_smart_inc(b);
	return (1);
}
