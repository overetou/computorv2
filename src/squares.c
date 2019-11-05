/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   squares.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: overetou <overetou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/05 17:39:48 by overetou          #+#    #+#             */
/*   Updated: 2019/11/05 21:07:12 by overetou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "computor.h"

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
		enable_matrice_mode(m);
		track_add(&(((t_master*)m)->exec_tracks), (t_link*)link_track_create(NULL));
	}
	else
	{
		if (prev(m) != COMA && prev(m) != NOTHING)
		{
			track_add(&(((t_master*)m)->exec_tracks), (t_link*)link_track_create(NULL));
			((t_master*)m)->matrice_depht++;

		}
		else
			handle_line_error(m, "Wrong matrix format.");
	}
}

void	extract_matrix(t_master *m, t_content *matrix)
{
	if (get_last_last_expr(m) == get_last_first_expr(m))
	{
		*matrix = get_last_first_expr(m);
		(m->exec_tracks.last)->first = NULL;
	}
	else
	{
		get_last_last_expr(m)->next = NULL;
		matrix->content.expr = get_last_first_expr(m);
		matrix->info = MATRIX;
		track_remove_last(&(((t_master*)m)->exec_tracks), destroy_link_track);
	}
}

char	close_square_exec(t_buf *b, void *m)
{
	t_content	matrix;

	if (((t_master*)m)->matrice_depht == 0)
	{
		handle_line_error(m, "Illegal closing bracket detected.");
	}
	else
	{
		if (get_last_first_expr(m) == NULL);
		{
			handle_line_error(m, "void matrix element detected.");
			return (1);
		}
		get_last_last_expr(m)->next = NULL;
		extract_matrix(m, &matrix);
		inject_expr(m, matrix);
		((t_master*)m)->matrice_depht--;
		if (((t_master*)m)->matrice_depht == 0)
			disable_matrice_mode(m);
	}
}

char	coma_exec(t_buf *b, void *m)
{
	if (((t_master*)m)->matrice_depht == 0)
		handle_line_error(m, "Illegal coma detected");
	else
	{
		prev_adr(m) = COMA;
		read_smart_inc(b);
	}
}

char	semilicon_exec(t_buf *b, void *m)
{
	if (((t_master*)m)->matrice_depht == 0 || prev(m) != MATRIX)
		handle_line_error(m, "Illegal semilicon detected");
	else
	{
		prev_adr(m) = SEMILICON;
		read_smart_inc(b);
	}
}
