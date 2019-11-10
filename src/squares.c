/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   squares.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: overetou <overetou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/05 17:39:48 by overetou          #+#    #+#             */
/*   Updated: 2019/11/10 18:07:32 by overetou         ###   ########.fr       */
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
	if (((t_master*)m)->matrice_depht == 0)
	{
		((t_master*)m)->matrice_depht = 1;
		add_level(m);
	}
	else
	{
		if ((prev(m) == SEMILICON || prev(m) == COMA || prev(m) == NOTHING) && ((t_master*)m)->matrice_depht == 1)
		{
			add_level(m);
			((t_master*)m)->matrice_depht++;
		}
		else
		{
			printf("prev = %d\n", prev(m));
			handle_line_error(m, "Wrong matrix format.");
			return (1);
		}
	}
	read_smart_inc(b);
	return (1);
}

void	extract_matrix(t_master *m, t_expr *matrix)
{
	matrix->info = MATRIX;
	matrix->content.expr = get_last_first_expr(m);
	((t_link_track*)(m->exec_tracks.last))->first = NULL;
	track_remove_last(&(((t_master*)m)->exec_tracks), destroy_link_track);
}

char	close_square_exec(t_buf *b, void *m)
{
	t_expr	matrix;

	putendl("Closing bracket detected.");
	if (((t_master*)m)->matrice_depht == 0)
		handle_line_error(m, "Illegal closing bracket detected.");
	else
	{
		if (get_last_first_expr(m) == NULL)
			handle_line_error(m, "Void matrix element detected.");
		else
		{
			get_last_last_expr(m)->next = NULL;
			putendl("Ended a matrix row.");
			extract_matrix(m, &matrix);
			inject_expr(m, &matrix);
			((t_master*)m)->matrice_depht--;
			read_smart_inc(b);
		}
	}
	return (1);
}

char	coma_exec(t_buf *b, void *m)
{
	if (((t_master*)m)->matrice_depht == 0)
		handle_line_error(m, "Illegal coma detected");
	else
	{
		*(prev_adr(m)) = COMA;
		read_smart_inc(b);
	}
	return (1);
}

char	semilicon_exec(t_buf *b, void *m)
{
	if (((t_master*)m)->matrice_depht != 1 || prev(m) != VALUE)
		handle_line_error(m, "Illegal semilicon detected");
	else
	{
		*(prev_adr(m)) = SEMILICON;
		read_smart_inc(b);
	}
	return (1);
}
