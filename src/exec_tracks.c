/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_tracks.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: overetou <overetou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/18 17:28:12 by overetou          #+#    #+#             */
/*   Updated: 2019/10/26 18:17:19 by overetou         ###   ########.fr       */
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
	read_go_next_line(&(m->buf));
	prepare_new_line(m);
}

//Here we must be guaranteed that the current expression and all the preceding one (if any)
//are defined. Returns 1 on success, 0 other wise.
char	exec_cell_if_prior(t_master *m, int value)
{
	if (int_is_comprised(prev(m), MINUS_MULT, MINUS_MODULO))
		*(prev_adr(m)) -= 5;
	if (prev(m) == MULT)
		EXEC_LAST *= value;
	else if (prev(m) == DIV)
		EXEC_LAST /= value;
	else if (prev(m) == MODULO)
		EXEC_LAST %= value;
	else
		return (0);
	return (1);
}

//Ne pas oublier de se proteger des overflows ici aussi.
int	get_addition_result(t_link_track *t)
{
	int			result;
	t_simple	*s;

	result = (int)(t_simpl_content(t->first));
	if (t->first == t->last)
		return (result);
	s = (t_simple*)(t->first->next);
	while (s != (t_simple*)(t->last))
	{
		result += (int)(s->content);
		s = s->next;
	}
	result += (int)(s->content);
	return (result);
}
