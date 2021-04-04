/*======================================================================*/
/*         OBJECT MANAGER                                               */
/*          om_bg_data.h                                                */
/*                                                                      */
/*      Copyright (C) 1998, NINTENDO Co,Ltd.                            */
/*                                                                      */
/*======================================================================*/

#ifndef _OM_BG_DATA_H_
#define _OM_BG_DATA_H_

#include "om_bg_def.h"
#include "om_bg_zoneA.h"
#include "om_bg_zoneB.h"
#include "om_bg_zoneC.h"
#include "om_bg_zoneD.h"
#include "om_bg_zoneE.h"

/* ===========================================================================
	Map Hit Evaluation Data (wall coordinates data)
 =========================================================================== */
OM_BG_ZONE_ST *om_ZoneAll1[] = { &om_ZoneE1, &om_ZoneE2, &om_ZoneE3, &om_ZoneE4, &om_ZoneE5,
                                 &om_ZoneD1, &om_ZoneD2, &om_ZoneD3, &om_ZoneD4, &om_ZoneD5,
                                 &om_ZoneC1, &om_ZoneC2, &om_ZoneC3, &om_ZoneC4, &om_ZoneC5,
                                 &om_ZoneB1, &om_ZoneB2, &om_ZoneB3, &om_ZoneB4, &om_ZoneB5,
                                 &om_ZoneA1, &om_ZoneA2, &om_ZoneA3, &om_ZoneA4, &om_ZoneA5
};

/* ===========================================================================
	Cornflake Arrangement Data
 =========================================================================== */
OM_CORNFRAKE_ST om_cornfrake1[] = { {0, 1, -2700, 0, -1500, 100.0},
                                    {4, 1,  2700, 0,  2100, 100.0}
};


/* ===========================================================================
	Map Data for Rendering (wall data)
 =========================================================================== */
extern OM_BACKGROUND_GROUP_ST _ombggrp_A1Group[];
extern OM_BACKGROUND_GROUP_ST _ombggrp_B1Group[];
extern OM_BACKGROUND_GROUP_ST _ombggrp_C1Group[];
extern OM_BACKGROUND_GROUP_ST _ombggrp_D1Group[];
extern OM_BACKGROUND_GROUP_ST _ombggrp_E1Group[];


OM_BACKGROUND_ST    om_background_A1[] = {
	{&_ombggrp_A1Group[0],	 0, 0, "A1"},
	{&_ombggrp_A1Group[1],	 0, 0, "A2"},
	{&_ombggrp_A1Group[2],	 0, 0, "A3"},
	{&_ombggrp_A1Group[3],	 0, 0, "A4"},
	{&_ombggrp_A1Group[4],	 0, 0, "A5"},
	{NULL,		         0, 0, ""},
};

OM_BACKGROUND_ST    om_background_B1[] = {
	{&_ombggrp_B1Group[0],	 0, 0, "B1"},
	{&_ombggrp_B1Group[1],	 0, 0, "B2"},
	{&_ombggrp_B1Group[2],	 0, 0, "B3"},
	{&_ombggrp_B1Group[3],	 0, 0, "B4"},
	{&_ombggrp_B1Group[4],	 0, 0, "B5"},
	{NULL,		         0, 0, ""},
};

OM_BACKGROUND_ST    om_background_C1[] = {
	{&_ombggrp_C1Group[0],	 0, 0, "C1"},
	{&_ombggrp_C1Group[1],	 0, 0, "C2"},
	{&_ombggrp_C1Group[2],	 0, 0, "C3"},
	{&_ombggrp_C1Group[3],	 0, 0, "C4"},
	{&_ombggrp_C1Group[4],	 0, 0, "C5"},
	{NULL,		         0, 0, ""},
};

OM_BACKGROUND_ST    om_background_D1[] = {
	{&_ombggrp_D1Group[0],	 0, 0, "D1"},
	{&_ombggrp_D1Group[1],	 0, 0, "D2"},
	{&_ombggrp_D1Group[2],	 0, 0, "D3"},
	{&_ombggrp_D1Group[3],	 0, 0, "D4"},
	{&_ombggrp_D1Group[4],	 0, 0, "D5"},
	{NULL,		         0, 0, ""},
};

OM_BACKGROUND_ST    om_background_E1[] = {
	{&_ombggrp_E1Group[0],	 0, 0, "E1"},
	{&_ombggrp_E1Group[1],	 0, 0, "E2"},
	{&_ombggrp_E1Group[2],	 0, 0, "E3"},
	{&_ombggrp_E1Group[3],	 0, 0, "E4"},
	{&_ombggrp_E1Group[4],	 0, 0, "E5"},
	{NULL,		         0, 0, ""},
};

OM_BACKGROUND_ST *om_background_group[] = { om_background_E1,
                                            om_background_D1,
                                            om_background_C1,
                                            om_background_B1,
                                            om_background_A1,
};


/* ===========================================================================
	Stage Data
 =========================================================================== */
OM_BG_STAGE_ST om_bg_stage_data = { 0,
                                    {0, 0, 0, 0, 0},
                                    {0, 0, 0, 0, 0, 0, 0,
                                     0, 0, 0, 0, 0, 0, 0,
                                     0, 0, 0, 0, 0, 0, 0,
                                     0, 0, 0, 0, 0, 0, 0,
                                     0, 1, 1, 0, 0, 0, 0,
                                     0, 1, 1, 0, 0, 0, 0,
                                     0, 0, 0, 0, 0, 0, 0,},
                                    2,
                                    3660,
                                    5,
                                    om_ZoneAll1,
                                    om_cornfrake1,
                                    om_background_group
};

#endif /* _OM_BG_DATA_H_ end */

