/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parenthesis.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: overetou <overetou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/26 17:05:58 by overetou          #+#    #+#             */
/*   Updated: 2019/10/26 18:52:45 by overetou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "computor.h"
#include <stdio.h>

char	open_par_exec(t_buf *b, void *m)
{
	track_add(&(((t_master*)m)->exec_tracks), (t_link*)link_track_create(NULL));
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
