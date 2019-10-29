/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_tracks.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: overetou <overetou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/18 17:28:12 by overetou          #+#    #+#             */
/*   Updated: 2019/10/29 18:32:20 by overetou         ###   ########.fr       */
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

void	handle_line_error(t_master *m, const char *s)
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

//Ne pas oublier de se proteger des overflows ici aussi. Puts the addition result inside the fist link of the track.
BOOL	refine_addition_result(t_link_track *t)
{
	t_expr	*current;
	t_expr	*next;

	if (t->first == t->last)
		return (1);
	current = (t_expr*)(t->first);
	next = current->next;
	while (next != (t_expr*)(t->last))
	{
		if (do_addition(current, next) == 0)
			return (0);
		next = next->next;
	}
	if (do_addition(current, next) == 0)
		return (0);
	return (1);
}
