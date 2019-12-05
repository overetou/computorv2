/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   squares.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: overetou <overetou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/05 17:39:48 by overetou          #+#    #+#             */
/*   Updated: 2019/11/15 18:40:37 by overetou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "computor.h"
#include <stdio.h>

char	illegal(t_buf *b, void *m)
{
	handle_line_error(m, "Illegal sign placement detected.");
	(void)b;
	return (1);
}

char	open_square_exec(t_buf *b, void *m)
{
	putendl("opening brackets detected");
	if (((t_master*)m)->matrice_depht == 0)
	{
		putendl("stepping up brackets depht to 1.");
		((t_master*)m)->matrice_depht = 1;
		add_level(m);
	}
	else
	{
		if ((prev(m) == SEMILICON || prev(m) == COMA || prev(m) == NOTHING) && ((t_master*)m)->matrice_depht == 1)
		{
			putendl("stepping up brackets depht to 2.");
			((t_master*)m)->matrice_depht++;
			add_level(m);
		}
		else
		{
			////printf("prev = %d\n", prev(m));
			handle_line_error(m, "Wrong matrix format.");
			return (1);
		}
	}
	read_smart_inc(b);
	return (1);
}

//void	extract_matrix(t_master *m, t_expr *matrix)
//{
//	matrix->info = MATRIX;
//	matrix->content.expr = get_last_first_expr(m);
//	((t_link_track*)(m->exec_tracks.last))->first = NULL;
//	remove_level(m);
//}

t_expr *extract_matrix_row(t_expr *first)
{
	return (t_expr_init((t_content)((void*)first), MATRIX_ROW));
}

char	close_square_exec(t_buf *b, void *m)
{
	t_expr	*new_elem;

	putendl("Closing bracket detected.");
	if (((t_master*)m)->matrice_depht == 2)
	{
		if (get_last_first_expr(m) == NULL)
			handle_line_error(m, "Void matrix element detected.");
		else
		{
			get_last_last_expr(m)->next = NULL; putendl("Ended a matrix row.");
			new_elem = extract_matrix_row(get_last_first_expr(m));
			printf("first element of the row = %f\n", get_last_first_expr(m)->content.flt);
			((t_link_track*)(((t_master*)m)->exec_tracks.last))->first = NULL;
		}
	}
	else if (((t_master*)m)->matrice_depht == 1)
	{
		get_last_last_expr(m)->next = NULL; putendl("Ended a matrix.");
		new_elem = t_expr_init((t_content)((void*)get_last_first_expr(m)), MATRIX);
		((t_link_track*)(((t_master*)m)->exec_tracks.last))->first = NULL;
	}
	else
	{
		handle_line_error(m, "Illegal closing bracket detected.");
		return (1);
	}
	remove_level(m);
	inject_expr(m, new_elem);
	(((t_master*)m)->matrice_depht)--;
	printf("brackets depht now equals: %d\n", ((t_master*)m)->matrice_depht);
	read_smart_inc(b);
	return (1);
}

char	virgule_exec(t_buf *b, void *m)
{
	putendl("virgule detected.");
	if (((t_master*)m)->matrice_depht != 2)
	{
		handle_line_error(m, "Illegal coma detected");
		return (0);
	}
	*(prev_adr(m)) = COMA;
	read_smart_inc(b);
	return (1);
}

char	pointvirgule_exec(t_buf *b, void *m)
{
	putendl("point virgule detected.");
	if (((t_master*)m)->matrice_depht != 1 || prev(m) != VALUE)
	{
		handle_line_error(m, "Illegal semilicon detected");
		return (0);
	}
	*(prev_adr(m)) = SEMILICON;
	read_smart_inc(b);
	return (1);
}
