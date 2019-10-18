/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   computor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: overetou <overetou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/16 15:18:32 by overetou          #+#    #+#             */
/*   Updated: 2019/10/18 17:56:06 by overetou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMPUTOR_H
# define COMPUTOR_H

#define NOTHING 0

typedef struct		s_var
{
	t_expr			*next;
	void			*value;
	char			*name;
	char			info;
}					t_var;

typedef struct		s_master
{
	t_track			trigger_strings;
	t_track			trigger_funcs;
	t_buf			buf;
	t_track			exec_tracks;
	t_track			funcs;
	t_track			vars;
	char			prev;
	char			equal_defined;
}					t_master;

void	store_expr(t_track *track, void *value);
t_expr	*create_expr(void *val);

#endif
