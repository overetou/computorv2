/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parenthesis.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: overetou <overetou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/26 17:05:58 by overetou          #+#    #+#             */
/*   Updated: 2019/10/28 14:33:26 by overetou         ###   ########.fr       */
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

char	close_par_exec(t_buf *b, void *m)
{
	int	value;

	if (((t_master*)m)->exec_tracks.first == ((t_master*)m)->exec_tracks.last)
		handle_line_error(m, "Closing parenthesis without match detected.");
	value = condense_last_track(m);
	track_remove_last(&(((t_master*)m)->exec_tracks), destroy_link_track);
	//printf("prev now = %d\n", prev(m));
	if (prev(m) == MINUS || int_is_comprised(prev(m), MINUS_PLUS, MINUS_MODULO))
		value = -value;
	//printf("Parenthesis closed. Contained value = %d\n", value);
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
	return (get_addition_result((t_link_track*)(m->exec_tracks.last)));
}
