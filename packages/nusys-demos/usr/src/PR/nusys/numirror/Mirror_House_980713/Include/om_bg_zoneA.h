/*======================================================================*/
/*         OBJECT MANAGER                                               */
/*          om_ba_zoneA.h                                               */
/*                                                                      */
/*      Copyright (C) 1998, NINTENDO Co,Ltd.                            */
/*                                                                      */
/*======================================================================*/

#ifndef _OM_BG_ZONE_A_H_
#define _OM_BG_ZONE_A_H_

/*** Coordinate data for walls in zone A1 ***/
OM_BG_WALL_ST om_ZoneA1_Grid0_Wall_Data[] = { {OM_WALL, -2700, 74, 1800},
                                              {OM_WALL, 0,0,0},
                                              {OM_WALL, -3000, 74, 2100},
                                              {OM_WALL, 0,0,0} };
OM_BG_WALL_ST om_ZoneA1_Grid1_Wall_Data[] = { {OM_WALL, -2100, 74, 1800},
                                              {OM_WALL, 0,0,0},
                                              {OM_WALL, 0,0,0},
                                              {OM_WALL, -1800, 74, 2100} };
OM_BG_WALL_ST om_ZoneA1_Grid2_Wall_Data[] = { {OM_WALL, 0,0,0},
                                              {OM_WALL, -2700, 74, 3000},
                                              {OM_START, -3000, 74, 2700},
                                              {OM_WALL, 0,0,0} };
OM_BG_WALL_ST om_ZoneA1_Grid3_Wall_Data[] = { {OM_WALL, 0,0,0},
                                              {OM_WALL, -2100, 74, 3000},
                                              {OM_WALL, 0,0,0},
                                              {OM_WALL, 0,0,0} };

/*** Zone A1 ***/
OM_BG_GRID_ST om_zoneA1_Grid0 = { 0, 0, 0, om_ZoneA1_Grid0_Wall_Data};
OM_BG_GRID_ST om_zoneA1_Grid1 = { 0, 0, 0, om_ZoneA1_Grid1_Wall_Data};
OM_BG_GRID_ST om_zoneA1_Grid2 = { 0, 0, 0, om_ZoneA1_Grid2_Wall_Data};
OM_BG_GRID_ST om_zoneA1_Grid3 = { 0, 0, 0, om_ZoneA1_Grid3_Wall_Data};

/*** Grid data for Zone A1 ***/
OM_BG_GRID_ST *om_zoneA1_Grid[] = { &om_zoneA1_Grid0, &om_zoneA1_Grid1, &om_zoneA1_Grid2, &om_zoneA1_Grid3 };


/*** Coordinate data for walls in Zone A2 ***/
OM_BG_WALL_ST om_ZoneA2_Grid0_Wall_Data[] = { {OM_WALL, -1500, 0, 1800}, 
                                              {OM_WALL, -1500, 0, 2400}, 
                                              {OM_WALL, -1800, 0, 2100}, 
                                              {OM_WALL, 0, 0, 0} };
OM_BG_WALL_ST om_ZoneA2_Grid1_Wall_Data[] = { {OM_WALL, 0, 0, 0},
                                              {OM_WALL, -900, 0, 2400},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0} };
OM_BG_WALL_ST om_ZoneA2_Grid2_Wall_Data[] = { {OM_WALL, -1500, 0, 2400},
                                              {OM_WALL, -1500, 0, 3000},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0} };
OM_BG_WALL_ST om_ZoneA2_Grid3_Wall_Data[] = { {OM_WALL, -900, 0, 2400},
                                              {OM_WALL, -900, 0, 3000},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0} };

/*** Zone A2  ***/
OM_BG_GRID_ST om_zoneA2_Grid0 = { 0, 0, 0, om_ZoneA2_Grid0_Wall_Data};
OM_BG_GRID_ST om_zoneA2_Grid1 = { 0, 0, 0, om_ZoneA2_Grid1_Wall_Data};
OM_BG_GRID_ST om_zoneA2_Grid2 = { 0, 0, 0, om_ZoneA2_Grid2_Wall_Data};
OM_BG_GRID_ST om_zoneA2_Grid3 = { 0, 0, 0, om_ZoneA2_Grid3_Wall_Data};

/*** Grid data for Zone A2 ***/
OM_BG_GRID_ST *om_zoneA2_Grid[] = { &om_zoneA2_Grid0, &om_zoneA2_Grid1, &om_zoneA2_Grid2, &om_zoneA2_Grid3 };


/*** Coordinate data for walls in Zone A3 ***/
OM_BG_WALL_ST om_ZoneA3_Grid0_Wall_Data[] = { {OM_WALL, 0, 0, 0},
                                              {OM_WALL, -300, 0, 2400},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 2100} };
OM_BG_WALL_ST om_ZoneA3_Grid1_Wall_Data[] = { {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 300, 0, 2400},
                                              {OM_WALL, 0, 0, 2100},
                                              {OM_WALL, 0, 0, 0} };
OM_BG_WALL_ST om_ZoneA3_Grid2_Wall_Data[] = { {OM_WALL, -300, 0, 2400},
                                              {OM_WALL, -300, 0, 3000},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0} };
OM_BG_WALL_ST om_ZoneA3_Grid3_Wall_Data[] = { {OM_WALL, 300, 0, 2400},
                                              {OM_WALL, 300, 0, 3000},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0} };

/*** Zone A3  ***/
OM_BG_GRID_ST om_zoneA3_Grid0 = { 0, 0, 0, om_ZoneA3_Grid0_Wall_Data};
OM_BG_GRID_ST om_zoneA3_Grid1 = { 0, 0, 0, om_ZoneA3_Grid1_Wall_Data};
OM_BG_GRID_ST om_zoneA3_Grid2 = { 0, 0, 0, om_ZoneA3_Grid2_Wall_Data};
OM_BG_GRID_ST om_zoneA3_Grid3 = { 0, 0, 0, om_ZoneA3_Grid3_Wall_Data};

/*** Grid data for Zone A3 ***/
OM_BG_GRID_ST *om_zoneA3_Grid[] = { &om_zoneA3_Grid0, &om_zoneA3_Grid1, &om_zoneA3_Grid2, &om_zoneA3_Grid3 };


/*** Coordinate data for walls in Zone A4 ***/
OM_BG_WALL_ST om_ZoneA4_Grid0_Wall_Data[] = { {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 900, 0, 2400},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0} };
OM_BG_WALL_ST om_ZoneA4_Grid1_Wall_Data[] = { {OM_WALL, 1500, 0, 1800},
                                              {OM_WALL, 1500, 0, 2400},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0} };
OM_BG_WALL_ST om_ZoneA4_Grid2_Wall_Data[] = { {OM_WALL, 900, 0, 2400},
                                              {OM_WALL, 900, 0, 3000},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0} };
OM_BG_WALL_ST om_ZoneA4_Grid3_Wall_Data[] = { {OM_WALL, 1500, 0, 2400},
                                              {OM_WALL, 1500, 0, 3000},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0} };

/*** Zone A4 ***/
OM_BG_GRID_ST om_zoneA4_Grid0 = { 0, 0, 0, om_ZoneA4_Grid0_Wall_Data};
OM_BG_GRID_ST om_zoneA4_Grid1 = { 0, 0, 0, om_ZoneA4_Grid1_Wall_Data};
OM_BG_GRID_ST om_zoneA4_Grid2 = { 0, 0, 0, om_ZoneA4_Grid2_Wall_Data};
OM_BG_GRID_ST om_zoneA4_Grid3 = { 0, 0, 0, om_ZoneA4_Grid3_Wall_Data};

/*** Grid data for Zone A4 ***/
OM_BG_GRID_ST *om_zoneA4_Grid[] = { &om_zoneA4_Grid0, &om_zoneA4_Grid1, &om_zoneA4_Grid2, &om_zoneA4_Grid3 };


/*** Coordinate data for walls in Zone A5 **/
OM_BG_WALL_ST om_ZoneA5_Grid0_Wall_Data[] = { {OM_WALL, 2100, 0, 1800},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0} };
OM_BG_WALL_ST om_ZoneA5_Grid1_Wall_Data[] = { {OM_WALL, 2700, 0, 1800},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 3000, 0, 2100} };
OM_BG_WALL_ST om_ZoneA5_Grid2_Wall_Data[] = { {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 2100, 0, 3000},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 0, 0, 0} };
OM_BG_WALL_ST om_ZoneA5_Grid3_Wall_Data[] = { {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 2700, 0, 3000},
                                              {OM_WALL, 0, 0, 0},
                                              {OM_WALL, 3000, 0, 2700} };

/*** Zone A5 **/
OM_BG_GRID_ST om_zoneA5_Grid0 = { 0, 0, 0, om_ZoneA5_Grid0_Wall_Data};
OM_BG_GRID_ST om_zoneA5_Grid1 = { 0, 0, 0, om_ZoneA5_Grid1_Wall_Data};
OM_BG_GRID_ST om_zoneA5_Grid2 = { 0, 0, 0, om_ZoneA5_Grid2_Wall_Data};
OM_BG_GRID_ST om_zoneA5_Grid3 = { 0, 0, 0, om_ZoneA5_Grid3_Wall_Data};

/*** Grid data for Zone A5 ***/
OM_BG_GRID_ST *om_zoneA5_Grid[] = { &om_zoneA5_Grid0, &om_zoneA5_Grid1, &om_zoneA5_Grid2, &om_zoneA5_Grid3 };


OM_BG_ZONE_ST om_ZoneA1 = { "A1", 0, 0, 4, om_zoneA1_Grid };
OM_BG_ZONE_ST om_ZoneA2 = { "A2", 1, 0, 4, om_zoneA2_Grid };
OM_BG_ZONE_ST om_ZoneA3 = { "A3", 2, 0, 4, om_zoneA3_Grid };
OM_BG_ZONE_ST om_ZoneA4 = { "A4", 3, 0, 4, om_zoneA4_Grid };
OM_BG_ZONE_ST om_ZoneA5 = { "A5", 4, 0, 4, om_zoneA5_Grid };


#endif /* _OM_BG_ZONE_A_H_ end */

