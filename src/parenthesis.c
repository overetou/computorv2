/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parenthesis.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: overetou <overetou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/26 17:05:58 by overetou          #+#    #+#             */
/*   Updated: 2019/10/29 16:16:53 by overetou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "computor.h"
#include <stdio.h>

char	open_par_exec(t_buf *b, void *m)
{
	track_add(&(((t_master*)m)->exec_tracks), (t_link*)link_track_create(NULL));
	//putendl("Added a track.");
	*prev_adr(m) = NOTHING;
	read_smart_inc(b);
	return (1);
}

t_expr	*extract_refined_expr(t_master *m)
{
	t_expr* to_send;

	to_send = ((t_link_track*)(m->exec_tracks.last))->first;
	if (((t_link_track*)(m->exec_tracks.last))->last == to_send)
		((t_link_track*)(m->exec_tracks.last))->first = NULL;
	else
		((t_link_track*)(m->exec_tracks.last))->first = to_send->next;
	return (to_send);
}

char	close_par_exec(t_buf *b, void *m)
{
	t_expr	*value;

	if (((t_master*)m)->exec_tracks.first == ((t_master*)m)->exec_tracks.last)
		handle_line_error(m, "Closing parenthesis without match detected.");
	if (condense_last_track(m) == 0)
	{
		handle_line_error("The final addition of a track's components failed")
			return (1);
	}
	value = extract_refined_expr(m);
	track_remove_last(&(((t_master*)m)->exec_tracks), destroy_link_track);
	//printf("prev now = %d\n", prev(m));
	if (prev(m) == MINUS || int_is_comprised(prev(m), MINUS_PLUS, MINUS_MODULO))
		reverse_expr(value);
	inject_value(m, value);
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
	if (((t_link_track*)(m->exec_tracks.last))->first == NULL)
		return (0);
	return (refine_addition_result((t_link_track*)(m->exec_tracks.last)));
}
