/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   functions.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: overetou <overetou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/11 18:22:15 by overetou          #+#    #+#             */
/*   Updated: 2019/11/15 20:51:36 by overetou         ###   ########.fr       */
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
		printf("current char = %c\n", b->str[b->pos]);
		if (b->str[b->pos] == '=') 
			return (1);
	}
	return (0);
}

void	remove_level(t_master *m)
{
	track_remove_last(&(((t_master*)m)->exec_tracks), destroy_link_track);
}

t_expr	*extract_last_track_expr(t_master *m)
{
	t_expr	*e;

	e = get_last_first_expr(m);
	((t_link_track*)(m->exec_tracks.last))->first = NULL;
	remove_level(m);
	return (e);
}

t_expr	*get_argument(t_master *m, t_buf *b)
{
	t_expr	*e;

	read_till_false(b, is_sep);
	add_level(m);
	e = extract_last_track_expr(m);
	if (b->str[b->pos] == ')')
		return (e);
	return (NULL);
}

t_expr	*extract_func_result(t_master *m)
{
	if (refine_addition_result((t_link_track*)(m->exec_tracks.last)) == 0)
		return (NULL);
	return (extract_last_track_expr(m));
}

t_expr	*compute_func(t_master *m, t_expr *argument, char *func_name)
{
	t_expr	*cur;
	t_expr	*next;

	cur = get_func(m, func_name);
	cur = cur->content.expr;
	add_level(m);
	while (cur)
	{
		next = cur->next;
		if (cur->info == UNKNOWN)
			inject_expr(m, argument);
		else
			inject_expr(m, cur);
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
	if (b->str[b->pos] != '(')
		return (NULL);
	if (read_smart_inc(b) == 0)
		return (NULL);
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
	putstr("Content of parenthesis: ");dry_putstr(str, size);putchr('\n');
	read_smart_inc(b);
	return (str);
}

BOOL	handle_func(t_master *m, t_buf *b, char *s)
{
	t_expr	*e;
	char	*parenthesis_content;

	parenthesis_content = get_parenthesis_content(b);
	if (parenthesis_content == NULL && b->length == 0)
	{
		handle_line_error(m, "Problem with a function parenthesis.");
		return (1);
	}
	putendl("handle_line_error: succesfully read parenthesis content.");
	if (is_definition(m, b))
	{
		putendl("handle_func: definition found.");
		return (prepare_func_definition(m, b, s));
	}
	else
	{
		e = compute_func(m, get_argument(m, b), s);
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

t_var	*true_var_init(char *s)
{
	t_var	*new;

	new = malloc(sizeof(t_var));
	new->name = s;
	return (new);
}

char	prepare_func_definition(t_master *m, t_buf *b, char *s)
{
	read_till_false(b, is_sep);
	if (b->str[b->pos] == ')')
		m->to_define = NULL;
	else
	{
		if (!char_is_valid_var_name_material(b->str[b->pos]))
			return (0);
		m->to_define = read_word(b, char_is_valid_var_name_material);
	}
	track_add(&(m->funcs), (t_link*)true_var_init(s));
	m->equal_defined = DEFINE_FUNC;
	return (1);
}
