/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   computor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: overetou <overetou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/16 15:18:32 by overetou          #+#    #+#             */
/*   Updated: 2019/10/29 16:31:34 by overetou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMPUTOR_H
# define COMPUTOR_H

//Those defines are used to know what the previous element encountered was.
//Operations preced by a MINUS_ are operations with the second element being negated.
#define NOTHING			0
#define MINUS			1
#define PLUS			2
#define MULT			3
#define DIV				4
#define MODULO			5
#define MINUS_MINUS		6
#define MINUS_PLUS		7
#define MINUS_MULT		8
#define MINUS_DIV		9
#define MINUS_MODULO	10
#define EQUAL			11
#define INTEROGATION	12
#define VALUE			13

//The different values that info can take are:
#define RATIONNAL		0
//content is a simple float.
#define IRATIONNAL		1
//content is a float and we consider that it is multiplied by i.
#define MATRICE			2
//Content points on the first link matrice.
#define FUNCTION		3
//Content points on the first link of an expr list
#define UNKNOWN
//content is a float multplied by an unknown (used exclusivaly in functions)
#define IRA_UNKNOWN
//content is a float multplied by an unknown and i(used exclusivaly in functions)

#define EXEC_TRACK_LAST_AS_LINK_TRACK ((t_link_track*)(m->exec_tracks.last))

#include "../../tools/libft/header/libft.h"

typedef union	u_content
{
	float		*flt;
	//t_complex	*cpl;
	//t_matrice	*matr;
	//t_func	*func;
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

typedef struct  s_cust_link_track
{
    struct s_link_track *next;
    t_link      *first;
    t_link      *last;
	char		prev;
}               t_cust_link_track;

typedef struct		s_master
{
	t_track			trigger_strings;
	t_track			trigger_funcs;
	t_buf			buf;
	t_track			exec_tracks;
	t_track			funcs;
	t_track			vars;
	char			equal_defined;//0 for not defined, 1 for var assignment on left, 2 for right.
	char			*to_define;
}					t_master;

int		condense_last_track(t_master *m);
char	prev(t_master *m);
char	*prev_adr(t_master *m);
void	print_track_values(t_master *m);
void	store_expr(t_track *track, void *value);
void	prepare_new_line(t_master *m);
char	exec_cell_if_prior(t_master *m, int value);
BOOL	refine_addition_result(t_link_track *t);
void	inject_value(t_master *m, int value);
void	mix_in_value(t_master *m, int value);
void	handle_line_error(t_master *m, const char *s);
char	plus_exec(t_buf *b, void *m);
char	num_store(t_buf *b, void *m);
char	star_exec(t_buf *b, void *m);
char	div_exec(t_buf *b, void *m);
char	modulo_exec(t_buf *b, void *m);
char	minus_exec(t_buf *b, void *m);
char	alpha_exec(t_buf *b, void *m);
char	open_par_exec(t_buf *b, void *m);
char	close_par_exec(t_buf *b, void *m);
char	equal_exec(t_buf *b, void *m);
char	endline_exec(t_buf *b, void *m);
void	do_multiplication(t_master *m, t_content* value, char info);
void	do_division(t_master *m, t_content* value, char info);
void	do_modulation(t_master *m, t_content* value, char info);
void	reverse_expr(t_expr *e)
#endif
