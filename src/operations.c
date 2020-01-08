/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operations.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: overetou <overetou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/29 14:30:31 by overetou          #+#    #+#             */
/*   Updated: 2019/11/18 19:16:58 by overetou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "computor.h"
#include <stdio.h>

int get_list_len(t_link *l)
{
	int count;

	count = 0;
	while (l)
	{
		count++;
		l = l->next;
	}
	return (count);
}

//A simple value here means not a matrice and not a function.
BOOL is_simple_value(char info)
{
	return (info == RATIONNAL || info == IRATIONNAL);
}

//Mutiply rationnal and ira between themself.
void simple_mult(t_expr *receiver, t_expr *op1, t_content op2, char op2info)
{
	//printf("simple_mult: %f * %f\n", op1->content.flt, op2.flt);
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
	printf("result = %f with type: %d\n", receiver->content.flt, receiver->info);
}

//We know that value is a pack.
void singl_mult_pack(t_expr *e, t_content value)
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

//e is the particul of a pack multiplication that was just calculated. C is a pack content that updates its content with e.
void dissolve_expr_in_content(t_content *c, t_expr *e)
{
	t_expr *head;

	if (c->expr == NULL)
	{
		putendl("dissolve_expr_in_content: initiating the receiving content.");
		c->expr = t_expr_init(e->content, e->info);
		((t_expr*)c->expr)->next = NULL;
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
				return;
			}
			head = head->next;
		}
		putendl("Found corresponding info on value to add and one already present in the future result.");
		head->content.flt += e->content.flt;
		//TODO: if the result is 0, delete the member?
		printf("updated value = %f\n", head->content.flt);
		////printf("dissolve_expr_in_content: augmented a value to %f\n", head->content.flt);
	}
}

void pack_mult_pack(t_expr *target_pack, t_content pack)
{
	t_content replace;
	t_expr *head1;
	t_expr *head2;
	t_expr temp;

	putendl("pack_mult_pack: entered.");
	head1 = (target_pack->content).expr;
	//head2 = pack.expr;//TO_DELETE
	//printf("content: pack1= %f, %f; pack2= %f, %f and adresses are equal is: %d\n", head1->content.flt, head1->next->content.flt, head2->content.flt, head2->next->content.flt, &(head1->content) == &(head2->content));
	replace.expr = NULL;
	while (head1)
	{
		head2 = pack.expr;
		while (head2)
		{
			simple_mult(&temp, head1, head2->content, head2->info);
			printf("temp value: %f, info: %d\n", temp.content.flt, temp.info);
			dissolve_expr_in_content(&replace, &temp);
			head2 = head2->next;
		}
		head1 = head1->next;
	}
	//TODO:	free_expr(head1);
	//TODO:	free_expr(head2);
	target_pack->content = replace;
	printf("Now target_pack content = %f, %f\n", ((t_expr*)(target_pack->content.expr))->content.flt, ((t_expr*)(target_pack->content.expr))->next->content.flt);
}

void apply_singl_mult(t_expr *pack, t_content c, char info)
{
	while (pack)
	{
		simple_mult(pack, pack, c, info);
		pack = pack->next;
	}
}

void multiply_pack_by(t_expr *pack, t_content c, char info)
{
	putendl("multiply_pack_by: entered.");
	if (info == IRATIONNAL || info == RATIONNAL)
		apply_singl_mult(pack, c, info);
	else if (info == PACK)
		pack_mult_pack(pack, c);
}

void	link_track_insert(t_link_track *t, void *link)
{
	if (t->first)
		link_track_add(t, link);
	else
		link_track_init(t, link);
}

//WARNING: complex numbers are not supported yet.
void do_multiplication(t_master *m, t_expr *e)
{
	char m_info;

	//putendl("\nWelcome in multiplication land !");
	m_info = get_last_last_expr(m)->info;
	if (m_info == RATIONNAL || m_info == IRATIONNAL)
	{
		multiply_unknowns(get_last_last_expr(m), e);
		if (e->info == RATIONNAL || e->info == IRATIONNAL)
			simple_mult(get_last_last_expr(m), get_last_last_expr(m), e->content, e->info);
		else if (e->info == PACK)
			singl_mult_pack(get_last_last_expr(m), e->content);
		else if (e->info == MATRIX)//TODO: handle the other way around.
		{
			t_expr *save;

			save = simple_mult_matrix(get_last_last_expr(m), e);
			link_track_remove_link((t_link_track*)(m->exec_tracks.last), (t_link*)get_last_last_expr(m));
			link_track_insert((t_link_track*)(m->exec_tracks.last), save);
		}
		else
			putendl("Unkown operation case");//TODO: add proper error management.
	}
	else if (m_info == PACK)
		multiply_pack_by(get_last_last_expr(m), e->content, e->info);
	else if (m_info == MATRIX)
	{
		if (e->info == MATRIX)
			term_by_term_matrix_mult(get_last_last_expr(m), e);
		else if (e->info != PACK)//TODO: add pack management.
			simple_mult_matrix(e, get_last_last_expr(m));
	}
	else
	{
		putendl("unkown operation case");
		return;
	}
	if (m_info == MATRIX || e->info != MATRIX)
		free(e);
}

void do_division(t_master *m, t_content value, char info)//todo: directly pass an expr to have unknowns on ra and ira too.
{
	t_expr *temp;

	temp = t_expr_init(value, info);
	prepare_diviser(temp);
	do_multiplication(m, temp);
}

void do_modulation(t_master *m, t_content value, char info)
{
	if (info == RATIONNAL)
		get_last_last_expr(m)->content.flt = (float)(((int)(get_last_last_expr(m)->content.flt)) % ((int)(value.flt))) + get_float_tail(get_last_last_expr(m)->content.flt);
	else
		putendl("unkown operation case");
}

void operate_matrix_point(t_expr *m1, t_expr *m2, int coord[2])
{
	t_expr *x1;
	t_expr *y1;
	t_expr *x2;
	t_expr *y2;
	int mem;
	int final_count;
	t_expr charge;
	float value;

	y1 = m1->content.expr;
	mem = coord[1];
	while (mem--)
		y1 = y1->next;
	x1 = y1->content.expr;
	y2 = m2->content.expr;
	final_count = get_list_len(((t_expr *)(m1->content.expr))->content.expr);
	value = 0;
	while (final_count--)
	{
		mem = coord[0];
		x2 = y2->content.expr;
		while (mem--)
			x2 = x2->next;
		simple_mult(&charge, x1, x2->content, x2->info);
		value += charge.content.flt;
		x1 = x1->next;
	}
	x1 = y1->content.expr;
	while (coord[0]--)
		x1 = x1->next;
	x1->content.flt = value;
}
/*
char matrix_multiplication(t_expr *m1, t_expr *m2)
{
	t_expr *x1;
	t_expr *y1;
	t_expr *x2;
	t_expr *y2;
	int coord[2];
	int count;

	y1 = m1->content.expr;
	x1 = y1->content.expr;
	y2 = m2->content.expr;
	x2 = y2->content.expr;
	if (get_list_len((t_link *)y1) != get_list_len((t_link *)x2) || get_list_len((t_link *)x1) != get_list_len((t_link *)y2))
		return (0);
	coord[0] = 0;
	coord[1] = 0;
	count = get_list_len((t_link *)y1) + get_list_len((t_link *)x1);
	while (count--)
	{
		operate_matrix_point(m1, m2, coord);
		(coord[1])++;
	}
	(coord[0])++;
	(coord[1])++;
	return (1);
}*/

char matrix_to_elem(t_expr *m1, t_expr *m2)
{
	t_expr *x;
	t_expr *y;

	if (m1->info != m2->info)
	{
		if (m1->info != MATRIX)
		{
			swap_pointer((void **)(&m1), (void *)(&m2));
		}
		y = m1->content.expr;
		while (y)
		{
			x = y->content.expr;
			while (x)
			{
				simple_mult(x, x, m2->content, m2->info);
				x = x->next;
			}
			y = y->next;
		}
	}
	return (1);
}

BOOL matrix_addition(t_expr *m1, t_expr *m2)
{
	t_expr *x1;
	t_expr *x2;

	putendl("entered matrix addition");
	m1 = m1->content.expr;
	m2 = m2->content.expr;
	while (m1)
	{
		if (!m2)
			return (0);
		x1 = m1->content.expr;
		x2 = m2->content.expr;
		while (x1)
		{
			if (!x2)
				return (0);
			addition_same_type(x1, x2);
			x1 = x1->next;
			x2 = x2->next;
		}
		m1 = m1->next;
		m2 = m2->next;
	}
	return (1);
}

char addition_same_type(t_expr *m1, t_expr *m2)
{
	if (m1->info == RATIONNAL || m1->info == IRATIONNAL)
		m1->content.flt += m2->content.flt;
	if (m1->info == MATRIX)
	{
		if (matrix_addition(m1, m2) == 0)
			return (0);
	}
	m2->info = PROCESSED;
	return (1);
}

t_expr *reverse_expr(t_expr *e)
{
	if (e->info == RATIONNAL || e->info == IRATIONNAL)
		e->content.flt = -(e->content.flt);
	else
		putendl("!!! Could not find suitable operation to reverse an expression.");
	return (e);
}

void	display_sub_matrix(t_expr *sub_mtrx, t_master *m)
{
	putchr('[');
	while (sub_mtrx)
	{
		display_expr(sub_mtrx, m, 0);
		if (sub_mtrx->next)
			putstr(", ");
		sub_mtrx = sub_mtrx->next;
	}
	putchr(']');
}

void display_matrix(t_expr *e, t_master *m)
{
	//putendl("entered display matrix");
	while (e)
	{
		//printf("first matrix member info:%d\n", x->info);
			//printf("matrix member info:%d\n", x->info);
		display_sub_matrix(e->content.expr, m);
		e = e->next;
		if (e)
			putchr('\n');
	}
}

void display_expr(t_expr *e, t_master *m, BOOL display_minus)
{
	//putendl("entered display expr\n");
	if (e->info == RATIONNAL)
		quick_put_float(e->content.flt, display_minus);
	else if (e->info == IRATIONNAL)
	{
		if (e->content.flt > 1.00001 || e->content.flt < 0.9999)
			quick_put_float(e->content.flt, display_minus);
		putchr('i');
	}
	else if (e->info == MATRIX)
	{
		display_matrix(e->content.expr, m);
	}
	else if (e->info == UNKNOWN)
	{
		putstr(m->to_define);
		if (e->content.integer > 1)
		{
			putchr('^');
			quick_putnb(e->content.integer, display_minus);
		}
	}
}

t_expr *get_last_first_expr(t_master *m)
{
	return ((t_expr *)(((t_link_track *)(m->exec_tracks.last))->first));
}

t_expr *get_last_last_expr(t_master *m)
{
	return ((t_expr *)(((t_link_track *)(m->exec_tracks.last))->last));
}

//This func will send back the fusion of the two. Use track_replace to make the most out of it.
t_expr	*simple_mult_matrix(t_expr *simple, t_expr *matrix)
{
	t_expr *head;
	t_expr *wire;

	wire = matrix->content.expr;
	printf("first arg info: %d, info2: %d\n", simple->info, matrix->info);
	while (wire)
	{
		head = wire->content.expr;
		while (head)
		{
			simple_mult(head, head, simple->content, simple->info);
			printf("inner mult result: %f\n", head->content.flt);
			head = head->next;
		}
		wire = wire->next;
	}
	putendl("At the end of simple_mult_matrix");
	return (matrix);
}

void	term_by_term_matrix_mult(t_expr *receiver, t_expr *multiplier)
{
	t_expr	*head1;
	t_expr	*head2;

	receiver = receiver->content.expr;
	multiplier = multiplier->content.expr;
	while (receiver)
	{
		head1 = receiver->content.expr;
		head2 = multiplier->content.expr;
		while (head1)
		{
			simple_mult(head1, head1, head2->content, head2->info);
			head1 = head1->next;
			head2 = head2->next;
		}
		receiver = receiver->next;
		multiplier = multiplier->next;
	}
}

t_expr	*create_result_matrix(size_t len)
{
	size_t	line;
	size_t	column;
	t_expr	*liner;
	t_expr	*columner;
	t_expr	*result;

	printf("create_result_matrix: len = %zu\n", len);
	result = t_expr_init((t_content)NULL, MATRIX);
	result->content.expr = t_expr_init((t_content)NULL, MATRIX);
	liner = result->content.expr;
	line = 1;
	while (1)
	{
		liner->content.expr = t_expr_init((t_content)0, RATIONNAL);
		putendl("\ncreated a matrix link");
		columner = liner->content.expr;
		column = 1;
		while (column != len)
		{
			columner->next = t_expr_init((t_content)0, RATIONNAL);
			putendl("created a matrix link");
			columner = columner->next;
			column++;
		}
		columner->next = NULL;
		putendl("Before break trial.");
		if (line == len)
		{
			putendl("break requested.\n");
			break;
		}
		liner->next = t_expr_init((t_content)NULL, MATRIX);
		liner = liner->next;
		line++;
	}
	liner->next = NULL;
	return (result);
}

void	cross_point_operation(t_expr **target, int column, t_expr *line_cell, t_expr *top_right_meta, t_master *m)
{
	t_expr	*columner;
	int		current_column;
	t_expr	*adder;
	t_expr	*to_delete;

	putendl("Entered cross_point_operation");

	add_level(m);
	while (top_right_meta)
	{
		current_column = 0;
		columner = top_right_meta->content.expr;
		while (current_column != column)
		{
			columner = columner->next;
			current_column++;
		}
		printf("mult value 1: %f, mult value 2: %f\n", line_cell->content.flt, columner->content.flt);
		adder = t_expr_init((t_content)NULL, RATIONNAL);
		simple_mult(adder, line_cell, columner->content, columner->info);
		mix_in_expr(m, adder);
		top_right_meta = top_right_meta->next;
		line_cell = line_cell->next;
	}
	refine_addition_result((t_link_track*)m->exec_tracks.last);
	//*target = (t_expr*)copy_expr((t_link*)get_last_first_expr(m));
	(*target)->content = get_last_first_expr(m)->content;
	(*target)->info = get_last_first_expr(m)->info;
	(*target)->unknown_degree = get_last_first_expr(m)->unknown_degree;
	if (((t_link_track*)(m->exec_tracks.last))->first == ((t_link_track*)(m->exec_tracks.last))->last)
	{
		((t_link_track*)(m->exec_tracks.last))->first = NULL;
		putendl("one element in the surcouche case.");
	}
	else
	{
		to_delete = get_last_first_expr(m);	
		((t_link_track*)(m->exec_tracks.last))->first = (t_link*)(to_delete->next);
		free(to_delete);
	}
	remove_level(m);
	//printf("result = %f\n", *target->content.flt);
}

//problem: il faut stocker les resultats hors de la matrice cible sinon le resultat sera de plus en plus faux.
BOOL	matrix_multiplication(t_master *m, t_expr *multiplier)
{
	if (get_last_last_expr(m)->info != MATRIX || multiplier->info != MATRIX)
	{putendl("Error, could be cared for before in double_star_exec. In part.");return (0);}

	t_expr	*receiver;
	t_expr	*head2;
	int		column;
	t_expr	*result_matrix;
	t_expr	*target;

	putendl("MATRIX MULTIPLICATION");
	receiver = get_last_last_expr(m)->content.expr;
	multiplier = multiplier->content.expr;
	result_matrix = create_result_matrix(get_list_len((t_link*)receiver));
	//printf("info1: %d, info2: %d, info3: %d\n", result_matrix->info, ((t_expr*)(result_matrix->content.expr))->info, ((t_expr*)(((t_expr*)(result_matrix->content.expr))->content.expr))->info);
	//display_matrix(result_matrix->content.expr, m);
	target = result_matrix->content.expr;
	while (target)
	{
		column = 0;
		head2 = target->content.expr;
		while (head2)
		{
			cross_point_operation(&head2, column, receiver->content.expr, multiplier, m);
			printf("value: %f, %d, %p\n", head2->content.flt, head2->info, head2->next);
			head2 = head2->next;
			column++;
		}
		receiver = receiver->next;
		target = target->next;
		putendl("receiver: loop end.");
	}
	//display_matrix(result_matrix->content.expr, m);
	link_track_remove_link((t_link_track*)(m->exec_tracks.last), (t_link*)get_last_last_expr(m));
	mix_in_expr(m, result_matrix);
	return (1);
}

void	invert_expr(t_expr *e)
{
	e->content.flt = 1 / e->content.flt;
	e->unknown_degree = -(e->unknown_degree);
}

//This func invert all invertable values and indictors of a content.
void	prepare_diviser(t_expr *e)
{
	t_expr	*head;

	if (e->info == PACK)
	{
		head = e->content.expr;
		invert_expr(head);
		head = head->next;
		invert_expr(head);
	}
	else
		invert_expr(e);
}