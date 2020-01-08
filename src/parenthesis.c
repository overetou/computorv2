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
	//putendl("add_level: adding a level.");
	track_add(&(m->exec_tracks), (t_link*)cust_link_track_create(NULL));
	(*prev_adr(m)) = NOTHING;
}

char	open_par_exec(t_buf *b, void *m)
{
	if (((t_master*)m)->equal_defined == DEFINE_FUNC)
		mix_in_expr(m, t_expr_init((t_content)NULL, PARENT_OPEN));
	else
		add_level(m);
	read_smart_inc(b);
	return (1);
}

t_expr *pack_init(t_expr *e)
{
	return (t_expr_init((t_content)((void*)e), PACK));
}

t_expr *pack_if_needed(t_master *m)
{
	t_expr *curr;
	t_expr *first;

	//putendl("PACK IF NEEDED");
	first = get_last_first_expr(m);
	if (first == get_last_last_expr(m) || first->next->info == PROCESSED)
	{
		((t_link_track*)(m->exec_tracks.last))->first = NULL;
		return (first);
	}
	curr = first;
	while (1)
	{
		if (curr->next->info == PROCESSED)
		{
			((t_link_track*)(m->exec_tracks.last))->first = (t_link*)curr->next;
			break;
		}
		curr = curr->next;
		//here we know that curr is not Processed.
		if (curr == get_last_last_expr(m)) //This condition makes sure that we never go past last.
		{
			((t_link_track*)(m->exec_tracks.last))->first = NULL;
			break;
		}
	}
	curr->next = NULL;
	////printf("new pack content = %f ; %f\n", first->content.flt, first->next->content.flt);
	return (pack_init(first));
}

void	do_close_par_manipulation(t_master *m)
{
	t_expr	*value;

	//putendl("entering do_close_par_manipulation");
	if (((t_master*)m)->exec_tracks.first == ((t_master*)m)->exec_tracks.last)
	{
		handle_line_error(m, "Closing parenthesis without match detected.");
		return;
	}
	if (condense_last_track(m) == 0)
	{
		handle_line_error(m, "The final addition of a track's components failed");
		return;
	}
	value = pack_if_needed(m);
	remove_level(m);
	//printf("value info that got out of the parent: %d. value = %f\n", value->info, value->content.flt);
	inject_expr(m, value);
}

char	close_par_exec(t_buf *b, void *m)
{
	if (((t_master*)m)->equal_defined == DEFINE_FUNC)
		mix_in_expr(m, t_expr_init((t_content)NULL, PARENT_CLOSE));
	else
		do_close_par_manipulation(m);
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
	//putendl("condense_last_track: refine_addition_result");
	return (refine_addition_result((t_link_track*)(m->exec_tracks.last)));
}