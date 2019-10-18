/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_tracks.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: overetou <overetou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/18 17:28:12 by overetou          #+#    #+#             */
/*   Updated: 2019/10/18 20:00:34 by overetou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "computor.h"

void	init_expr(t_simple *e)
{
	e->content = (void*)1;
}

void	prepare_new_line(t_master *m)
{
	destroy_track_from_to(m->exec_tracks->first->next, m->exec_tracks->last);
	m->exec_tracks->last = m->exec_tracks->first;
	m->prev = NOTHING;
	m->equal_defined = 0;
	init_expr(m->exec_tracks->first);
}

void	handle_line_error(t_master *m, const char *s)
{
	read_go_next_line(m->buf);
	prepare_new_line(m);
	putendl(s);
}
