/*======================================================================*/
/*	Mirror House							*/
/*	om_bg_entry.c							*/
/*									*/
/*	Copyright (C) 1998 MONEGI CORPORATION				*/
/*									*/
/*======================================================================*/
#include "pack.h"

#include "nd.h"
#include "nusys.h"
#include "math.h"
#include <string.h>

#include "om.h"
#include "om_bg_data.h"
#include "om_common.h"

/* ===========================================================================
	Function Prototype Declarations
 =========================================================================== */
int om_bg_stage_entry(OM_BG_STAGE_ST **stage);
void om_bg_stage_entry_delete(OM_BG_STAGE_ST *stage);
u32 om_bg_character_wall_hit_check(OM_CHARACTER_DATA *data, OM_BG_STAGE_ST *stage);
void om_bg_zone_coordinate_get(OM_CHARACTER_DATA *data, u32 *zone_x, u32 *zone_y, u32 *zone_z);

static int om_bg_xz_zone_wall_hit_check(OM_BG_ZONE_ST *zone, float myx, float myz);
static int om_bg_hwall_down_hit_check(OM_BG_WALL_ST *wall, float myx, float myz);
static int om_bg_hwall_up_hit_check(OM_BG_WALL_ST *wall, float myx, float myz);
static int om_bg_vwall_left_hit_check(OM_BG_WALL_ST *wall, float myx, float myz);
static int om_bg_vwall_right_hit_check(OM_BG_WALL_ST *wall, float myx, float myz);
void om_bg_map_area_culling(u32 *area_dsp_flg, u32 area_number);
void om_bg_map_zone_culling(u32 *zone_dsp_flg, u32 area_number, u32 zone_x, u32 zone_z);

/* ===========================================================================
	Function Declarations
 =========================================================================== */
int om_bg_stage_entry(OM_BG_STAGE_ST **stage)
{
    if (*stage == (OM_BG_STAGE_ST*)NULL) {
        *stage = (OM_BG_STAGE_ST*)malloc(sizeof(OM_BG_STAGE_ST));
    }
    if (*stage == (OM_BG_STAGE_ST*)NULL ) {
        osSyncPrintf("Memory Alloc Error\n");
        return -1;
    }

    memcpy((char*)*stage, (char*)&om_bg_stage_data, sizeof(OM_BG_STAGE_ST));
    return 0;
}

void om_bg_stage_entry_delete(OM_BG_STAGE_ST *stage)
{
    /* Free memory reserved for the map */
    free(stage);
}

Gfx *om_bg_backgroundst_draw(Gfx *gp, OM_CHARACTER_ST *st, OM_BG_STAGE_ST *stage)
{
    u32 area, zone, obj, zone_index_start;
    Mtx m;
    float mtx2[4][4];
    OM_CHARACTER_DATA       *data;
    OM_BACKGROUND_ST        *bst;
    OM_BACKGROUND_GROUP_ST  *bg;
    OM_BACKGROUND_OBJECT_ST *bo;

    data  = st->data;

    for(area=0; area<5; area++)
    {
        /* Skip map rendering when the area is not displayed */
        if (!stage->area_dsp_flg[area])
            continue;

        bst = stage->bgst[area];

        /* xxCalculate the elements of the display flag array for the zone where Flake-kun is present from the zone coordinates where Flake is present.xx */
        zone_index_start = ((data->zone_z+1)*7);
        for(zone=0; zone<5; zone++, zone_index_start++)
        {
            /* Skip map rendering when the area is not displayed */
            if (!stage->zone_dsp_flg[zone_index_start]) {
                continue;
            }

            bg = bst[zone].bg;
            bo = bg->bo;

            for (obj=0; obj<bg->object_n; obj++)
            {
                guTranslateF(mtx2, bg->x + bo->x, bg->y + bo->y, bg->z - bo->z);
                guMtxF2L(mtx2, &m);
                gp = gfxLoadMatrix(gp, &m);
                gSPSegment(gp++, 4, bo->dsegbase[0]);	/*	gfx*/
                gSPSegment(gp++, 5, bo->dsegbase[1]);	/*	vertex*/
                gSPSegment(gp++, 6, bo->dsegbase[2]);	/*	texture*/
                gSPSegment(gp++, 7, bo->dsegbase[3]);	/*	tlut*/
                gSPDisplayList( gp++, bo->dsegbase[0]);
                bo++;
            }
        }
    }
    return gp;
}

void om_bg_zone_coordinate_get(OM_CHARACTER_DATA *data, u32 *zone_x, u32 *zone_y, u32 *zone_z)
{
    /* Calculate the zone in which the main character is currently located on the map from its x, y, and z coordinates. */
    if (zone_x != (u32*)NULL) {
        *zone_x = (3000 + data->x) / 1200;
    }

    if (zone_y != (u32*)NULL) {
        *zone_y = (3000 + data->y) / 1200;
    }

    if (zone_z != (u32*)NULL) {
        *zone_z = (3000 + data->z) / 1200;
    }
    data->area_number = *zone_z;  /* Save the area number for the location of the target character */
    data->zone_x      = *zone_x;  /* Save the x zone where the target character is located. */
    data->zone_z      = *zone_z;  /* Save the Z zone where the target character is located.  */
}

u32 om_bg_character_wall_hit_check(OM_CHARACTER_DATA *data, OM_BG_STAGE_ST *stage)
{
    u32 ret;
    u32 zone_x, zone_z;
    OM_BG_ZONE_ST     *zone;

    /* Obtain the zone coordinates of the location of the current target character */
    om_bg_zone_coordinate_get(data, &zone_x, NULL, &zone_z);

    /* Obtain the zone data for the location of the current target character and perform a hit check */
    zone = stage->zone[(zone_z*5)+zone_x];
    if ((ret = om_bg_xz_zone_wall_hit_check(zone, data->x, data->z))) {
        data->x = data->xb;
        data->z = data->zb;
        return ret;
    }
    return 0;
}

static int om_bg_xz_zone_wall_hit_check(OM_BG_ZONE_ST *zone, float myx, float myz)
{
    u32 i, j, attribute;
    OM_BG_GRID_ST *grid;
    OM_BG_WALL_ST *wall;

    /* Check the wall of the 2x2 grid */
    for(i=0; i<4; i++)
    {
        grid = zone->grid[i];
        /* Perform hit check of the walls (maximum of 4)that can be present on 1 grid */
        for(j=0; j<4; j++)
        {
            wall = &grid->wall[j];
            attribute = wall->attribute;  /* Set the wall attributes */
            switch(j)
            {
                case 0: /* Wall located on the upper portion of the grid */
                    if (om_bg_hwall_up_hit_check(wall, myx, myz)) {
                        return attribute;
                    }
                    break;
                case 1: /* Wall located on the lower portion of the grid */
                    if (om_bg_hwall_down_hit_check(wall, myx, myz)) {
                        return attribute;
                    }
                    break;
                case 2: /* Wall located to the left side of the grid */
                    if (om_bg_vwall_left_hit_check(wall, myx, myz)) {
                        return attribute;
                    }
                    break;
                case 3: /* Wall located to the right side of the grid */
                    if (om_bg_vwall_right_hit_check(wall, myx, myz)) {
                        return attribute;
                    }
                    break;
                default:
                    break;
            }
        }
    }
    return 0;
}

static int om_bg_hwall_down_hit_check(OM_BG_WALL_ST *wall, float myx, float myz)
{
    float wx, wz, wx1, wx2;
    
    wx = wall->x;
    wz = wall->z;

    if ((wx == 0) && (wz == 0))
        return 0;

    wx1 = wx - 300;
    wx2 = wx + 300;

    /* Perform hit check for wall arranged horizontally */
    /* Check whether the x coordinates of the main character are within the x coordinates of the wall (within its width). */
    if ((wx1 < myx) && (wx2 > myx)) {
        /* Check whether the Z coordinates of the main character are within the z coordinates of the wall (wall thickness). */
        if ((myz > (wz-200)) && (myz < wz)) {
            return 1;
        }
    }
    return 0;
}

static int om_bg_hwall_up_hit_check(OM_BG_WALL_ST *wall, float myx, float myz)
{
    float wx, wz, wx1, wx2;

    wx = wall->x;
    wz = wall->z;

    if ((wx == 0) && (wz == 0))
        return 0;

    wx1 = wx - 300;
    wx2 = wx + 300;

    /* Perform hit check for wall arranged horizontally */
    /* Check whether the x coordinates of the main character are within the x coordinates of the wall (within its width). */
    if ((wx1 < myx) && (wx2 > myx)) {
        /* Check whether the z coordinates of the main character are within the z coordinates of the wall (wall thickness). */
        if ((myz < (wz+150)) && (myz > wz)) {
            return 1;
        }
    }
    return 0;
}

static int om_bg_vwall_left_hit_check(OM_BG_WALL_ST *wall, float myx, float myz)
{
    float myx_b;
    float wx, wz, wz1, wz2;
    
    wx = wall->x;
    wz = wall->z;

    if ((wx == 0) && (wz == 0))
        return 0;

    myx_b = myx;
    /* Reverse the coordinates if the x and z coordinates of the main character are negative. */
    if (myx < 0)
        myx = -(myx);
    /* Reverse the coordinates if the x and z coordinates of the wall are negative. */
    if (wx < 0)
        wx = -(wx);

    wz1 = wz - 300;
    wz2 = wz + 300;

    /* Perform hit check of wall arranged vertically. */
    /* Check whether the z coordinates of the main character are within the width of the z coordinates of the wall. */
    if ((wz1 < myz) && (wz2 > myz)) {
        /* If the coordinates of the main character are in the negative region of the map */
        if (myx_b < 0) {
            /* Check whether the x coordinates of the main character are within the x coordinates of the wall (wall thickness). */
            if ((myx > (wx-150)) && (myx < wx)) {
                return 1;
            }
        }
        /* If the coordinates of the main character are in the positive region of the map */
        if (myx_b > 0) {
            /* Check whether the x coordinates of the main character are within the x coordinates of the wall (wall thickness). */
            if ((myx < (wx+150)) && (myx > wx)) {
                return 1;
            }
        }
    }
    return 0;
}

static int om_bg_vwall_right_hit_check(OM_BG_WALL_ST *wall, float myx, float myz)
{
    float myx_b;
    float wx;
    float wz, wz1, wz2;
    
    wx = wall->x;
    wz = wall->z;

    if ((wx == 0) && (wz == 0))
        return 0;

    myx_b = myx;
    /* Reverse the coordinates if the x and z coordinates of the main character  are negative. */
    if (myx < 0)
        myx = -(myx);
    /* Reverse the coordinates if the x and z coordinates of the wall are negative. */
    if (wx < 0) {
        wx = -(wx);
    }

    wz1 = wz - 300.0F;  /* Wall endpoint */
    wz2 = wz + 300.0F;  /* Wall endpoint */

    /* Perform hit check of wall arranged vertically  */
    /* Check whether the z coordinates of the main character are within the width of the z coordinates of the wall.  */
    if ((wz1 < myz) && (wz2 > myz)) {
        /* If the coordinates of the main character are in the positive region of the map */
        if (myx_b > 0) {
            /* Check whether the x coordinates of the main character are within the x coordinates of the wall (wall thickness). */
            if ((myx > (wx-150)) && (myx < wx)) {
                return 1;
            }
        }
        /* If the coordinates of the main character are in the positive region of the map  */
        if (myx_b < 0) {
            /* Check whether the x coordinates of the main character are within the x coordinates of the wall (wall thickness). */
            if ((myx < (wx+150)) && (myx > wx)) {
                return 1;
            }
        }
    }
    return 0;
}

void om_bg_map_area_culling(u32 *area_dsp_flg, u32 area_number)
{
    if (area_number == 4) {
        area_dsp_flg[area_number-2] = OM_DISPLAY;
        area_dsp_flg[area_number-1] = OM_DISPLAY;
        area_dsp_flg[area_number]   = OM_DISPLAY;
    } else if (area_number == 0) {
        area_dsp_flg[area_number+1] = OM_DISPLAY;
        area_dsp_flg[area_number+2] = OM_DISPLAY;
        area_dsp_flg[area_number]   = OM_DISPLAY;
    } else {
        area_dsp_flg[area_number-1] = OM_DISPLAY;
        area_dsp_flg[area_number]   = OM_DISPLAY;
        area_dsp_flg[area_number+1] = OM_DISPLAY;
    }
}

void om_bg_map_zone_culling(u32 *zone_dsp_flg, u32 area_number, u32 zone_x, u32 zone_z)
{
    u32 zone_index;

    zone_index = ((zone_z+1)*7)+zone_x;
    zone_dsp_flg[zone_index]   = OM_DISPLAY;
    zone_dsp_flg[zone_index-1] = OM_DISPLAY;
    zone_dsp_flg[zone_index+1] = OM_DISPLAY;
}

