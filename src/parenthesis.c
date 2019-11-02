/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parenthesis.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: overetou <overetou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/26 17:05:58 by overetou          #+#    #+#             */
/*   Updated: 2019/11/02 18:01:02 by overetou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "computor.h"
#include <stdio.h>

BOOL	is_inside_parenthesis(t_master *m)
{
	return (m->exec_tracks.first != m->exec_tracks.last);
}

char	open_par_exec(t_buf *b, void *m)
{
	track_add(&(((t_master*)m)->exec_tracks), (t_link*)link_track_create(NULL));
	//putendl("Added a track.");
	*prev_adr(m) = NOTHING;
	read_smart_inc(b);
	return (1);
}

void	extract_refined_expr(t_master *m, t_expr **value)
{
	t_expr		*traveler;
	t_content	c;

	*value = (t_expr*)(((t_link_track*)(m->exec_tracks.last))->first);
	if (((t_link_track*)(m->exec_tracks.last))->last == (t_link*)value)
		((t_link_track*)(m->exec_tracks.last))->first = NULL;
	else
	{
		traveler = (t_expr*)(((t_link_track*)(m->exec_tracks.last))->first)->next;
		if (traveler->info != PROCESSED)
		{
			c.expr = *value;
			*value = t_expr_init(c, PACK);
			while (traveler->next->info != PROCESSED)
				traveler = traveler->next;
			((t_link_track*)(m->exec_tracks.last))->first = (t_link*)traveler->next;
		}
		else
			((t_link_track*)(m->exec_tracks.last))->first = (t_link*)traveler;
		traveler->next = NULL;
	}
}

char	close_par_exec(t_buf *b, void *m)
{
	t_expr	*value;

	if (((t_master*)m)->exec_tracks.first == ((t_master*)m)->exec_tracks.last)
		handle_line_error(m, "Closing parenthesis without match detected.");
	if (condense_last_track(m) == 0)
	{
		handle_line_error(m, "The final addition of a track's components failed");
		return (1);
	}
	extract_refined_expr(m, &value);
	track_remove_last(&(((t_master*)m)->exec_tracks), destroy_link_track);
	//printf("prev now = %d\n", prev(m));
	if (prev(m) == MINUS || int_is_comprised(prev(m), MINUS_PLUS, MINUS_MODULO))
		reverse_expr(value);
	inject_expr(m, value);
	read_smart_inc(b);
	return (1);
}

char	prev(t_master *m)
{
	return (((t_cust_link_track*)(m->exec_tracks.last))->prev);
}

char	*prev_adr(t_master *m)
{
	return (&(((t_cust_link_track*)(m->exec_tracks.last))->prev));
}

int		condense_last_track(t_master *m)
{
	putendl("Condensing");
	if (((t_link_track*)(m->exec_tracks.last))->first == NULL)
		return (0);
	return (refine_addition_result((t_link_track*)(m->exec_tracks.last)));
}
