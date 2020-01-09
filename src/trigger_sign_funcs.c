/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trigger_sign_funcs.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: overetou <overetou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/18 18:18:55 by overetou          #+#    #+#             */
/*   Updated: 2019/11/18 21:20:20 by overetou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "computor.h"
#include <stdio.h>

BOOL	fetch_power_data(t_buf *b, int *power)
{
	read_till_false(b, is_sep);
	if (is_digit(b->str[b->pos]))
		return (read_int(b, power));
	return (0);
}

BOOL	power_ira(t_content *c, char *info, int power)
{
	t_expr	temp;

	if (power == 0)
	{
		(*c).flt = 1;
		*info = RATIONNAL;
		return (1);
	}
	temp.content = *c;
	temp.info = *info;
	while (--power)
		simple_mult(&temp, &temp, *c, *info);
	*c = temp.content;
	*info = temp.info;
	return (1);
}

BOOL	power_pack(t_content *c, char *info, int power)
{
	t_content temp;
	t_expr	actual_expr;

	if (power == 0)
	{
		//TODO: free the content before changing it.
		(*c).flt = 1;
		*info = RATIONNAL;
		return (1);
	}
	temp.expr = create_list_copy(c->expr, copy_expr);
	actual_expr.content = *c;
	actual_expr.info = *info;
	while (--power)
		pack_mult_pack(&actual_expr, temp);
	//Todo: free the copy of the pack created for the occasion.
	*c = actual_expr.content;
	return (1);
}

BOOL	apply_power(t_content *c, char *info, int power)
{
	if (*info == RATIONNAL)
		return (float_simple_power(&(c->flt), power));
	else if (*info == IRATIONNAL)
		return(power_ira(c, info, power));
	else if (*info == PACK)
	{
		putendl("inside power pack.");
		return (power_pack(c, info, power));
	}
	
	return (0);
}

BOOL	next_sign_is_power(t_buf *b)
{
	read_till_false(b, is_sep);
	return (b->str[b->pos] == '^');
}

BOOL	get_power(t_buf *b, t_content *c, char *info)
{
	int	power;

//	putendl("get_power: Entered.");
	//printf("preparing to read a potential power. Current char = %c\n", b->str[b->pos]);
	if (next_sign_is_power(b))
	{
		read_smart_inc(b);
		power = 1;
		if (fetch_power_data(b, &power) == 0)
			return (0);
		////printf("get_power: power = %d\n", power);
		return (apply_power(c, info, power));
	}
	return (1);
}

char	num_store(t_buf *b, void *m)
{
	t_content	value;
	char		info;
	
	if (prev(m) == VALUE)
	{
		handle_line_error(m, "Two values were consecutively defined.");
		return (1);
	}
	info = RATIONNAL;
	value.flt = 1;
	if (!read_float(b, &(value.flt)))
	{
		handle_line_error(m, "Problem while parsing a number.");
		return (1);
	}
//	if (get_power(b, &value, &info) == 0)
//	{
//		handle_line_error(m, "Problem while parsing a power detected.");
//		return (1);
//	}
	//printf("num_store: read number = %f\n", value.flt);
	if (((t_master*)m)->equal_defined == DEFINE_FUNC)
	{
		putendl("////////////\nDirectly adding.\n///////////");
		mix_in_expr(m, t_expr_init(value, info));
		*(prev_adr(m)) = VALUE;
	}
	else
		inject_value(m, value, info, 1);
	printf("numstore: last = %f\n", get_last_last_expr(m)->content.flt);
	return (1);
}

void	double_star_exec(t_buf *b, void *m)
{
	putendl("!!!!!!Entered double star exec.");
	read_smart_inc(b);//TODO: add error management if the increase fails.
	*(prev_adr(m)) = DOUBLE_MULT;
	if (((t_master*)m)->equal_defined == DEFINE_FUNC)
	{
		putendl("mult added in the function definition.");
		mix_in_expr(m, t_expr_init((t_content)NULL, DOUBLE_MULT));
	}
}

char	star_exec(t_buf *b, void *m)
{
	//putendl("Entered star exec.");
	if (prev(m) != VALUE)
		handle_line_error(m, "Expected a value before a '*'.");
	read_smart_inc(b);//TODO: add error management if the increase fails.
	if (b->str[b->pos] == '*')
		double_star_exec(b, m);
	else
	{
		*(prev_adr(m)) = MULT;
		if (((t_master*)m)->equal_defined == DEFINE_FUNC)
		{
			//putendl("mult added in the function definition.");
			mix_in_expr(m, t_expr_init((t_content)NULL, MULT));
		}
	}
	return (1);
}

char	div_exec(t_buf *b, void *m)
{
	if (prev(m) != VALUE)
	{
		handle_line_error(m, "No value found behind '/'.");
		return (1);
	}
	*(prev_adr(m)) = DIV;
	read_smart_inc(b);
	if (((t_master*)m)->equal_defined == DEFINE_FUNC)
		mix_in_expr(m, t_expr_init((t_content)NULL, DIV));
	return (1);
}

char	modulo_exec(t_buf *b, void *m)
{
	if (prev(m) != VALUE)
	{
		handle_line_error(m, "No value found behind '%'.");
		return (1);
	}
	*(prev_adr(m)) = MODULO;
	read_smart_inc(b);
	if (((t_master*)m)->equal_defined == DEFINE_FUNC)
		mix_in_expr(m, t_expr_init((t_content)NULL, MODULO));
	return (1);
}

char	power_exec(t_buf *b, void *m)
{
	(void)b;
	handle_line_error(m, "A '^' was found in a strange place.");
//	if (((t_master*)m)->equal_defined == DEFINE_FUNC)
//		mix_in_expr(m, t_expr_init((t_content)NULL, POWER));
	return (1);
}

char	minus_exec(t_buf *b, void *m)
{
	if (int_is_comprised(prev(m), MINUS, MODULO))
		*(prev_adr(m)) += 5;
	else if (prev(m) == NOTHING || prev(m) == VALUE || prev(m) == EQUAL)
	{
		//putendl("prev = minus.");
		*(prev_adr(m)) = MINUS;
	}
	else
	{
		handle_line_error(m, "'-' was not preceded by a compatible element.");
		return (1);
	}
	read_smart_inc(b);
	if (((t_master*)m)->equal_defined == DEFINE_FUNC)
		mix_in_expr(m, t_expr_init((t_content)NULL, MINUS));
	return (1);
}

char	plus_exec(t_buf *b, void *m)
{
	if (prev(m) != VALUE)
	{
		handle_line_error(m, "'+' was not preceded by a value.");
		return (1);
	}
	else
		*(prev_adr(m)) = PLUS;
	read_smart_inc(b);
	if (((t_master*)m)->equal_defined == DEFINE_FUNC)
		mix_in_expr(m, t_expr_init((t_content)NULL, PLUS));
	return (1);
}
