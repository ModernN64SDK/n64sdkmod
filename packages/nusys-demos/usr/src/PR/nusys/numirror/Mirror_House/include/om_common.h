/*======================================================================*/
/*	Mirror House							*/
/*	om_common.h							*/
/*									*/
/*	Copyright (C) 1998 MONEGI CORPORATION				*/
/*									*/
/*======================================================================*/
#include "om.h"

#ifndef _OM_COMMON_H_
#define _OM_COMMON_H_

extern void om_com_character_cur_instance_change(OM_CHARACTER_DATA *data, int nd_no);
extern void om_com_character_globalresource_change(OM_CHARACTER_DATA *data);
extern OM_CHARACTER_DATA *om_com_charast_chain_search(OM_CHARACTER_ST *st, u32 entry_number);
extern int om_com_circle_hit_check(OM_HIT_CHECK_ST *obj1, OM_HIT_CHECK_ST *obj2);
extern void om_com_memcpy(void *dist, void *source, u32 copy_size);
extern void om_com_memset(void *dist, char set_data, u32 set_size);

#endif
