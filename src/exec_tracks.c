/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_tracks.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: overetou <overetou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/18 17:28:12 by overetou          #+#    #+#             */
/*   Updated: 2019/11/26 17:18:43 by overetou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "computor.h"
#include <stdio.h>

void	prepare_new_line(t_master *m)
{
	destroy_link_track_content((t_link_track*)(m->exec_tracks.first));
	putendl("\nbefore invalid write size.");
	*(prev_adr(m)) = NOTHING;
	putendl("after invalid write size.");
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
char	exec_cell_if_prior(t_master *m, t_expr *e)
{
	////printf("exec_cell_if_prior: prev = %d\n", prev(m));
	if (int_is_comprised(prev(m), MINUS_MULT, MINUS_MODULO))
		*(prev_adr(m)) -= 5;
	if (prev(m) == MULT)
	{
		putendl("/!\\ mult detected");
		do_multiplication(m, e);
	}
	else if (prev(m) == DIV)
		do_division(m, e->content, e->info);
	else if (prev(m) == MODULO)
		do_modulation(m, e->content, e->info);
	else
		return (0);
	return (1);
}

void	aglomerate_type(t_expr *e1, t_expr* e2, t_expr *last)
{
	while (1)
	{
		//printf("aglomerate_type: try\n\tval:%f info:%d x degree:%zu\n\tval:%f info:%d x degree:%zu\n", e1->content.flt, e1->info, e1->unknown_degree, e2->content.flt, e2->info, e2->unknown_degree);
		if (e1->info == e2->info && e1->unknown_degree == e2->unknown_degree)
		{
			addition_same_type(e1, e2);
			e2->info = PROCESSED;
		}
		if (e2 == last)
			break;
		e2 = e2->next;
	}
}

t_link *copy_expr(t_link *l)
{
	//printf("copying an expression. Content = %f, info = %d\n", ((t_expr*)l)->content.flt, ((t_expr*)l)->info);
	return	((t_link*)t_expr_init(((t_expr*)l)->content, ((t_expr*)l)->info));
}

t_expr	*unzip_pack(t_link_track *t, t_expr *curr)
{
	////printf("unzip_pack: %f ; %f\n", new_cur->content.flt, new_cur->next->content.flt);
	return ((t_expr*)link_track_replace_link_with_list_copy(t, (t_link*)curr, (t_link*)(curr->content.expr), copy_expr));
}

//Ne pas oublier de se proteger des overflows ici aussi. Puts the addition result inside the firsts link of the track.
BOOL	refine_addition_result(t_link_track *t)
{
	t_expr	*current;
	t_expr	*next;
	
	if (t->first == t->last)
	{
		putendl("entered one element in the track case. (in refine addition)");
		if (((t_expr*)t->first)->info == PACK)
			unzip_pack(t, (t_expr*)t->first);
		printf("expression type: %d\n", ((t_expr*)(t->first))->info);
		return (1);
	}
	current = (t_expr*)(t->first);
	while (1)
	{
		putendl("Entered multiple elements case in refine track.");
		if (current->info == PACK)
		{
		putendl("tatbefore infinite loop.");
			current = unzip_pack(t, current);
			putendl("refine_addition_result: still alive after unzip");
			//printf("args = %f, %f, %f\n", current->content.flt, current->next->content.flt, current->next->next->content.flt);
		}
		next = current->next;
		putendl("Alive before aglomerate by type.");
		aglomerate_type(current, next, (t_expr*)t->last);
		putendl("after infinite loop");
		////printf("aglo result: %f * x^%zu\n", current->content.flt, current->unknown_degree);
		link_track_push_internal_link((t_link*)current, t);
		putendl("PUSH DONE");
		current = next;
		////printf("new curr: %f * x^%zu, info = %d\n", current->content.flt, current->unknown_degree, current->info);
		while (current->info == PROCESSED && current != (t_expr*)(t->last))
			current = current->next;
		if (current == (t_expr*)(t->last))
			return (1);
		putendl("refine_addition_result: one spin.");
	}
}