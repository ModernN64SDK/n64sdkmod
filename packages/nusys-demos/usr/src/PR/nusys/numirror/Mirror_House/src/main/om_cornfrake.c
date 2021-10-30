/*======================================================================*/
/*	Mirror House							*/
/*	om_cornfrake.c							*/
/*									*/
/*	Copyright (C) 1998 MONEGI CORPORATION				*/
/*									*/
/*======================================================================*/
#include "pack.h"

#include <math.h>
#include "nd.h"
#include "nusys.h"

#include "om.h"

/* ===========================================================================
	External Reference Declarations
 =========================================================================== */
extern EXCONTPAD *pad;

/* ===========================================================================
	Function Prototype Declarations 
 =========================================================================== */
void om_cornfrake_main(OM_CHARACTER_ST *st, OM_BG_STAGE_ST *stage);
void om_cornfrake_data_init(OM_BG_STAGE_ST *stage);
static void om_cornfrake_animation(OM_CHARACTER_ST *st);
static int om_cornfrake_display_check(OM_CHARACTER_DATA *data, OM_BG_STAGE_ST *stage);

/* ===========================================================================
	Function Definitions 
 =========================================================================== */
void om_cornfrake_main(OM_CHARACTER_ST *st, OM_BG_STAGE_ST *stage)
{
    om_cornfrake_display_check(st->data->next, stage);
    om_cornfrake_animation(st);
}

void om_cornfrake_data_init(OM_BG_STAGE_ST *stage)
{
    int corn_num;
    OM_CORNFRAKE_ST *corn;

    corn = stage->corn;

    /* Set the display flag for cornflakes to display  */
    for(corn_num=0; corn_num<stage->cornfrake_num; corn_num++)
    {
        corn[corn_num].dsp_status = OM_DISPLAY;
    }
}

static void om_cornfrake_animation(OM_CHARACTER_ST *st)
{
    static int r_flag = 0;
    OM_CHARACTER_DATA *data;

    data = st->data->next;

    if (!r_flag) {
        data->rx += 1.0F;
        if (data->rx > 45.0F)
            r_flag = 1;
    }
    if (r_flag) {
        data->rx -= 1.0F;
        if (data->rx < -45.0F)
            r_flag = 0;
    }
}

static int om_cornfrake_display_check(OM_CHARACTER_DATA *data, OM_BG_STAGE_ST *stage)
{
    u32 corn_num;
    OM_CORNFRAKE_ST *corn;

    corn = stage->corn;

    for(corn_num=0; corn_num<stage->cornfrake_num; corn_num++)
    {
        /* No display check if cornflakes already xxdisplayedxx */
        if (corn[corn_num].dsp_status == 2)
            continue;

        /* If rendering is turned on for the area where cornflakes are arranged,*/
        /* cornflakes are displayed */
        if (stage->area_dsp_flg[corn[corn_num].area_number]) {
            data->x = corn[corn_num].x;
            data->y = corn[corn_num].y;
            data->z = corn[corn_num].z;
            data->dsp_status = OM_DISPLAY;
            corn[corn_num].dsp_status = OM_DISPLAY;
            return 0;
        } else {
            data->dsp_status = OM_NODISPLAY;
        }
    }
    return -1; /* Error */
}


