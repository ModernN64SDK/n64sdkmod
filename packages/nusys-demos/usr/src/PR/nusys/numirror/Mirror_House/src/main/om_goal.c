/*======================================================================*/
/*	Mirror House							*/
/*	om_goal.c							*/
/*									*/
/*	Copyright (C) 1998 MONEGI CORPORATION				*/
/*									*/
/*======================================================================*/
#include "pack.h"

#include <math.h>
#include "nd.h"
#include "nusys.h"

#include "om.h"
#include "om_def.h"

/* ===========================================================================
	External Reference Declarations 
 =========================================================================== */

/* ===========================================================================
	External Function Reference Declarations 
 =========================================================================== */
extern void om_com_character_cur_instance_change(OM_CHARACTER_DATA *data, int nd_no);
extern void om_com_character_globalresource_change(OM_CHARACTER_DATA *data);
extern OM_CHARACTER_DATA *om_com_charast_chain_search(OM_CHARACTER_ST *st, u32 entry_number);

/* ===========================================================================
	Function Prototype Declarations 
 =========================================================================== */
void om_goal_plate_init(OM_CHARACTER_ST *st);
void om_goal_goal_plate_main(OM_CHARACTER_ST *st);

/* ===========================================================================
	Function Definitions 
 =========================================================================== */
void om_goal_plate_init(OM_CHARACTER_ST *st)
{
    OM_CHARACTER_DATA *data;

    data = om_com_charast_chain_search(st, OM_GOAL_PLATE);
    data->dsp_status = OM_DISPLAY;
}

void om_goal_goal_plate_main(OM_CHARACTER_ST *st)
{
    OM_CHARACTER_DATA *data;

    data = om_com_charast_chain_search(st, OM_GOAL_PLATE);
    data->ry += 0.5F;
}

