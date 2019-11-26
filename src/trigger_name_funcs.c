/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trigger_name_funcs.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: overetou <overetou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/19 16:14:02 by overetou          #+#    #+#             */
/*   Updated: 2019/11/19 21:16:59 by overetou         ###   ########.fr       */
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
	m->to_define = NULL;
}

void	prepare_var_def(t_master *m, char *s)
{
	m->to_define = s;
	m->equal_defined = DEFINE_VAR;
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
	new->unknown_degree = 0;
	return (new);
}

//Writes directly on the first time, then add a cell.
void	mix_in_expr(t_master *m, t_expr *e)
{
	if (e->info == PACK)
	{
		e->info = ((t_expr*)(e->content.expr))->info;
		e->content = ((t_expr*)(e->content.expr))->content;
	}
	if (EXEC_TRACK_LAST_AS_LINK_TRACK->first == NULL)
	{
		putendl("mix_in_expr: INIT");
		link_track_init(EXEC_TRACK_LAST_AS_LINK_TRACK, (t_link*)e);
	}
	else
	{
		putendl("mix_in_expr: ADD");
		link_track_add(EXEC_TRACK_LAST_AS_LINK_TRACK, (t_link*)e);
	}
	printf("added an expr of type :%d\n", e->info);
	if (m->exec_tracks.first == m->exec_tracks.last)
		putendl("mix_in_expr: added on main track.");
	printf("Mixed: %f * x^%zu\n", e->content.flt, e->unknown_degree);
}

void	inject_value(t_master *m, t_content content, char info)
{
	if (!exec_cell_if_prior((t_master*)m, t_expr_init(content, info)))
		mix_in_expr(m, t_expr_init(content, info));
	*(prev_adr(m)) = VALUE;
}

void	inject_expr(t_master *m, t_expr *e)
{
	if (m->matrice_depht)
	{
		putendl("XXXXXX\nmatrice depht detected.\nXXXXXX");
		mix_in_expr(m, e);
		e = e->content.expr;
	}
	else
	{

		if (exec_cell_if_prior(m, e))
		{
			//putendl("inject_expr:");
			putendl("A prioritary operation was executed");
			//free_expr(e);//use a special function here: free_expr that can free packs.
		}
		else
		{
			mix_in_expr(m,e);
		}
	}
	*(prev_adr(m)) = VALUE;
}

void	*get_item(t_track *t, const char *name)
{
	t_var	*p;

	p = (t_var*)(t->first);
	if (p == NULL)
		return (NULL);
	while (p != (t_var*)(t->last))
	{
		if (str_perfect_match(name, p->name))
			return ((void*)p);
		p = p->next;
	}
	if (str_perfect_match(name, p->name))
		return ((void*)p);
	return (NULL);
}

void	*get_var(t_master *m, const char *name)
{
	return (get_item(&(m->vars), name));
}

char	get_item_value(t_content *n, t_master *m, const char *name)
{
	t_var	*v;

	v = get_var(m, name);
	if (v == NULL)
		return (-1);
	*n = v->content;
	return (v->info);
}

void	mix_var_value(t_master *m, char *name)
{
	t_content	value;
	char		info;

	info = get_item_value(&value, m, name);
	if (info >= 0)
		mix_in_expr(m, t_expr_init(value, info));
	else
	{
		free(m->to_define);
		m->to_define = NULL;
		handle_line_error(m, "The definition of a variable was not found.");
	}
	free(name);
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
		putendl("inject_var_value: trying to add a var as an unknown.");
		try_var_as_unknown(m, name);//as a result, if we reach endline before a ?, we check if there are unknown and if there are, error.
	}
	free(name);
}

void	convert_to_irationnal(t_expr *e)
{
	if (e->info == RATIONNAL)
	{
		e->info = IRATIONNAL;
	}
	else if (e->info == IRATIONNAL)
	{
		putendl("Il y a quelqu'un ?");
		e->content.flt = -(e->content.flt);
		e->info = RATIONNAL;
	}
}

BOOL	apply_i(t_master *m)
{
	t_content	c;

	if (prev(m) == VALUE || prev(m) == MULT)
	{
		convert_to_irationnal(get_last_last_expr(m));
		printf("apply_i: Converted %f to i.\n", get_last_last_expr(m)->content.flt);
	}
	else if (prev(m) == PLUS)
	{
		c.flt = 1;
		mix_in_expr(m, t_expr_init(c, IRATIONNAL));
	}
	else if (prev(m) == MINUS)
	{
		c.flt = -1;
		mix_in_expr(m, t_expr_init(c, IRATIONNAL));
	}
	else
		handle_line_error(m, "'i' was used in a non supported scenario.");
	return (1);
}

//We already check at each equal if there is no double.
char	alpha_exec(t_buf *b, void *m)
{
	char	*s;

	printf("alpha_exec: to_define = %p\n", ((t_master*)m)->to_define);
	s = read_word(b, char_is_valid_var_name_material);
	if (str_perfect_match(s, "i"))
		return (apply_i(m));
	if (b->str[b->pos] == '(')
		return (handle_func(m, b, s));
	if (((t_master*)m)->equal_defined == 0)
	{
		putendl("alpha_exec: '=' is not yet defined.");
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
	else if (((t_master*)m)->equal_defined == DEFINE_FUNC && str_perfect_match(s, ((t_master*)m)->to_define))
	{
		putendl("added an unknown in function definition.");
		mix_in_expr(m, t_expr_init((t_content)NULL, UNKNOWN));
		*(prev_adr(m)) = VALUE;
		return (1);
	}
	printf("alpha_exec: Preparing to inject a var val. equal_defined = %d\n", ((t_master*)m)->equal_defined);
	if (((t_master*)m)->equal_defined == DEFINE_FUNC)
		mix_var_value(m, s);
	else
		inject_var_value(m, s);
	*(prev_adr(m)) = VALUE;
	return (1);
}

void	define_variable(t_master *m)
{
	t_expr	*var;

	putendl("/////////////\n DEFINE_VARIABLE\n////////////");
	if (((t_master*)m)->vars.first == NULL)
		track_init(&(((t_master*)m)->vars), t_var_init(m));
	else
	{
		var = get_var(m, ((t_master*)m)->to_define);
		if (var)
			t_var_update(var, m);
		else
			track_add(&(((t_master*)m)->vars), t_var_init(m));
	}
	m->to_define = NULL;
}

void	display_sign(t_expr *e)
{
	if (e->info == MULT)
		putstr(" * ");
	else if (e->info == MINUS)
		putstr(" - ");
	else if (e->info == PLUS)
		putstr(" + ");
	else if (e->info == DIV)
		putstr(" / ");
	else if (e->info == MODULO)
		putstr(" % ");
}

void	display_func(t_master *m)
{
	t_expr *e;

	e = get_last_first_expr(m);
	printf("display_func: last expr info = %d\n", get_last_last_expr(m)->info);
	while (e)
	{
		display_expr(e, m);
		if (e->next == NULL)
			return ;
		e = e->next;
		display_sign(e);
		e = e->next;
	}
}

void	affect_func(t_master *m)
{
	((t_expr*)(m->funcs.last))->content.expr = get_last_first_expr(m);
	((t_link_track*)(m->exec_tracks.first))->first = NULL;
}

char	endline_exec(t_buf *b, void *m)
{
	if (((t_master*)m)->equal_defined == DEFINE_FUNC)
	{
		printf("endline_exec: first = %p, info = %d\n", get_last_first_expr(m), get_last_first_expr(m)->info);
		get_last_last_expr(m)->next = NULL;
		display_func(m);
		affect_func(m);
		((t_master*)m)->to_define = NULL;
	}
	else
	{
		if (condense_last_track(m) == 0)
			handle_line_error(m, "A problem happended while doing additions.");
		else if (((t_master*)m)->to_define)
		{
			define_variable(m);
		}
		display_last_expr(m);
	}
	putchr('\n');
	prepare_new_line(m);
	read_smart_inc(b);
	return (1);
}
