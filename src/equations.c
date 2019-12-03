#include "computor.h"

BOOL try_var_as_unknown(t_master *m, char *name)
{
	t_content content;
	t_expr		*e;

	if (m->to_define)
	{
		if (str_perfect_match(m->to_define, name) == 0)
		{
			handle_line_error(m, "Too many unknown variables.");
			return (0);
		}
	}
	m->to_define = name;
	content.flt = 1;
	e = t_expr_init(content, UNKNOWN);
	e->unknown_degree = 1;
	//TODO: add power handlement.
	inject_expr(m, e);
	////printf("Injected: %s as an unkown candidate.\n", name);
	return (1);
}

void multiply_unknowns(t_expr *e1, t_expr *e2)
{
	e1->unknown_degree += e2->unknown_degree;
	////printf("multiply_unknowns: %f * x^%zu\n", e1->content.flt, e1->unknown_degree);
}

char equal_exec(t_buf *b, void *m)
{
	if (((t_master *)m)->equal_defined)
		handle_line_error(m, "At least two '=' are present on the line.");
	((t_master *)m)->equal_defined = SIMPLE_EQUAL;
	add_level(m);
	*(prev_adr(m)) = EQUAL;
	read_smart_inc(b);
	return (1);
}

BOOL pass_right_to_left(t_master *m)
{
	t_expr *e;
	t_expr *next;
	t_expr *last;

	e = get_last_first_expr(m);
	next = e->next;
	last = get_last_last_expr(m);
	((t_link_track *)(m->exec_tracks.last))->first = NULL;
	remove_level(m);
	while (e != last)
	{
		if (e->info == MATRIX)
		{
			//TODO: free the rest of the line.
			return (0);
		}
		mix_in_expr(m, reverse_expr(e)); //Does this function free e? If not free it there.
		e = next;
		next = next->next;
	}
	mix_in_expr(m, reverse_expr(e)); //Does this function free e? If not free it there.
	return (1);
}

void	display_equation_solution(t_master *m)
{
	//resolve: We now have a group of expression of different degree. we classify them into a float tab.
	float	candidates[3];
	float	solutions[2];
	int		sol_nb;
	t_expr	*e;
	size_t	degree;

	e = get_last_first_expr(m);
	degree = 0;
	while (1)
	{
		////printf("display_equation_solution: %f * x^%zu\n", e->content.flt, e->unknown_degree);
		candidates[e->unknown_degree] = e->content.flt;
		update_if_superior(&degree, e->unknown_degree);
		if (e == get_last_last_expr(m) || e->next->info == PROCESSED)
			break;
		e = e->next;
	}
	//display
	sol_nb = do_resolution(candidates, degree + 1, solutions);
	display_solutions(solutions, sol_nb);
}

char interogation_exec(t_buf *b, void *m)
{
	//this symbol must end a line.
	putendl("\nInterrogation exec!");
	if (read_smart_inc(b) == 0 || b->str[b->pos] == '\n')
	{
		if (((t_master *)m)->equal_defined == SIMPLE_EQUAL)
		{
			//We want to inject every expression of the right side (current level) to the right side and revert them when doing so. If a matrix is encountered, line error.
			//Note: this function lower the level by a floor.
			if (pass_right_to_left(m) == 0)
				return (1);
			//We condense the total expression.
			putendl("interogation_exec: refine_addition_result");
			refine_addition_result((t_link_track *)(((t_master *)m)->exec_tracks.first));
			//Now the expression is simplified. We send the data to the equation resolving module trough a wrapper that will also display the solution / error.
			display_equation_solution(m);
			//Then we send the commands back to the user.
			prepare_new_line(m);
			read_smart_inc(b);
		}
		else
			handle_line_error(m, "Interogation point placed in an incoherent context.");
	}
	else
		handle_line_error(m, "Interrogation point followed by a symbol.");
	return (1);
}