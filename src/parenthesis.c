/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parenthesis.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: overetou <overetou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/26 17:05:58 by overetou          #+#    #+#             */
/*   Updated: 2019/10/26 17:43:03 by overetou         ###   ########.fr       */
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
	value = CONDENSE_LAST_TRACK;
	track_remove_last(&(((t_master*)m)->exec_tracks), destroy_link_track);
	inject_value(m, value);
	read_smart_inc(b);
	return (1);
}
