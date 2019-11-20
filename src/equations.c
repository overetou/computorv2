#include "computor.h"

BOOL    try_var_as_unknown(t_master *m, char *name)
{
    if (m->to_define)
    {
        handle_line_error(m, "Too many unknown variables.");
        return (0);
    }
    m->to_define = name;
    inject_value(m, (t_content)NULL, UNKNOWN);
}

//operation cases with unknowns.