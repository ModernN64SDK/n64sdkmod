/*======================================================================*/
/*         OBJECT MANAGER                                               */
/*          om_ba_zoneB.h                                               */
/*                                                                      */
/*      Copyright (C) 1998, NINTENDO Co,Ltd.                            */
/*                                                                      */
/*======================================================================*/

#ifndef _OM_BG_ZONE_B_H_
#define _OM_BG_ZONE_B_H_

/*** Coordinate data for walls in Zone B1 ***/
OM_BG_WALL_ST om_ZoneB1_Grid0_Wall_Data[] = { {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0,0,0},
                                              {OM_WALL, -3000, 74, 900},
                                              {OM_WALL, 0,0,0} };
OM_BG_WALL_ST om_ZoneB1_Grid1_Wall_Data[] = { {OM_WALL, -2100, 74, 600},
                                              {OM_WALL, 0,0,0},
                                              {OM_WALL, 0,0,0},
                                              {OM_WALL, 0, 74, 0} };
OM_BG_WALL_ST om_ZoneB1_Grid2_Wall_Data[] = { {OM_WALL, 0,0,0},
                                              {OM_WALL, -2700, 74, 1800},
                                              {OM_WALL, -3000, 74, 1500},
                                              {OM_WALL, 0,0,0} };
OM_BG_WALL_ST om_ZoneB1_Grid3_Wall_Data[] = { {OM_WALL, 0,0,0},
                                              {OM_WALL, -2100, 74, 1800},
                                              {OM_WALL, 0,0,0},
                                              {OM_WALL, -1800,0,1500} };

/*** Zone B1  ***/
OM_BG_GRID_ST om_zoneB1_Grid0 = { 0, 0, 0, om_ZoneB1_Grid0_Wall_Data};
OM_BG_GRID_ST om_zoneB1_Grid1 = { 0, 0, 0, om_ZoneB1_Grid1_Wall_Data};
OM_BG_GRID_ST om_zoneB1_Grid2 = { 0, 0, 0, om_ZoneB1_Grid2_Wall_Data};
OM_BG_GRID_ST om_zoneB1_Grid3 = { 0, 0, 0, om_ZoneB1_Grid3_Wall_Data};

/*** Grid data for Zone B1 ***/
OM_BG_GRID_ST *om_zoneB1_Grid[] = { &om_zoneB1_Grid0, &om_zoneB1_Grid1, &om_zoneB1_Grid2, &om_zoneB1_Grid3 };


/*** Coordinate data for walls in Zone B2 ***/
OM_BG_WALL_ST om_ZoneB2_Grid0_Wall_Data[] = { {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, -1200, 0, 900} };
OM_BG_WALL_ST om_ZoneB2_Grid1_Wall_Data[] = { {OM_WALL, -900, 0, 600},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, -1200, 0, 900},
                                              {OM_WALL, -600, 0, 900} };
OM_BG_WALL_ST om_ZoneB2_Grid2_Wall_Data[] = { {OM_WALL, 0, 0, 0},
                                              {OM_WALL, -1500, 0, 1800},
                                              {OM_WALL, -1800, 0, 1500},
                                              {OM_WALL, -1200, 0, 1500} };
OM_BG_WALL_ST om_ZoneB2_Grid3_Wall_Data[] = { {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, -1200, 0, 1500},
                                              {OM_WALL, 0, 0, 0} };

/*** Zone B2 ***/
OM_BG_GRID_ST om_zoneB2_Grid0 = { 0, 0, 0, om_ZoneB2_Grid0_Wall_Data};
OM_BG_GRID_ST om_zoneB2_Grid1 = { 0, 0, 0, om_ZoneB2_Grid1_Wall_Data};
OM_BG_GRID_ST om_zoneB2_Grid2 = { 0, 0, 0, om_ZoneB2_Grid2_Wall_Data};
OM_BG_GRID_ST om_zoneB2_Grid3 = { 0, 0, 0, om_ZoneB2_Grid3_Wall_Data};

/*** Grid data for Zone B2 ***/
OM_BG_GRID_ST *om_zoneB2_Grid[] = { &om_zoneB2_Grid0, &om_zoneB2_Grid1, &om_zoneB2_Grid2, &om_zoneB2_Grid3 };


/*** Coordinate data for walls in Zone B3 ***/
OM_BG_WALL_ST om_ZoneB3_Grid0_Wall_Data[] = { {OM_WALL, 0, 0, 0},
                                              {OM_WALL, -300, 0, 1200},
                                              {OM_WALL, -600, 0, 900},
                                              {OM_WALL, 0, 0, 900} };
OM_BG_WALL_ST om_ZoneB3_Grid1_Wall_Data[] = { {OM_WALL, 300, 0, 600},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 900},
                                              {OM_WALL, 0, 0, 0} };
OM_BG_WALL_ST om_ZoneB3_Grid2_Wall_Data[] = { {OM_WALL, -300, 0, 1200},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0} };
OM_BG_WALL_ST om_ZoneB3_Grid3_Wall_Data[] = { {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0} };

/*** Zone B3 ***/
OM_BG_GRID_ST om_zoneB3_Grid0 = { 0, 0, 0, om_ZoneB3_Grid0_Wall_Data};
OM_BG_GRID_ST om_zoneB3_Grid1 = { 0, 0, 0, om_ZoneB3_Grid1_Wall_Data};
OM_BG_GRID_ST om_zoneB3_Grid2 = { 0, 0, 0, om_ZoneB3_Grid2_Wall_Data};
OM_BG_GRID_ST om_zoneB3_Grid3 = { 0, 0, 0, om_ZoneB3_Grid3_Wall_Data};

/*** Grid data for Zone B3 ***/
OM_BG_GRID_ST *om_zoneB3_Grid[] = { &om_zoneB3_Grid0, &om_zoneB3_Grid1, &om_zoneB3_Grid2, &om_zoneB3_Grid3 };


/*** Coordinate data for walls in Zone B4 ***/
OM_BG_WALL_ST om_ZoneB4_Grid0_Wall_Data[] = { {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 900, 0, 1200},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0} };
OM_BG_WALL_ST om_ZoneB4_Grid1_Wall_Data[] = { {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 1500, 0, 1200},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0} };
OM_BG_WALL_ST om_ZoneB4_Grid2_Wall_Data[] = { {OM_WALL, 900, 0, 1200},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0} };
OM_BG_WALL_ST om_ZoneB4_Grid3_Wall_Data[] = { {OM_WALL, 1500, 0, 1200},
                                              {OM_WALL, 1500, 0, 1800},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0} };

/*** Zone B4 ***/
OM_BG_GRID_ST om_zoneB4_Grid0 = { 0, 0, 0, om_ZoneB4_Grid0_Wall_Data};
OM_BG_GRID_ST om_zoneB4_Grid1 = { 0, 0, 0, om_ZoneB4_Grid1_Wall_Data};
OM_BG_GRID_ST om_zoneB4_Grid2 = { 0, 0, 0, om_ZoneB4_Grid2_Wall_Data};
OM_BG_GRID_ST om_zoneB4_Grid3 = { 0, 0, 0, om_ZoneB4_Grid3_Wall_Data};

/*** Grid data for Zone B4 ***/
OM_BG_GRID_ST *om_zoneB4_Grid[] = { &om_zoneB4_Grid0, &om_zoneB4_Grid1, &om_zoneB4_Grid2, &om_zoneB4_Grid3 };


/*** Coordinate data for walls in Zone B5 ***/
OM_BG_WALL_ST om_ZoneB5_Grid0_Wall_Data[] = { {OM_WALL, 2100, 0, 600},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0} };
OM_BG_WALL_ST om_ZoneB5_Grid1_Wall_Data[] = { {OM_WALL, 2700, 0, 600},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 3000, 0, 900} };
OM_BG_WALL_ST om_ZoneB5_Grid2_Wall_Data[] = { {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 2100, 0, 1800},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0} };
OM_BG_WALL_ST om_ZoneB5_Grid3_Wall_Data[] = { {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 2700, 0, 1800},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 3000, 0, 1500} };

/*** Zone B5 ***/
OM_BG_GRID_ST om_zoneB5_Grid0 = { 0, 0, 0, om_ZoneB5_Grid0_Wall_Data};
OM_BG_GRID_ST om_zoneB5_Grid1 = { 0, 0, 0, om_ZoneB5_Grid1_Wall_Data};
OM_BG_GRID_ST om_zoneB5_Grid2 = { 0, 0, 0, om_ZoneB5_Grid2_Wall_Data};
OM_BG_GRID_ST om_zoneB5_Grid3 = { 0, 0, 0, om_ZoneB5_Grid3_Wall_Data};

/*** Grid data for Zone B5 ***/
OM_BG_GRID_ST *om_zoneB5_Grid[] = { &om_zoneB5_Grid0, &om_zoneB5_Grid1, &om_zoneB5_Grid2, &om_zoneB5_Grid3 };


OM_BG_ZONE_ST om_ZoneB1 = { "B1", 0, 0, 3, om_zoneB1_Grid };
OM_BG_ZONE_ST om_ZoneB2 = { "B2", 1, 0, 3, om_zoneB2_Grid };
OM_BG_ZONE_ST om_ZoneB3 = { "B3", 2, 0, 3, om_zoneB3_Grid };
OM_BG_ZONE_ST om_ZoneB4 = { "B4", 3, 0, 3, om_zoneB4_Grid };
OM_BG_ZONE_ST om_ZoneB5 = { "B5", 4, 0, 3, om_zoneB5_Grid };

#endif /* _OM_BG_ZONE_B_H_ end */

