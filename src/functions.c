/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   functions.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: overetou <overetou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/11 18:22:15 by overetou          #+#    #+#             */
/*   Updated: 2019/11/20 16:07:19 by overetou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "computor.h"
#include <stdio.h>

void	*get_func(t_master *m, const char *name)
{
	return (get_item(&(m->funcs), name));
}

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

void	remove_level(t_master *m)
{
	//printf("Trying to obliterate what stands at adress: %p\n", m->exec_tracks.last);
	track_remove_last(&(m->exec_tracks), destroy_link_track);
	//putendl("level successfully removed.");
}

t_expr	*extract_last_track_expr(t_master *m)
{
	t_expr	*e;

	//putendl("??????????????EXTRACT");
	e = get_last_first_expr(m);
	//if (((t_link_track*)(m->exec_tracks.last))->first == ((t_link_track*)(m->exec_tracks.last))->last)
		((t_link_track*)(m->exec_tracks.last))->first = NULL;
	//else
	//	((t_link_track*)(m->exec_tracks.last))->first = (t_link*)e->next;
	remove_level(m);
	printf("returned from func info: %d\n", e->info);
	return (e);
}

t_expr	*extract_func_result(t_master *m)
{
	putendl("extract_func_result: refine_addition_result");
	if (refine_addition_result((t_link_track*)(m->exec_tracks.last)) == 0)
	{
		putendl("refine_addition_result failed");
		return (NULL);
	}
	//printf("last first expr info = %d, value = %f\n", get_last_first_expr(m)->info, get_last_first_expr(m)->content.flt);
	return (extract_last_track_expr(m));
}

void	exec_symbol(t_master *m, char info)
{
	if (info == PARENT_OPEN)
		add_level(m);
	else if (info == PARENT_CLOSE)
		do_close_par_manipulation(m);
}

t_expr	*compute_func(t_master *m, t_expr *argument, char *func_name)
{
	t_expr	*cur;
	t_expr	powered_arg;

	putendl("ENTERED compute_func");
	printf("Current buf char: %c\n", m->buf.str[m->buf.pos]);
	cur = get_func(m, func_name);
	if (cur == NULL || argument == NULL)
	{
		putendl("failed to retreive the func");
		return (NULL);
	}
	cur = cur->content.expr;
	//printf("compute_func: found a function corresponding to the name. First value info = %d\n", cur->info);
	add_level(m);
	while (cur)
	{
		if (cur->info == UNKNOWN)
		{
			putendl("unknown detected.");
			printf("power of the unknown: %d\n", cur->content.integer);
			powered_arg.content = argument->content;
			powered_arg.info = argument->info;
			apply_power(&(powered_arg.content), &(powered_arg.info), cur->content.integer);
			printf("value injected after pwer: %f\n", powered_arg.content.flt);
			printf("Current buf char: %c\n", m->buf.str[m->buf.pos]);
			inject_value(m, powered_arg.content, powered_arg.info, 0);
		}
		else if (cur->info >= RATIONNAL)
		{
			putendl("value detected");
			inject_expr(m, (t_expr*)copy_expr((t_link*)cur));
		}
		if (int_is_comprised(cur->info, 1, 6))
			(*prev_adr(m)) = cur->info;
		else
			exec_symbol(m, cur->info);
		cur = cur->next;
	}
	return (extract_func_result(m));
}

char	*get_parenthesis_content(t_buf *b)
{
	size_t	size;
	char	*str;

	size = 1;
	if (b->str[b->pos] == ')')
		return (NULL);
	str = malloc(1 * sizeof(char));
	str[0] = b->str[b->pos];
	if (read_smart_inc(b) == 0)
		return (NULL);
	while (b->str[b->pos] != ')')
	{
		str = realloc(str, (size + 1) * sizeof(char));
		str[size] = b->str[b->pos];
		size++;
		if (read_smart_inc(b) == 0)
		{
			free(str);
			return (NULL);
		}
	}
	str = realloc(str, (size + 1) * sizeof(char));
	str[size] = '\0';
	putstr("Content of parenthesis: ");putendl(str);
	read_smart_inc(b);
	return (str);
}

t_var	*t_var_simple_init(char *s)
{
	t_var	*new;

	new = malloc(sizeof(t_var));
	new->name = s;
	return (new);
}

void	track_insert(t_track *t, t_link *l)
{
	if (t->first == NULL)
	{
		//putendl("track_insert: init");
		track_init(t, l);
	}
	else
	{
		//putendl("track_insert: add");
		track_add(t, l);
	}
}

void	prepare_func_definition(t_master *m, t_buf *b, char *s, char *parent_content)
{
	t_expr	*func;

	printf("ENtered prepare_func_definition. s = %s\n", s);
	func = get_item(&(m->funcs), s);
	if (func)
	{
		putendl("a func was found with that name.");
		printf("current buf = %c\n", m->buf.str[m->buf.pos]);
		if (read_smart_inc(b) == 0)
		{
			handle_line_error(m, "could not find a proper char after a func(x)=...");
			return;
		}
		read_till_false(b, is_sep);
		printf("current buf = %c\n", m->buf.str[m->buf.pos]);
		if (b->str[b->pos] == '?')
		{
			m->to_define = parent_content;
			display_given_func(m, func);
			m->equal_defined = OBJECT_DISPLAY;
		}
		else
		{
			/* Préparer ce qui pourra servir à redéfinir la foncion ou à résoudre une équation avec.*/
			extract_link_from_track(&(m->funcs), (t_link*)func);
			track_insert(&(m->funcs), (t_link*)func);
			m->equal_defined = DEFINE_FUNC;
			m->to_define = parent_content;
		}
		return;
	}
	read_till_false(b, is_sep);
	m->to_define = parent_content;
	track_insert(&(m->funcs), (t_link*)t_var_simple_init(s));
	m->equal_defined = DEFINE_FUNC;
	printf("first definition: current buf = %c\n", b->str[b->pos]);
	read_smart_inc(b);
}

BOOL	process_parent_and_handle_if_def_requested(t_master *m, t_buf *b, t_expr **e, char **parent_content)
{
	*e = NULL;

	if (read_smart_inc(b) == 0)
		return (0);
	if (is_alpha(b->str[b->pos]))
	{
		//putendl("alpha detected inside parenthesis.");
		*parent_content = get_parenthesis_content(b);
		if (*parent_content == NULL)
			return (0);
		if (is_definition(m, b))
		{
			//putendl("is definition was triggered. inside process_parent_and_handle_if_def_requested\n");
			return (1);
		}
		*e = (t_expr*)get_var(m, *parent_content);//TODO: understand what is going on here. This instruction seems to never be useful.
		return (0);
	}
	add_level(m);
	if (is_digit(b->str[b->pos]))
	{
		num_store(b, m);
		read_smart_inc(b);
	}
	else
		return (0);
	*e = extract_last_track_expr(m);
	return (0);
}

BOOL	handle_func(t_master *m, t_buf *b, char *s)
{
	t_expr	*e;
	char	*parent_content;

	//here parents stands for parenthesis.
	if (process_parent_and_handle_if_def_requested(m, b, &e, &parent_content))
	{
		putendl("handle_func: definition request found.");
		prepare_func_definition(m, b, s, parent_content);
	}
	else if (e == NULL)
		return (0);//If e == NULL and the previous conditions equals 0, it means that no function def is requested but that what was found inside the parenthesis makes no sens as argument for a func.
	else
	{
		printf("about to compute the func. Param = %f, info = %d\n", e->content.flt, e->info);
		e = compute_func(m, e, s);
		if (e)
		{
			//printf("handle_func: e still exists. info: %d, value: %f\n", e->info, e->content.flt);
			inject_expr(m, e);
		}
		else
			handle_line_error(m, "Problem detected while computing a function.");
		//printf("Before advancing, current buffer char = %c\n", m->buf.str[m->buf.pos]);
	}
	return (1);
}
