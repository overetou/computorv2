#include "computor.h"

void	init_master(t_master *m)
{
	//focus buf on std input and refresh it.
	open_stdin(&(m->buf));
	//allocate trigger_funcs and strings.
	trigger_init((t_trigger_set*)m, "*", star_exec);
	trigger_add((t_trigger_set*)m, "+", plus_exec);
	trigger_add((t_trigger_set*)m, "-", minus_exec);
	trigger_add((t_trigger_set*)m, "/", div_exec);
	trigger_add((t_trigger_set*)m, "%", modulo_exec);
//	trigger_add((t_trigger_set*)m, "(", );
//	trigger_add((t_trigger_set*)m, ")", );
//	trigger_add((t_trigger_set*)m, "=", );
//	trigger_add((t_trigger_set*)m, "?", );
	trigger_add((t_trigger_set*)m, "\n", endline_exec);
	trigger_add((t_trigger_set*)m, "1234567890", num_store);
	trigger_add((t_trigger_set*)m, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz", alpha_exec);
	track_init(&(m->exec_tracks), (t_link*)create_void_simple());
	m->to_define = NULL;
	prepare_new_line(m);
}

int	main(void)
{
	t_master	m;
	
	init_master(&m);
	parse_and_trigger(&(m.buf), (t_trigger_set*)&m);
	return (0);
}
