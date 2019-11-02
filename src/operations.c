/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operations.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: overetou <overetou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/29 14:30:31 by overetou          #+#    #+#             */
/*   Updated: 2019/11/02 19:08:41 by overetou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "computor.h"
#include <stdio.h>

//A simple value here means not a matrice and not a function.
BOOL	is_simple_value(char info)
{
	return (info == RATIONNAL || info == IRATIONNAL);
}

void	multiply_whole_pack(t_expr *pack, t_content v, char info)
{
	pack = pack->content.expr;
	while (pack)
	{
		if (info == RATIONNAL)
			pack->content.flt *= v.flt;
		else if (info == IRATIONNAL)
		{
			if (pack->info == RATIONNAL)
			{
				pack->content.flt *= v.flt;
				pack->info = IRATIONNAL;
			}
			else if (pack->info == IRATIONNAL)
			{
				pack->content.flt *= -(v.flt);
				pack->info = RATIONNAL;
			}
		}
		pack = pack->next;
	}
}

//WARNING: complex numbers are not supported yet.
void	do_multiplication(t_master *m, t_content value, char info)
{
	char	m_info;

	m_info = get_last_last_expr(m)->info;
	if (info == RATIONNAL)
	{
		if (m_info == RATIONNAL || m_info == IRATIONNAL)
			get_last_last_expr(m)->content.flt *= value.flt;
		else
			putendl("unkown operation case");
	}
	else if (info == IRATIONNAL)
	{
		if (m_info == RATIONNAL)
		{
			get_last_last_expr(m)->content.flt *= value.flt;
			get_last_last_expr(m)->info = IRATIONNAL;
		}
		else if (m_info == IRATIONNAL)
		{
			get_last_last_expr(m)->content.flt *= -value.flt;
			get_last_last_expr(m)->info = RATIONNAL;
		}
		else
			putendl("unkown operation case");
	}
	else if (info == PACK)
		multiply_whole_pack(get_last_last_expr(m), value, info);
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

char	addition_same_type(t_expr *m1, t_expr* m2)
{
	if (m1->info == RATIONNAL || m1->info == IRATIONNAL)
		m1->content.flt += m2->content.flt;
	m2->info = PROCESSED;
	return (1);
}

void	reverse_expr(t_expr *e)
{
	if (e->info == RATIONNAL || e->info == IRATIONNAL)
		e->content.flt = -(e->content.flt);
}

void	display_last_expr(t_master *m)
{
	t_expr	*e;
	char	printed;

	e = (t_expr*)(((t_link_track*)(m->exec_tracks.first))->first);
	printed = 0;
	while (e != (t_expr*)(((t_link_track*)(m->exec_tracks.first))->last))
	{
		if (e->info == PROCESSED)
			return ;
		if (printed)
		{
			putchr(' ');
			if ((e->info == RATIONNAL || e->info == IRATIONNAL) && e->content.flt < 0)
			{
				e->content.flt = -(e->content.flt);
				putstr("- ");
			}
			else
				putstr("+ ");
		}
		if (e->info == RATIONNAL)
			quick_put_float(e->content.flt);
		else if (e->info == IRATIONNAL)
		{
			if (e->content.flt > 1.00001 || e->content.flt < 0.9999)
				quick_put_float(e->content.flt);
			putchr('i');
		}
		printed = 1;
		e = e->next;
	}
	if (printed)
	{
		putchr(' ');
		if ((e->info == RATIONNAL || e->info == IRATIONNAL) && e->content.flt < 0)
		{
			e->content.flt = -(e->content.flt);
			putstr("- ");
		}
		else
			putstr("+ ");
	}
	if (e->info == RATIONNAL)
		quick_put_float(e->content.flt);
	else if (e->info == IRATIONNAL)
	{
		if (e->content.flt > 1.00001 || e->content.flt < 0.9999)
			quick_put_float(e->content.flt);
		putchr('i');
	}
}

t_expr*	get_last_last_expr(t_master *m)
{
	return ((t_expr*)(((t_link_track*)(m->exec_tracks.last))->last));
}
