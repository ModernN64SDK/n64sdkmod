/*======================================================================*/
/*          OBJECT MANAGER                                              */
/*          om_ba_zoneE.h                                               */
/*                                                                      */
/*      Copyright (C) 1998, NINTENDO Co,Ltd.                            */
/*                                                                      */
/*======================================================================*/

#ifndef _OM_BG_ZONE_E_H_
#define _OM_BG_ZONE_E_H_

/*** Coordinate data for walls in Zone E1 ***/
OM_BG_WALL_ST om_ZoneE1_Grid0_Wall_Data[] = { {OM_WALL, -2700, 0, -3000},
                                              {OM_WALL, 0,0,0},
                                              {OM_GOAL, -3000, 74, -2700},
                                              {OM_WALL, 0,0,0} };
OM_BG_WALL_ST om_ZoneE1_Grid1_Wall_Data[] = { {OM_WALL, -2100, 0, -3000},
                                              {OM_WALL, -2100,0,-2400},
                                              {OM_WALL, 0,0,0},
                                              {OM_WALL, -1800, 0, -2700} };
OM_BG_WALL_ST om_ZoneE1_Grid2_Wall_Data[] = { {OM_WALL, 0,0,0},
                                              {OM_WALL, -2700, 0, -1800},
                                              {OM_WALL, -3000, 74, -2100},
                                              {OM_WALL, 0, 0, 0} };
OM_BG_WALL_ST om_ZoneE1_Grid3_Wall_Data[] = { {OM_WALL, -2100, 0, -2400},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0,0,0} };

/*** Zone E1 ***/
OM_BG_GRID_ST om_zoneE1_Grid0 = { 0, 0, 0, om_ZoneE1_Grid0_Wall_Data};
OM_BG_GRID_ST om_zoneE1_Grid1 = { 0, 0, 0, om_ZoneE1_Grid1_Wall_Data};
OM_BG_GRID_ST om_zoneE1_Grid2 = { 0, 0, 0, om_ZoneE1_Grid2_Wall_Data};
OM_BG_GRID_ST om_zoneE1_Grid3 = { 0, 0, 0, om_ZoneE1_Grid3_Wall_Data};

/*** Grid data for Zone E1 ***/
OM_BG_GRID_ST *om_zoneE1_Grid[] = { &om_zoneE1_Grid0, &om_zoneE1_Grid1, &om_zoneE1_Grid2, &om_zoneE1_Grid3 };


/*** Coordinate data for walls in Zone E2 ***/
OM_BG_WALL_ST om_ZoneE2_Grid0_Wall_Data[] = { {OM_WALL, -1500, 0, -3000},
                                              {OM_WALL, -1500, 0, -2400},
                                              {OM_WALL, -1800, 0, -2700},
                                              {OM_WALL, 0, 0, 0} };
OM_BG_WALL_ST om_ZoneE2_Grid1_Wall_Data[] = { {OM_WALL, -900, 0, -3000},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0} };
OM_BG_WALL_ST om_ZoneE2_Grid2_Wall_Data[] = { {OM_WALL, -1500, 0, -2400},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, -1200, 0, -2100} };
OM_BG_WALL_ST om_ZoneE2_Grid3_Wall_Data[] = { {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, -1200, 0, -2100},
                                              {OM_WALL, -600, 0, -2100} };

/*** Zone E2 ***/
OM_BG_GRID_ST om_zoneE2_Grid0 = { 0, 0, 0, om_ZoneE2_Grid0_Wall_Data};
OM_BG_GRID_ST om_zoneE2_Grid1 = { 0, 0, 0, om_ZoneE2_Grid1_Wall_Data};
OM_BG_GRID_ST om_zoneE2_Grid2 = { 0, 0, 0, om_ZoneE2_Grid2_Wall_Data};
OM_BG_GRID_ST om_zoneE2_Grid3 = { 0, 0, 0, om_ZoneE2_Grid3_Wall_Data};

/*** Grid data for Zone E2 ***/
OM_BG_GRID_ST *om_zoneE2_Grid[] = { &om_zoneE2_Grid0, &om_zoneE2_Grid1, &om_zoneE2_Grid2, &om_zoneE2_Grid3 };


/*** Coordinate data for walls in Zone E3 ***/
OM_BG_WALL_ST om_ZoneE3_Grid0_Wall_Data[] = { {OM_WALL, -300, 0, -3000},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0} };
OM_BG_WALL_ST om_ZoneE3_Grid1_Wall_Data[] = { {OM_WALL, 300, 0, -3000},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 600, 0, -2700} };
OM_BG_WALL_ST om_ZoneE3_Grid2_Wall_Data[] = { {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, -600, 0, -2100},
                                              {OM_WALL, 0, 0, -2100} };
OM_BG_WALL_ST om_ZoneE3_Grid3_Wall_Data[] = { {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 300, 0, -1800},
                                              {OM_WALL, 0, 0, -2100},
                                              {OM_WALL, 0, 0, 0} };

/*** Zone E3 ***/
OM_BG_GRID_ST om_zoneE3_Grid0 = { 0, 0, 0, om_ZoneE3_Grid0_Wall_Data};
OM_BG_GRID_ST om_zoneE3_Grid1 = { 0, 0, 0, om_ZoneE3_Grid1_Wall_Data};
OM_BG_GRID_ST om_zoneE3_Grid2 = { 0, 0, 0, om_ZoneE3_Grid2_Wall_Data};
OM_BG_GRID_ST om_zoneE3_Grid3 = { 0, 0, 0, om_ZoneE3_Grid3_Wall_Data};

/*** Grid data for Zone E3 ***/
OM_BG_GRID_ST *om_zoneE3_Grid[] = { &om_zoneE3_Grid0, &om_zoneE3_Grid1, &om_zoneE3_Grid2, &om_zoneE3_Grid3 };


/*** Coordinate data for walls in Zone E4 ***/
OM_BG_WALL_ST om_ZoneE4_Grid0_Wall_Data[] = { {OM_WALL, 900, 0, -3000},
                                              {OM_WALL, 900, 0, -2400},
                                              {OM_WALL, 600, 0, -2700},
                                              {OM_WALL, 0, 0, 0} };
OM_BG_WALL_ST om_ZoneE4_Grid1_Wall_Data[] = { {OM_WALL, 1500, 0, -3000},
                                              {OM_WALL, 1500, 0, -2400},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0} };
OM_BG_WALL_ST om_ZoneE4_Grid2_Wall_Data[] = { {OM_WALL, 900, 0, -2400},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0} };
OM_BG_WALL_ST om_ZoneE4_Grid3_Wall_Data[] = { {OM_WALL, 1500, 0, -2400},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 1800, 0, -2100} };

/*** Zone E4 ***/
OM_BG_GRID_ST om_zoneE4_Grid0 = { 0, 0, 0, om_ZoneE4_Grid0_Wall_Data};
OM_BG_GRID_ST om_zoneE4_Grid1 = { 0, 0, 0, om_ZoneE4_Grid1_Wall_Data};
OM_BG_GRID_ST om_zoneE4_Grid2 = { 0, 0, 0, om_ZoneE4_Grid2_Wall_Data};
OM_BG_GRID_ST om_zoneE4_Grid3 = { 0, 0, 0, om_ZoneE4_Grid3_Wall_Data};

/*** Grid data for Zone E4 ***/
OM_BG_GRID_ST *om_zoneE4_Grid[] = { &om_zoneE4_Grid0, &om_zoneE4_Grid1, &om_zoneE4_Grid2, &om_zoneE4_Grid3 };


/*** Grid data for Zone E5 ***/
OM_BG_WALL_ST om_ZoneE5_Grid0_Wall_Data[] = { {OM_WALL, 2100, 0, -3000},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0} };
OM_BG_WALL_ST om_ZoneE5_Grid1_Wall_Data[] = { {OM_WALL, 2700, 0, -3000},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 3000, 0, -2700} };
OM_BG_WALL_ST om_ZoneE5_Grid2_Wall_Data[] = { {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 2100, 0, -1800},
                                              {OM_WALL, 1800, 0, -2100},
                                              {OM_WALL, 0, 0, 0} };
OM_BG_WALL_ST om_ZoneE5_Grid3_Wall_Data[] = { {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 2700, 0, -1800},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 3000, 0, -2100} };

/*** Zone E5 ***/
OM_BG_GRID_ST om_zoneE5_Grid0 = { 0, 0, 0, om_ZoneE5_Grid0_Wall_Data};
OM_BG_GRID_ST om_zoneE5_Grid1 = { 0, 0, 0, om_ZoneE5_Grid1_Wall_Data};
OM_BG_GRID_ST om_zoneE5_Grid2 = { 0, 0, 0, om_ZoneE5_Grid2_Wall_Data};
OM_BG_GRID_ST om_zoneE5_Grid3 = { 0, 0, 0, om_ZoneE5_Grid3_Wall_Data};

/*** Grid data for Zone E5 ***/
OM_BG_GRID_ST *om_zoneE5_Grid[] = { &om_zoneE5_Grid0, &om_zoneE5_Grid1, &om_zoneE5_Grid2, &om_zoneE5_Grid3 };


OM_BG_ZONE_ST om_ZoneE1 = { "E1", 0, 0, 0, om_zoneE1_Grid };
OM_BG_ZONE_ST om_ZoneE2 = { "E2", 1, 0, 0, om_zoneE2_Grid };
OM_BG_ZONE_ST om_ZoneE3 = { "E3", 2, 0, 0, om_zoneE3_Grid };
OM_BG_ZONE_ST om_ZoneE4 = { "E4", 3, 0, 0, om_zoneE4_Grid };
OM_BG_ZONE_ST om_ZoneE5 = { "E5", 4, 0, 0, om_zoneE5_Grid };

#endif /* _OM_BG_ZONE_E_H_ end */

