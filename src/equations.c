#include "computor.h"

BOOL    try_var_as_unknown(t_master *m, char *name)
{
    t_content content;

    if (m->to_define)
    {
        handle_line_error(m, "Too many unknown variables.");
        return (0);
    }
    m->to_define = name;
    content.flt = 1;
    inject_value(m, content, UNKNOWN);
}

void    multiply_anytype_by_unknown(t_expr *e1, t_expr *e2)
{
    if (e1->info == UNKNOWN)
        swap_pointer(&e1, &e2);//Now we have the garantee that e2 is an unkown
    e1->unknown_degree += e2->unknown_degree;
    e2->unknown_degree = 0;
}