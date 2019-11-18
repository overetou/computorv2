/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parenthesis.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: overetou <overetou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/26 17:05:58 by overetou          #+#    #+#             */
/*   Updated: 2019/11/18 19:07:13 by overetou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "computor.h"
#include <stdio.h>

BOOL	is_inside_parenthesis(t_master *m)
{
	return (m->exec_tracks.first != m->exec_tracks.last);
}

void	add_level(t_master *m)
{
	track_add(&(((t_master*)m)->exec_tracks), (t_link*)link_track_create(NULL));
	*prev_adr(m) = NOTHING;
}

char	open_par_exec(t_buf *b, void *m)
{
	add_level(m);
	read_smart_inc(b);
	return (1);
}

//t_expr	*p;
//p = get_last_last_expr(m);p = p->content.expr;
//while (p)
//{
//	printf("v: %f, t: %d\n", p->content.flt, p->info);
//	p = p->next;
//}

void	extract_refined_expr(t_master *m, t_expr **value)
{
	t_expr		*traveler;
	t_content	c;

	*value = (t_expr*)(((t_link_track*)(m->exec_tracks.last))->first);
	if (((t_link_track*)(m->exec_tracks.last))->last == (t_link*)value)
		((t_link_track*)(m->exec_tracks.last))->first = NULL;
	else
	{
		printf("extract_refined_expr: future pack detected.\n");
		traveler = ((t_expr*)((t_link_track*)(m->exec_tracks.last))->first);
		if (traveler->info != PROCESSED)
		{
			c.expr = *value;
			*value = t_expr_init(c, PACK);
			while (traveler->next != get_last_last_expr(m) &&
					traveler->next->info == PROCESSED)
			{
				traveler = traveler->next;
			}
			traveler = traveler->next;
			((t_link_track*)(m->exec_tracks.last))->first = (t_link*)traveler->next;
			traveler->next = NULL;
		}
		else
			((t_link_track*)(m->exec_tracks.last))->first = (t_link*)traveler;
	}
}

char	close_par_exec(t_buf *b, void *m)
{
	t_expr	*value;

	putendl("!!!!!!!!!!!!!!!!!!!!!!Close par");
	if (((t_master*)m)->exec_tracks.first == ((t_master*)m)->exec_tracks.last)
		handle_line_error(m, "Closing parenthesis without match detected.");
	if (condense_last_track(m) == 0)
	{
		handle_line_error(m, "The final addition of a track's components failed");
		return (1);
	}
	printf("close_par_exec: condensing succeded\n");
	extract_refined_expr(m, &value);
	printf("close_par_exec: refinement succeded\n");
	track_remove_last(&(((t_master*)m)->exec_tracks), destroy_link_track);
	printf("close_par_exec: Lowered by a level. Previous is now: %d\n", prev(m));
	if (prev(m) == MINUS || int_is_comprised(prev(m), MINUS_PLUS, MINUS_MODULO))
		reverse_expr(value);
	printf("close_par_exec: trying to inject an expr of type: %d\n", value->info);
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
	if (((t_link_track*)(m->exec_tracks.last))->first == NULL)
		return (0);
	return (refine_addition_result((t_link_track*)(m->exec_tracks.last)));
}
