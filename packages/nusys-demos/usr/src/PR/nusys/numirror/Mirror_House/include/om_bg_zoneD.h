/*======================================================================*/
/*         OBJECT MANAGER                                               */
/*          om_ba_zoneD.h                                               */
/*                                                                      */
/*      Copyright (C) 1998, NINTENDO Co,Ltd.                            */
/*                                                                      */
/*======================================================================*/

#ifndef _OM_BG_ZONE_D_H_
#define _OM_BG_ZONE_D_H_

/*** Coordinate data for walls in Zone D1 ***/
OM_BG_WALL_ST om_ZoneD1_Grid0_Wall_Data[] = { {OM_WALL, -2700, 0, -1800},
                                              {OM_WALL, 0,0,0},
                                              {OM_WALL, -3000, 74, -1500},
                                              {OM_WALL, -2400,0,-1500} };
OM_BG_WALL_ST om_ZoneD1_Grid1_Wall_Data[] = { {OM_WALL, 0, 0, 0},
                                              {OM_WALL, -2100,0,-1200},
                                              {OM_WALL, -2400,0,-1500},
                                              {OM_WALL, -1800, 0, -1500} };
OM_BG_WALL_ST om_ZoneD1_Grid2_Wall_Data[] = { {OM_WALL, 0,0,0},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, -3000, 74, -900},
                                              {OM_WALL, -2400, 0, -900} };
OM_BG_WALL_ST om_ZoneD1_Grid3_Wall_Data[] = { {OM_WALL, -2100, 0, -1200},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, -2400, 0, -900},
                                              {OM_WALL, 0,0,0} };

/*** Zone D1 ***/
OM_BG_GRID_ST om_zoneD1_Grid0 = { 0, 0, 0, om_ZoneD1_Grid0_Wall_Data};
OM_BG_GRID_ST om_zoneD1_Grid1 = { 0, 0, 0, om_ZoneD1_Grid1_Wall_Data};
OM_BG_GRID_ST om_zoneD1_Grid2 = { 0, 0, 0, om_ZoneD1_Grid2_Wall_Data};
OM_BG_GRID_ST om_zoneD1_Grid3 = { 0, 0, 0, om_ZoneD1_Grid3_Wall_Data};

/*** Grid data for Zone D1 ***/
OM_BG_GRID_ST *om_zoneD1_Grid[] = { &om_zoneD1_Grid0, &om_zoneD1_Grid1, &om_zoneD1_Grid2, &om_zoneD1_Grid3 };


/*** Coordinate data for walls in Zone D2 ***/
OM_BG_WALL_ST om_ZoneD2_Grid0_Wall_Data[] = { {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, -1800, 0, -1500},
                                              {OM_WALL, -1200, 0, -1500} };
OM_BG_WALL_ST om_ZoneD2_Grid1_Wall_Data[] = { {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, -1200, 0, -1500},
                                              {OM_WALL, -600, 0, -1500} };
OM_BG_WALL_ST om_ZoneD2_Grid2_Wall_Data[] = { {OM_WALL, 0, 0, 0},
                                              {OM_WALL, -1500, 0, -600},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, -1200, 0, -900} };
OM_BG_WALL_ST om_ZoneD2_Grid3_Wall_Data[] = { {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, -1200, 0, -900},
                                              {OM_WALL, 0, 0, 0} };

/*** Zone D2 ***/
OM_BG_GRID_ST om_zoneD2_Grid0 = { 0, 0, 0, om_ZoneD2_Grid0_Wall_Data};
OM_BG_GRID_ST om_zoneD2_Grid1 = { 0, 0, 0, om_ZoneD2_Grid1_Wall_Data};
OM_BG_GRID_ST om_zoneD2_Grid2 = { 0, 0, 0, om_ZoneD2_Grid2_Wall_Data};
OM_BG_GRID_ST om_zoneD2_Grid3 = { 0, 0, 0, om_ZoneD2_Grid3_Wall_Data};

/*** Grid data for Zone D2 ***/
OM_BG_GRID_ST *om_zoneD2_Grid[] = { &om_zoneD2_Grid0, &om_zoneD2_Grid1, &om_zoneD2_Grid2, &om_zoneD2_Grid3 };


/*** Coordinate data for walls in Zone D3 ***/
OM_BG_WALL_ST om_ZoneD3_Grid0_Wall_Data[] = { {OM_WALL, 0, 0, 0},
                                              {OM_WALL, -300, 0, -1200},
                                              {OM_WALL, -600, 0, -1500},
                                              {OM_WALL, 0, 0, 0} };
OM_BG_WALL_ST om_ZoneD3_Grid1_Wall_Data[] = { {OM_WALL, 300, 0, -1800},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0} };
OM_BG_WALL_ST om_ZoneD3_Grid2_Wall_Data[] = { {OM_WALL, -300, 0, -1200},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, -900} };
OM_BG_WALL_ST om_ZoneD3_Grid3_Wall_Data[] = { {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, -900},
                                              {OM_WALL, 600, 0, -900} };

/*** Zone D3 ***/
OM_BG_GRID_ST om_zoneD3_Grid0 = { 0, 0, 0, om_ZoneD3_Grid0_Wall_Data};
OM_BG_GRID_ST om_zoneD3_Grid1 = { 0, 0, 0, om_ZoneD3_Grid1_Wall_Data};
OM_BG_GRID_ST om_zoneD3_Grid2 = { 0, 0, 0, om_ZoneD3_Grid2_Wall_Data};
OM_BG_GRID_ST om_zoneD3_Grid3 = { 0, 0, 0, om_ZoneD3_Grid3_Wall_Data};

/*** Grid data for Zone D3 ***/
OM_BG_GRID_ST *om_zoneD3_Grid[] = { &om_zoneD3_Grid0, &om_zoneD3_Grid1, &om_zoneD3_Grid2, &om_zoneD3_Grid3 };


/*** Coordinate data for walls in Zone D4 ***/
OM_BG_WALL_ST om_ZoneD4_Grid0_Wall_Data[] = { {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0} };
OM_BG_WALL_ST om_ZoneD4_Grid1_Wall_Data[] = { {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0} };
OM_BG_WALL_ST om_ZoneD4_Grid2_Wall_Data[] = { {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 600, 0, -900},
                                              {OM_WALL, 0, 0, 0} };
OM_BG_WALL_ST om_ZoneD4_Grid3_Wall_Data[] = { {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0} };

/*** Zone D4 ***/
OM_BG_GRID_ST om_zoneD4_Grid0 = { 0, 0, 0, om_ZoneD4_Grid0_Wall_Data};
OM_BG_GRID_ST om_zoneD4_Grid1 = { 0, 0, 0, om_ZoneD4_Grid1_Wall_Data};
OM_BG_GRID_ST om_zoneD4_Grid2 = { 0, 0, 0, om_ZoneD4_Grid2_Wall_Data};
OM_BG_GRID_ST om_zoneD4_Grid3 = { 0, 0, 0, om_ZoneD4_Grid3_Wall_Data};

/*** Grid data for Zone D4 ***/
OM_BG_GRID_ST *om_zoneD4_Grid[] = { &om_zoneD4_Grid0, &om_zoneD4_Grid1, &om_zoneD4_Grid2, &om_zoneD4_Grid3 };


/*** Coordinate data for walls in Zone D5 ***/
OM_BG_WALL_ST om_ZoneD5_Grid0_Wall_Data[] = { {OM_WALL, 2100, 0, -1800},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 2400, 0, -1500} };
OM_BG_WALL_ST om_ZoneD5_Grid1_Wall_Data[] = { {OM_WALL, 2700, 0, -1800},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 2400, 0, -1500},
                                              {OM_WALL, 3000, 0, -1500} };
OM_BG_WALL_ST om_ZoneD5_Grid2_Wall_Data[] = { {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 2100, 0, -600},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 2400, 0, -900} };
OM_BG_WALL_ST om_ZoneD5_Grid3_Wall_Data[] = { {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 2400, 0, -900},
                                              {OM_WALL, 3000, 0, -900} };

/*** Zone D5 ***/
OM_BG_GRID_ST om_zoneD5_Grid0 = { 0, 0, 0, om_ZoneD5_Grid0_Wall_Data};
OM_BG_GRID_ST om_zoneD5_Grid1 = { 0, 0, 0, om_ZoneD5_Grid1_Wall_Data};
OM_BG_GRID_ST om_zoneD5_Grid2 = { 0, 0, 0, om_ZoneD5_Grid2_Wall_Data};
OM_BG_GRID_ST om_zoneD5_Grid3 = { 0, 0, 0, om_ZoneD5_Grid3_Wall_Data};

/*** Grid data for Zone D5 ***/
OM_BG_GRID_ST *om_zoneD5_Grid[] = { &om_zoneD5_Grid0, &om_zoneD5_Grid1, &om_zoneD5_Grid2, &om_zoneD5_Grid3 };


OM_BG_ZONE_ST om_ZoneD1 = { "D1", 0, 0, 1, om_zoneD1_Grid };
OM_BG_ZONE_ST om_ZoneD2 = { "D2", 1, 0, 1, om_zoneD2_Grid };
OM_BG_ZONE_ST om_ZoneD3 = { "D3", 2, 0, 1, om_zoneD3_Grid };
OM_BG_ZONE_ST om_ZoneD4 = { "D4", 3, 0, 1, om_zoneD4_Grid };
OM_BG_ZONE_ST om_ZoneD5 = { "D5", 4, 0, 1, om_zoneD5_Grid };

#endif /* _OM_BG_ZONE_D_H_ end */

