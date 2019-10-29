/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operations.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: overetou <overetou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/29 14:30:31 by overetou          #+#    #+#             */
/*   Updated: 2019/10/29 16:19:33 by overetou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "computor.h"

//A simple value here means not a matrice and not a function.
BOOL	is_simple_value(char info)
{
	return (info == RATIONNAL || info == IRATIONNAL);
}

//WARNING: complex numbers are not supported yet.
void	do_multiplication(t_master *m, t_content* value, char info)
{
	if (m->info == RATIONNAL && info == RATIONNAL)
		*(get_last_value_adr(m)) *= value->flt;
	else
		putendl("unkown operation case");
}

void	do_division(t_master *m, t_content* value, char info)
{
	if (m->info == RATIONNAL && info == RATIONNAL)
		*(get_last_value_adr(m)) /= value->flt;
	else
		putendl("unkown operation case");
}

void	do_modulation(t_master *m, t_content* value, char info)
{
	if (m->info == RATIONNAL && info == RATIONNAL)
		*(get_last_value_adr(m)) = (float)(((int)(*(get_last_value_adr(m)))) % ((int)(value->flt))) + get_float_tail(*(get_last_value_adr(m)));
	else
		putendl("unkown operation case");
}

char	do_addition(t_expr *m1, t_expr* m2)
{
	float	safe;

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
