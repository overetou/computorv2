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
	((t_link_track*)(m->exec_tracks.last))->first = NULL;
	remove_level(m);
	return (e);
}

t_expr	*extract_func_result(t_master *m)
{
	putendl("extract_func_result: refine_addition_result");
	if (refine_addition_result((t_link_track*)(m->exec_tracks.last)) == 0)
		return (NULL);
	return (extract_last_track_expr(m));
}

t_expr	*compute_func(t_master *m, t_expr *argument, char *func_name)
{
	t_expr	*cur;
	t_expr	*next;
	t_expr	powered_arg;

	putendl("ENTERED compute_func");
	cur = get_func(m, func_name);
	if (cur == NULL || argument == NULL)
		return (NULL);
	cur = cur->content.expr;
	//printf("compute_func: found a function corresponding to the name. First value info = %d\n", cur->info);
	add_level(m);
	while (cur)
	{
		next = cur->next;
		if (cur->info == UNKNOWN)
		{
			powered_arg.content = argument->content;
			powered_arg.info = argument->info;
			apply_power(&(powered_arg.content), &(powered_arg.info), cur->content.integer);
			inject_value(m, powered_arg.content, powered_arg.info);
		}
		else
			inject_expr(m, (t_expr*)copy_expr((t_link*)cur));
		cur = next;
		if (cur)
		{
			(*prev_adr(m)) = cur->info;
			next = cur->next;
			//free_expr(cur);
			cur = next;
		}
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
	str = malloc(sizeof(char));
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
	read_till_false(b, is_sep);
	m->to_define = parent_content;
	track_insert(&(m->funcs), (t_link*)t_var_simple_init(s));
	m->equal_defined = DEFINE_FUNC;
}

BOOL	process_parent_and_handle_if_def_requested(t_master *m, t_buf *b, t_expr **e, char **parent_content)
{
	*e = NULL;

	if (read_smart_inc(b) == 0)
		return (0);
	if (is_alpha(b->str[b->pos]))
	{
		*parent_content = get_parenthesis_content(b);
		if (*parent_content == NULL)
			return (0);
		if (is_definition(m, b))
			return (1);
		*e = (t_expr*)get_var(m, *parent_content);//TODO: understand what is going on here. This instruction seems to never be useful.
		return (0);
	}
	add_level(m);
	if (is_digit(b->str[b->pos]))
		num_store(b, m);
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
		read_smart_inc(b);
	}
	else if (e == NULL)
		return (0);//If e == NULL and the previous conditions equals 0, it means that no function def is requested but that what was found inside the parenthesis makes no sens as argument for a func.
	else
	{
		e = compute_func(m, e, s);
		if (e)
			inject_expr(m, e);
		else
			handle_line_error(m, "Problem detected while computing a function.");
		read_smart_inc(b);
	}
	return (1);
}
