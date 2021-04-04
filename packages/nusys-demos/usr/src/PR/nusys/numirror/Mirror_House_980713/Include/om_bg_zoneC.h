/*======================================================================*/
/*         OBJECT MANAGER                                               */
/*          om_ba_zoneC.h                                               */
/*                                                                      */
/*      Copyright (C) 1998, NINTENDO Co,Ltd.                            */
/*                                                                      */
/*======================================================================*/

#ifndef _OM_BG_ZONE_C_H_
#define _OM_BG_ZONE_C_H_

/*** Coordinate data for walls in Zone C1 ***/
OM_BG_WALL_ST om_ZoneC1_Grid0_Wall_Data[] = { {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0,0,0},
                                              {OM_WALL, -3000, 74, -300},
                                              {OM_WALL, -2400,0,-300} };
OM_BG_WALL_ST om_ZoneC1_Grid1_Wall_Data[] = { {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0,0,0},
                                              {OM_WALL, -2400,0,-300},
                                              {OM_WALL, 0, 0, 0} };
OM_BG_WALL_ST om_ZoneC1_Grid2_Wall_Data[] = { {OM_WALL, 0,0,0},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, -3000, 74, 300},
                                              {OM_WALL, -2400,0,300} };
OM_BG_WALL_ST om_ZoneC1_Grid3_Wall_Data[] = { {OM_WALL, 0,0,0},
                                              {OM_WALL, -2100, 74, 600},
                                              {OM_WALL, -2400,0,300},
                                              {OM_WALL, 0,0,0} };

/*** Zone C1 ***/
OM_BG_GRID_ST om_zoneC1_Grid0 = { 0, 0, 0, om_ZoneC1_Grid0_Wall_Data};
OM_BG_GRID_ST om_zoneC1_Grid1 = { 0, 0, 0, om_ZoneC1_Grid1_Wall_Data};
OM_BG_GRID_ST om_zoneC1_Grid2 = { 0, 0, 0, om_ZoneC1_Grid2_Wall_Data};
OM_BG_GRID_ST om_zoneC1_Grid3 = { 0, 0, 0, om_ZoneC1_Grid3_Wall_Data};

/*** Grid data for Zone C1 ***/
OM_BG_GRID_ST *om_zoneC1_Grid[] = { &om_zoneC1_Grid0, &om_zoneC1_Grid1, &om_zoneC1_Grid2, &om_zoneC1_Grid3 };


/*** Coordinate data for walls in Zone C2 ***/
OM_BG_WALL_ST om_ZoneC2_Grid0_Wall_Data[] = { {OM_WALL, -1500, 0, -600},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0} };
OM_BG_WALL_ST om_ZoneC2_Grid1_Wall_Data[] = { {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0} };
OM_BG_WALL_ST om_ZoneC2_Grid2_Wall_Data[] = { {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0} };
OM_BG_WALL_ST om_ZoneC2_Grid3_Wall_Data[] = { {OM_WALL, 0, 0, 0},
                                              {OM_WALL, -900, 0, 600},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0} };

/*** Zone C2 ***/
OM_BG_GRID_ST om_zoneC2_Grid0 = { 0, 0, 0, om_ZoneC2_Grid0_Wall_Data};
OM_BG_GRID_ST om_zoneC2_Grid1 = { 0, 0, 0, om_ZoneC2_Grid1_Wall_Data};
OM_BG_GRID_ST om_zoneC2_Grid2 = { 0, 0, 0, om_ZoneC2_Grid2_Wall_Data};
OM_BG_GRID_ST om_zoneC2_Grid3 = { 0, 0, 0, om_ZoneC2_Grid3_Wall_Data};

/*** Grid data for Zone C2 ***/
OM_BG_GRID_ST *om_zoneC2_Grid[] = { &om_zoneC2_Grid0, &om_zoneC2_Grid1, &om_zoneC2_Grid2, &om_zoneC2_Grid3 };


/*** Coordinate data for walls in Zone C3 ***/
OM_BG_WALL_ST om_ZoneC3_Grid0_Wall_Data[] = { {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, -300} };
OM_BG_WALL_ST om_ZoneC3_Grid1_Wall_Data[] = { {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, -300},
                                              {OM_WALL, 600, 0, -300} };
OM_BG_WALL_ST om_ZoneC3_Grid2_Wall_Data[] = { {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0} };
OM_BG_WALL_ST om_ZoneC3_Grid3_Wall_Data[] = { {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 300, 0, 600},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 600, 0, 300} };

/*** Zone C3 ***/
OM_BG_GRID_ST om_zoneC3_Grid0 = { 0, 0, 0, om_ZoneC3_Grid0_Wall_Data};
OM_BG_GRID_ST om_zoneC3_Grid1 = { 0, 0, 0, om_ZoneC3_Grid1_Wall_Data};
OM_BG_GRID_ST om_zoneC3_Grid2 = { 0, 0, 0, om_ZoneC3_Grid2_Wall_Data};
OM_BG_GRID_ST om_zoneC3_Grid3 = { 0, 0, 0, om_ZoneC3_Grid3_Wall_Data};

/*** Grid data for Zone C3 ***/
OM_BG_GRID_ST *om_zoneC3_Grid[] = { &om_zoneC3_Grid0, &om_zoneC3_Grid1, &om_zoneC3_Grid2, &om_zoneC3_Grid3 };


/*** Coordinate data for walls in Zone C4 ***/
OM_BG_WALL_ST om_ZoneC4_Grid0_Wall_Data[] = { {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 600, 0, -300},
                                              {OM_WALL, 0, 0, 0} };
OM_BG_WALL_ST om_ZoneC4_Grid1_Wall_Data[] = { {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0} };
OM_BG_WALL_ST om_ZoneC4_Grid2_Wall_Data[] = { {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 600, 0, 300},
                                              {OM_WALL, 0, 0, 0} };
OM_BG_WALL_ST om_ZoneC4_Grid3_Wall_Data[] = { {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0} };

/*** Zone C4 ***/
OM_BG_GRID_ST om_zoneC4_Grid0 = { 0, 0, 0, om_ZoneC4_Grid0_Wall_Data};
OM_BG_GRID_ST om_zoneC4_Grid1 = { 0, 0, 0, om_ZoneC4_Grid1_Wall_Data};
OM_BG_GRID_ST om_zoneC4_Grid2 = { 0, 0, 0, om_ZoneC4_Grid2_Wall_Data};
OM_BG_GRID_ST om_zoneC4_Grid3 = { 0, 0, 0, om_ZoneC4_Grid3_Wall_Data};

/*** Grid data for Zone C4 ***/
OM_BG_GRID_ST *om_zoneC4_Grid[] = { &om_zoneC4_Grid0, &om_zoneC4_Grid1, &om_zoneC4_Grid2, &om_zoneC4_Grid3 };


/*** Coordinate data for walls in Zone C5 ***/
OM_BG_WALL_ST om_ZoneC5_Grid0_Wall_Data[] = { {OM_WALL, 2100, 0, -600},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0} };
OM_BG_WALL_ST om_ZoneC5_Grid1_Wall_Data[] = { {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 3000, 0, -300} };
OM_BG_WALL_ST om_ZoneC5_Grid2_Wall_Data[] = { {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 2100, 0, 600},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0} };
OM_BG_WALL_ST om_ZoneC5_Grid3_Wall_Data[] = { {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 2700, 0, 600},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 3000, 0, 300} };

/*** Zone C5 ***/
OM_BG_GRID_ST om_zoneC5_Grid0 = { 0, 0, 0, om_ZoneC5_Grid0_Wall_Data};
OM_BG_GRID_ST om_zoneC5_Grid1 = { 0, 0, 0, om_ZoneC5_Grid1_Wall_Data};
OM_BG_GRID_ST om_zoneC5_Grid2 = { 0, 0, 0, om_ZoneC5_Grid2_Wall_Data};
OM_BG_GRID_ST om_zoneC5_Grid3 = { 0, 0, 0, om_ZoneC5_Grid3_Wall_Data};

/*** Grid data for Zone C5 ***/
OM_BG_GRID_ST *om_zoneC5_Grid[] = { &om_zoneC5_Grid0, &om_zoneC5_Grid1, &om_zoneC5_Grid2, &om_zoneC5_Grid3 };


OM_BG_ZONE_ST om_ZoneC1 = { "C1", 0, 0, 2, om_zoneC1_Grid };
OM_BG_ZONE_ST om_ZoneC2 = { "C2", 1, 0, 2, om_zoneC2_Grid };
OM_BG_ZONE_ST om_ZoneC3 = { "C3", 2, 0, 2, om_zoneC3_Grid };
OM_BG_ZONE_ST om_ZoneC4 = { "C4", 3, 0, 2, om_zoneC4_Grid };
OM_BG_ZONE_ST om_ZoneC5 = { "C5", 4, 0, 2, om_zoneC5_Grid };

#endif /* _OM_BG_ZONE_C_H_ end */

