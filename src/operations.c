/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operations.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: overetou <overetou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/29 14:30:31 by overetou          #+#    #+#             */
/*   Updated: 2019/10/30 16:54:56 by overetou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "computor.h"
#include <stdio.h>

//A simple value here means not a matrice and not a function.
BOOL	is_simple_value(char info)
{
	return (info == RATIONNAL || info == IRATIONNAL);
}

//WARNING: complex numbers are not supported yet.
void	do_multiplication(t_master *m, t_content value, char info)
{
	if (info == RATIONNAL)
		get_last_last_expr(m)->content.flt *= value.flt;
	else
		putendl("unkown operation case");
}

void	do_division(t_master *m, t_content value, char info)
{
	if (info == RATIONNAL)
		get_last_last_expr(m)->content.flt /= value.flt;
	else
		putendl("unkown operation case");
}

void	do_modulation(t_master *m, t_content value, char info)
{
	if (info == RATIONNAL)
		get_last_last_expr(m)->content.flt = (float)(((int)(get_last_last_expr(m)->content.flt)) % ((int)(value.flt))) + get_float_tail(get_last_last_expr(m)->content.flt);
	else
		putendl("unkown operation case");
}

char	do_addition(t_expr *m1, t_expr* m2)
{
	float	safe;

	printf("Prepare to do some additions !\n");
	if (m1->info == RATIONNAL && m2->info == RATIONNAL)
	{
		safe = m1->content.flt;
		m1->content.flt += m2->content.flt;
		if (float_have_different_sign(safe, m1->content.flt))
			return (0);
	}
	else
	{
		putendl("unkown operation case");
		return (0);
	}
	return (1);
}

void	reverse_expr(t_expr *e)
{
	if (e->info == RATIONNAL || e->info == IRATIONNAL)
		e->content.flt = -(e->content.flt);
}

void	display_last_expr(t_master *m)
{
	t_content	c;
	char		info;

	c = ((t_expr*)(((t_link_track*)(m->exec_tracks.last))->first))->content;
	info = ((t_expr*)(((t_link_track*)(m->exec_tracks.last))->first))->info;
	if (info == RATIONNAL)
		quick_put_float(c.flt);
}

t_expr*	get_last_last_expr(t_master *m)
{
	return ((t_expr*)(((t_link_track*)(m->exec_tracks.last))->last));
}
