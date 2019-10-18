/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trigger_sign_funcs.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: overetou <overetou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/18 18:18:55 by overetou          #+#    #+#             */
/*   Updated: 2019/10/18 19:36:52 by overetou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "computor.h"

char	num_store(t_buf *b, t_master *m)
{
	if (m->prev == VALUE)
		handle_line_error(m, "Two values were consecutively defined.");//This func must read till endline, call prepare new line and putendl the given string.
	if (!read_int(b, t_simpl_content(m->exec_tracks->last))
		handle_line_error(m, "Integer overflow detected.");
	m->prev = VALUE;
}
