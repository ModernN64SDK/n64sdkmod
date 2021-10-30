/*======================================================================*/
/*	Mirror House							*/
/*	om_common.c							*/
/*									*/
/*	Copyright (C) 1998 MONEGI CORPORATION				*/
/*									*/
/*======================================================================*/
#include "pack.h"

#include "nd.h"
#include "om.h"

void om_com_character_cur_instance_change(OM_CHARACTER_DATA *data, int nd_no);
void om_com_character_globalresource_change(OM_CHARACTER_DATA *data);
OM_CHARACTER_DATA *om_com_charast_chain_search(OM_CHARACTER_ST *st, u32 entry_number);
int om_com_circle_hit_check(OM_HIT_CHECK_ST *obj1, OM_HIT_CHECK_ST *obj2);
void om_com__memcpy(void *dist, void *source, u32 copy_size);
void om_com__memset(void *dist, char set_data, u32 set_size);

void om_com_character_cur_instance_change(OM_CHARACTER_DATA *data, int nd_no)
{
    data->cur_instp = (data->instp_list + nd_no);
}

void om_com_character_globalresource_change(OM_CHARACTER_DATA *data)
{
    objlist   = data->cur_instp->objlist;
    animlist  = data->cur_instp->animlist;
    shapelist = data->cur_instp->shapelist;
    rawvtxlist = data->cur_instp->rawvtxlist;
    rawvtxcollist = data->cur_instp->rawvtxcollist;
    rawvtxnvlist = data->cur_instp->rawvtxnvlist;
}

OM_CHARACTER_DATA *om_com_charast_chain_search(OM_CHARACTER_ST *st, u32 entry_number)
{
    u32 i, max_entry;
    OM_CHARACTER_DATA *data;

    data = st->data;
    max_entry = st->header.character_entry;

    for(i=0; i<max_entry; i++)
    {
        if (data->entry_number == entry_number) {
            return data;
        }
        if (data->next == (OM_CHARACTER_DATA*)NULL)
            return (OM_CHARACTER_DATA*)-1;

        data = data->next;
    }
    return (OM_CHARACTER_DATA*)-1;
}


int om_com_circle_hit_check(OM_HIT_CHECK_ST *obj1, OM_HIT_CHECK_ST *obj2)
{
    float dx, dy, dz, distance1, distance2, r1, r2;

    dx = obj2->x - obj1->x;  /* x-coordinate diff detween main character and rival */
    dy = obj2->y - obj1->y;  /* y-coordinate diff detween main character and rival */
    dz = obj2->z - obj1->z;  /* z-coordinate diff detween main character and rival */
    r1 = obj1->r;
    r2 = obj2->r;

    if (dx < 0)
        dx = -dx;
    if (dy < 0)
        dy = -dy;
    if (dz < 0)
        dz = -dz;

    /* Determine the distance between the main character and the rival at two
	points using the Pythagorean theorem */
    distance1 = (dx * dx) + (dy * dy) + (dz * dz);
    distance2 = (r1 + r2) * (r1 + r2);

    if (distance1 < distance2)
        return 1;
    return 0;
}

void om_com_memcpy(void *dist, char *source, u32 copy_size)
{
    u32 i;
    char *d, *s;

    d = (char*)dist;
    s = (char*)source;
    for(i=0; i<copy_size; i++)
    {
        *d = *s;
        ++d;
        ++s;
    }
}

void om_com_memset(void *dist, char set_data, u32 set_size)
{
    u32 i;
    char *d;

    d = (char*)dist;
    for(i=0; i<set_size; i++)
    {
        *d = set_data;
        ++d;
    }
}

