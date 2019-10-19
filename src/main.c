#include "computor.h"

void	init_master(t_master *m)
{
	//focus buf on std input and refresh it.
	open_stdin(&(m->buf));
	//allocate trigger_funcs and strings.
	trigger_init(m, "*", star_exec);
	trigger_add(m, "+", plus_exec);
	trigger_add(m, "-", minus_exec);
	trigger_add(m, "/", div_exec);
	trigger_add(m, "%", modulo_exec);
//	trigger_add(m, "(", );
//	trigger_add(m, ")", );
//	trigger_add(m, "=", );
//	trigger_add(m, "?", );
	trigger_add(m, "\n", );
	trigger_add(m, "1234567890", num_store);
	trigger_add(m, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz", );
	track_init(&(m->exec_tracks), create_void_simple());
	m->to_define = NULL;
	prepare_new_line(m);
}

int	main(void)
{
	t_master	m;
	
	init_master(&m);
	parse_and_trigger(trigger_strings, &m);
	return (0);
}
