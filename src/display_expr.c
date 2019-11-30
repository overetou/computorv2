#include "computor.h"

void display_last_expr(t_master *m)
{
	t_expr *e;
	char printed;

	e = get_last_first_expr(m);
	printed = 0;
	while (1)
	{
		if (e->info == PROCESSED)
        {
            putendl("\ndisplay_last_expr: Encountered PROCESSED. Not displaying more.");
			return;
        }
		if (printed)
		{
			putchr(' ');
			if ((e->info == RATIONNAL || e->info == IRATIONNAL) && e->content.flt < 0)
			{
				e->content.flt = -(e->content.flt);//Problem: this unwantedly alters the content of variables.
				putstr("- ");
				display_expr(e, m);
				e->content.flt = -(e->content.flt);
			}
			else
			{
				putstr("+ ");
				display_expr(e, m);
			}
		}
		else
		{
			display_expr(e, m);
		}
		
        if (e == get_last_last_expr(m))
        {
            putendl("\ndisplay_last_expr: last element was the last of the track.");
            return ;
        }
		printed = 1;
		e = e->next;
	}
}
