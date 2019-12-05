#include "computor.h"

t_cust_link_track *cust_link_track_create(t_link *l)
{
    t_cust_link_track *new;

    new = malloc(sizeof(t_cust_link_track));
    link_track_init((t_link_track*)new, l);
    new->prev = NOTHING;
    return (new);
}