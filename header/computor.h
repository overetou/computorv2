/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   computor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: overetou <overetou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/16 15:18:32 by overetou          #+#    #+#             */
/*   Updated: 2019/10/23 19:28:25 by overetou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMPUTOR_H
# define COMPUTOR_H

//Those defines are used to know what the previous element encountered was.
//Operations preced by a MINUS_ are operations with the second element being negated.
#define NOTHING			0
#define PLUS			1
#define MINUS			2
#define MULT			3
#define DIV				4
#define MODULO			5
#define MINUS_PLUS		6
#define MINUS_MINUS		7
#define MINUS_MULT		8
#define MINUS_DIV		9
#define MINUS_MODULO	10
#define EQUAL			11
#define INTEROGATION	12
#define VALUE			13

#include "../../tools/libft/header/libft.h"

typedef union	u_content
{
	int			integ;
	UINT		uint;
	//t_matrice	*matr;
	float		*flt;
}				t_content;

typedef struct		s_expr
{
	struct s_expr	*next;
	t_content		content;
	char			*name;
	char			info;
}					t_expr;

typedef struct		s_var
{
	struct s_var	*next;
	t_content		content;
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
	char			*to_define;
}					t_master;

void	print_track_values(t_master *m);
void	store_expr(t_track *track, void *value);
void	prepare_new_line(t_master *m);
char	exec_cell_if_prior(t_master *m, int value);
int		get_addition_result(t_track *t);
void	mix_in_value(t_master *m, int value);
void	handle_line_error(t_master *m, const char *s);
char	plus_exec(t_buf *b, void *m);
char	num_store(t_buf *b, void *m);
char	star_exec(t_buf *b, void *m);
char	div_exec(t_buf *b, void *m);
char	modulo_exec(t_buf *b, void *m);
char	minus_exec(t_buf *b, void *m);
char	alpha_exec(t_buf *b, void *m);
char	equal_exec(t_buf *b, void *m);
char	endline_exec(t_buf *b, void *m);
#endif
