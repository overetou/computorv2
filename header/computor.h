/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   computor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: overetou <overetou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/16 15:18:32 by overetou          #+#    #+#             */
/*   Updated: 2019/10/16 18:03:51 by overetou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMPUTOR_H
# define COMPUTOR_H

typedef struct		s_expr
{
	struct s_expr	*next;
	void			*value;
	char			info;
}					t_expr;

typedef struct		s_var
{
	t_expr			*next;
	t_expr			*content;
	char			*name;
}					t_var;

typedef struct		s_master
{
	t_track			trigger_strings;
	t_track			trigger_funcs;
	t_track			exec_file;
	t_track			funcs;
	t_track			vars;
}					t_master;

void	store_expr(t_track *track, void *value);
t_expr	*create_expr(void *val);

#endif
