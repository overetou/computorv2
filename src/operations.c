/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operations.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: overetou <overetou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/29 14:30:31 by overetou          #+#    #+#             */
/*   Updated: 2019/11/05 21:15:25 by overetou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "computor.h"
#include <stdio.h>

//A simple value here means not a matrice and not a function.
BOOL	is_simple_value(char info)
{
	return (info == RATIONNAL || info == IRATIONNAL);
}

//Mutiply rationnal and ira between themself.
void	simple_mult(t_expr *receiver, t_expr *op1, t_content op2, char op2info)
{
	printf("simple_mult: %f * %f\n", op1->content.flt, op2.flt);
	receiver->content.flt = (op1->content.flt) * (op2.flt);
	if (op1->info == RATIONNAL)
	{
		if (op2info == IRATIONNAL)
			receiver->info = IRATIONNAL;
		else
			receiver->info = RATIONNAL;
	}
	else if (op1->info == IRATIONNAL)
	{
		if (op2info == IRATIONNAL)
		{
			receiver->info = RATIONNAL;
			receiver->content.flt *= -1;
		}
		else
			receiver->info = IRATIONNAL;
	}
//	printf("result = %f with type: %d\n", receiver->content.flt, receiver->info);
}

//We know that value is a pack.
void	singl_mult_pack(t_expr *e, t_content value)
{
	t_expr *head;

	head = value.expr;
	while (head != NULL)
	{
		simple_mult(head, head, e->content, e->info);
		head = head->next;
	}
	e->content = value;
	e->info = PACK;
}

void	dissolve_expr_in_content(t_content *c, t_expr *e)
{
	t_expr *head;

	if (c->expr == NULL)
	{
		putendl("dissolve_expr_in_content: init pack");
		c->expr = t_expr_init(e->content, e->info);
	}
	else
	{
		head = c->expr;
		while (head->info != e->info)
		{
			if (head->next == NULL)
			{
				head->next = t_expr_init(e->content, e->info);
				head->next->next = NULL;
				return ;
			}
			head = head->next;
		}
		head->content.flt += e->content.flt;
		printf("dissolve_expr_in_content: augmented a value to %f\n", head->content.flt);
	}
}

void	pack_mult_pack(t_expr *target_pack, t_content pack)
{
	t_content	replace;
	t_expr		*head1;
	t_expr 		*head2;
	t_expr		temp;

	putendl("pack_mult_pack: entered.");
	head1 = (target_pack->content).expr;
	replace.expr = NULL;
	while (head1)
	{
		head2 = pack.expr;
		while (head2)
		{
			simple_mult(&temp, head1, head2->content, head2->info);
			dissolve_expr_in_content(&replace, &temp);
			head2 = head2->next;
		}
		head1 = head1->next;
	}
//	free_expr(head1);
//	free_expr(head2);
	target_pack->content = replace;
}

void	apply_singl_mult(t_expr *pack, t_content c, char info)
{
	while (pack)
	{
		simple_mult(pack, pack, c, info);
		pack = pack->next;
	}
}

void	multiply_pack_by(t_expr *pack, t_content c, char info)
{
	putendl("multiply_pack_by: entered.");
	if (info == IRATIONNAL || info == RATIONNAL)
		apply_singl_mult(pack, c, info);
	else if (info == PACK)
		pack_mult_pack(pack, c);
}

//WARNING: complex numbers are not supported yet.
void	do_multiplication(t_master *m, t_content value, char info)
{
	char	m_info;

	putendl("\nWelcome in multiplication land !");
	m_info = get_last_last_expr(m)->info;
	if (m_info == RATIONNAL || m_info == IRATIONNAL)
	{
		if (info == RATIONNAL || info == IRATIONNAL)
			simple_mult(get_last_last_expr(m), get_last_last_expr(m), value, info);
		else if (info == PACK)
			singl_mult_pack(get_last_last_expr(m), value);
	}
	else if (m_info == PACK)
		multiply_pack_by(get_last_last_expr(m), value, info);
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

char	matrix_addition(t_expr *m1, t_expr* m2)
{

}

char	addition_same_type(t_expr *m1, t_expr* m2)
{
	if (m1->info == RATIONNAL || m1->info == IRATIONNAL)
		m1->content.flt += m2->content.flt;
	if (m1->info == MATRIX)
		return (matrix_addition(m1, m2)));
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

t_expr*	get_last_first_expr(t_master *m)
{
	return ((t_expr*)(((t_link_track*)(m->exec_tracks.last))->first));
}

t_expr*	get_last_last_expr(t_master *m)
{
	return ((t_expr*)(((t_link_track*)(m->exec_tracks.last))->last));
}
