/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_tracks.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: overetou <overetou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/18 17:28:12 by overetou          #+#    #+#             */
/*   Updated: 2019/11/05 17:21:17 by overetou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "computor.h"
#include <stdio.h>

#define EXEC_LAST ((t_expr*)(((t_link_track*)(m->exec_tracks.last))->last))->content.integ

void	prepare_new_line(t_master *m)
{
	//print_track_values(m);
	destroy_link_track_content((t_link_track*)(m->exec_tracks.first));
	*(prev_adr(m)) = NOTHING;
	m->equal_defined = 0;
}

BOOL	handle_line_error(t_master *m, const char *s)
{
	putstr("Error: ");
	putendl(s);
	if (m->exec_tracks.first != m->exec_tracks.last)
		destroy_track_from_to(m->exec_tracks.first->next, m->exec_tracks.last, destroy_link_track);
	prepare_new_line(m);
	if (m->buf.str[m->buf.pos] == '\n')
		read_smart_inc(&(m->buf));
	else
		read_go_next_line(&(m->buf));
	return (1);
}

//Here we must be guaranteed that the current expression and all the preceding one (if any)
//are defined. Returns 1 on success, 0 other wise.
char	exec_cell_if_prior(t_master *m, t_content value, char info)
{
	if (int_is_comprised(prev(m), MINUS_MULT, MINUS_MODULO))
		*(prev_adr(m)) -= 5;
	if (prev(m) == MULT)
		do_multiplication(m, value, info);
	else if (prev(m) == DIV)
		do_division(m, value, info);
	else if (prev(m) == MODULO)
		do_modulation(m, value, info);
	else
		return (0);
	return (1);
}

void	aglomerate_type(t_expr *m1, t_expr* m2, t_expr *last)
{
	while (m2 != last)
	{
		if (m1->info == m2->info)
		{
			addition_same_type(m1, m2);
			m2->info = PROCESSED;
		}
		m2 = m2->next;
	}
	if (m1->info == m2->info)
	{
		addition_same_type(m1, m2);
		m2->info = PROCESSED;
	}
}

void	pull_up_aglomerated(t_expr *e, t_link_track *t)
{
	t_expr	*prec;

	if (e != (t_expr*)t->first)
	{
		prec = (t_expr*)(t->first);
		while (prec->next != e)
			prec = prec->next;
		prec->next = e->next;
		e->next = ((t_expr*)(t->first))->next;
		t->first = (t_link*)e;
	}

}

//Ne pas oublier de se proteger des overflows ici aussi. Puts the addition result inside the firsts link of the track.
BOOL	refine_addition_result(t_link_track *t)
{
	t_expr	*current;
	t_expr	*next;

	if (t->first == t->last)
	{
		if (((t_expr*)(t->first))->info == PACK)
		{
			current = (t_expr*)(t->first);
			t->first = (t_link*)(((t_expr*)(t->first))->content.expr);
			next = (t_expr*)(t->first);
			while (next->next)
				next = next->next;
			t->last = (t_link*)next;
		}
		return (1);
	}
	current = (t_expr*)(t->first);
	next = current->next;
	while (next != (t_expr*)(t->last))
	{
		if (next->info != PROCESSED)
		{
			if (current->info != next->info)
			{
				aglomerate_type(current, next, (t_expr*)t->last);
				pull_up_aglomerated(current, t);
				current = current->next;
				while (current->info == PROCESSED)
					current = current->next;
			}
			else if (addition_same_type(current, next) == 0)
				return (0);
		}
		next = next->next;
	}
	if (next->info == PROCESSED)
		return (1);
	if (current->info == next->info)
	{
		if (addition_same_type(current, next) == 0)
			return (0);
	}
	return (1);
}
