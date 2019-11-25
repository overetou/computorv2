/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   computor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: overetou <overetou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/16 15:18:32 by overetou          #+#    #+#             */
/*   Updated: 2019/11/19 21:05:34 by overetou         ###   ########.fr       */
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
#define POWER			14
#define COMA			15
#define SEMILICON		16
#define MATRIX			17

#define PROCESSED		-1
//The different values that info can take are:
#define RATIONNAL		0
//content is a simple float.
#define IRATIONNAL		1
//content is a float and we consider that it is multiplied by i.
#define MATRICE			2
//Content points on the first link matrice.
#define FUNCTION		3
//Content points on the first link of an expr list
#define UNKNOWN			4
//content is a float multplied by an unknown (used exclusivaly in functions)
#define IRA_UNKNOWN		5
//content is a float multplied by an unknown and i(used exclusivaly in functions)
#define PACK			6
//content is part of a set of 2 values: a and b in a + bi.

#define DEFINE_VAR 1
#define DEFINE_FUNC 2
#define SIMPLE_EQUAL 3

#define EXEC_TRACK_LAST_AS_LINK_TRACK ((t_link_track*)(m->exec_tracks.last))

#include "../../tools/libft/header/libft.h"

typedef union	u_content
{
	float		flt;
	void		*expr;
	//t_matrice	*matr;
	//t_func	*func;
}				t_content;

typedef struct		s_expr
{
	struct s_expr	*next;
	t_content		content;
	char			*name;
	char			info;
	size_t			unknown_degree;
}					t_expr;

typedef struct		s_var
{
	struct s_var	*next;
	t_content		content;
	char			*name;
	char			info;
}					t_var;

typedef struct		s_func
{
	struct s_func	*next;
	char			*name;
	t_track			expr;
}					t_func;

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
	int				matrice_depht;
}					t_master;

int		condense_last_track(t_master *m);
char	prev(t_master *m);
char	*prev_adr(t_master *m);
void	print_track_values(t_master *m);
void	store_expr(t_track *track, void *value);
void	prepare_new_line(t_master *m);
char	exec_cell_if_prior(t_master *m, t_content value, char info);
BOOL	refine_addition_result(t_link_track *t);
void	inject_value(t_master *m, t_content content, char info);
void	inject_expr(t_master *m, t_expr *e);
void	mix_in_value(t_master *m, t_content content, char info);
BOOL	handle_line_error(t_master *m, const char *s);
char	plus_exec(t_buf *b, void *m);
char	num_store(t_buf *b, void *m);
char	star_exec(t_buf *b, void *m);
char	div_exec(t_buf *b, void *m);
char	modulo_exec(t_buf *b, void *m);
char	power_exec(t_buf *b, void *m);
char	minus_exec(t_buf *b, void *m);
char	alpha_exec(t_buf *b, void *m);
char	open_par_exec(t_buf *b, void *m);
char	close_par_exec(t_buf *b, void *m);
char	equal_exec(t_buf *b, void *m);
char	endline_exec(t_buf *b, void *m);
void	do_multiplication(t_master *m, t_content value, char info);
void	do_division(t_master *m, t_content value, char info);
void	do_modulation(t_master *m, t_content value, char info);
char	addition_same_type(t_expr *m1, t_expr* m2);
t_expr*	reverse_expr(t_expr *e);
BOOL	is_inside_parenthesis(t_master *m);
void	display_last_expr(t_master *m);
t_expr*	get_last_last_expr(t_master *m);
t_expr*	get_last_first_expr(t_master *m);
BOOL	is_sep(const char c);
void	*t_expr_create(void);
void	*t_expr_init(t_content content, char info);
char	close_square_exec(t_buf *b, void *m);
char	open_square_exec(t_buf *b, void *m);
char	semilicon_exec(t_buf *b, void *m);
char	coma_exec(t_buf *b, void *m);
void	add_level(t_master *m);
BOOL	char_is_valid_var_name_material(const char c);
char	handle_func(t_master *m, t_buf *b, char *s);
void	remove_level(t_master *m);
void	*get_item(t_track *t, const char *name);
void	display_expr(t_expr *e, t_master *m);
void	*get_var(t_master *m, const char *name);
char	interogation_exec(t_buf *b, void *m);
BOOL try_var_as_unknown(t_master *m, char *name);
#endif
