/*======================================================================*/
/*      OBJECT MANAGER                                                  */
/*          om.h                                                        */
/*                                                                      */
/*      Copyright (C) 1998, NINTENDO Co,Ltd.                            */
/*                                                                      */
/*======================================================================*/
#include    "nd.h"
#include    "om_camera.h"
#include    "om_background.h"
#include    "om_bg.h"

#ifndef _OM_H_
#define _OM_H_

/* ----------------------------------------------------------------------------
    Program System Structure
---------------------------------------------------------------------------- */
typedef struct {
    int     seq_no;
} OM_PROGRAM_SEQ_ST;


/* ----------------------------------------------------------------------------
    Header Structure
---------------------------------------------------------------------------- */
typedef struct {
    int          data_entry_flag;    /* Data entry flag */
    u32          character_entry;    /* Number of character entries */
} OM_HEADER_ST;


/* ----------------------------------------------------------------------------
    Game Character Data Structures
---------------------------------------------------------------------------- */
typedef struct  om_ndd_instp {
    NDD_RESINFO  *cur_resource;         /* Value returned by ndResEntry() */
    NDD_INSTANCE *root_instp;           /* Root instance pointer */
    NDD_INSTANCE *obj_root_instp;       /* Object entry root */
    NDD_INSTANCE *anim_root_instp;      /* Animation entry root */
    NDD_INSTANCE *deform_root_instp;    /* Deform entry root */
    NDD_INSTANCE *dynvtx_root_instp;    /* Dynamic vertex entry root */
    NDD_INSTANCE *instp;                /* Value returned by ndEntryObject() */
    NDR_OBJ      **objlist;             /* Object resource entry list */
    NDR_ANIM     **animlist;            /* Animation resource entries */
    NDR_SHAPE    **shapelist;
    s16          **rawvtxlist;
    u32          **rawvtxcollist;
    s8           **rawvtxnvlist;
} OM_NDD_INSTPLIST;

typedef struct om_character_data {      /* Structure containing data for each character */
    u32     entry_number;               /* Character number */
    OM_NDD_INSTPLIST *cur_instp;        /* Pointer to the instance of the character currently being rendered */
    OM_NDD_INSTPLIST *instp_list;       /* Pointers (*3) to the NDD_INSTANCE list created by initialization  */
    float   x, y, z;                    /* Center position */
    float   xb, yb, zb;
    float   rx, ry, rz;                 /* Rotation angles for the x, y, and z axes */
    float   xi, yi, zi;                 /* Initial x, y, and z coordinates of the character */
    float   r;                          /* Radius from the center coordinates of the character */
    int     time_cnt;
    int     action_flg;
    int     walk_flg;
    int     control_no;                 /* Controller number of the current controller */
    int     dsp_status;                 /* Flag for display/no-display status */
    u32     area_number;                /* Map area number where the character exists */
    u32     zone_x, zone_z;             /*  Map zone numbers where the character exists */
    u32     deform_flg;                 /* Deform flag */
    struct om_character_data *next;     /* Pointer to the next data list*/
} OM_CHARACTER_DATA;

typedef struct {
    OM_HEADER_ST            header;     /* Header section */
    OM_CHARACTER_DATA       *data;      /* Character data section for the game  */
} OM_CHARACTER_ST;


/* ----------------------------------------------------------------------------
    Hit Checking Structure 
---------------------------------------------------------------------------- */
typedef struct {
    float x, y, z;
    float r;
} OM_HIT_CHECK_ST;

/* ----------------------------------------------------------------------------
    define
---------------------------------------------------------------------------- */
/*** Program Sequence Number  ***/
#define OM_SYSTEM_INIT      0             /* System initialization */
#define OM_OPENING_INIT     1
#define OM_GAME_TITLE       2             /* Game title */
#define OM_GAME_DATA_ENTRY  3             /* Game data entry */
#define OM_GAME_SELECT      4             /* Character selection */
#define OM_GAME_INIT        5             /* Game data initialization */
#define OM_GAME_MAIN        6             /* Game main function */
#define OM_STANDBY         99             /* Waiting */

#define OM_NODISPLAY        0             /* 3D model no-display flag */
#define OM_DISPLAY          1             /* 3D model display flag  */

#define MAX_OBJ           50
#define MAX_SHAPE         50
#define MAX_ANIM          50
#define MAX_RAWVTX        50
#define MAX_RAWVTXCOL     50
#define MAX_RAWVTXNV      50
#define OM_PREVIEW_TYPE   28

#define OM_STAGE_CLEAR_TIME 1200

/* ----------------------------------------------------------------------------
    Rendering Mode 
---------------------------------------------------------------------------- */
#define OM_RENDERMODE_MASK  0x3                     /*  Mask data for producing rendering mode */
#define OM_RENDERMODE_XLU   0x1                     /*  Translucent rendering */
#define OM_RENDERMODE_OPA   0x0                     /*  Opaque rendering */
#define OM_BG_2D            0x10                    /*  Perform 2D processing in BACKGROUND */


/* ===========================================================================
	Global Structure Declarations 
 =========================================================================== */
#ifdef _MAIN_

EXCONTPAD *pad;
OM_CHARACTER_ST       om_character_st;       /* Character data structure */
OM_CAMERA_ST	      *om_camera_st;         /* Camera data structure */
OM_PROGRAM_SEQ_ST     om_program_seq_st;     /* Program sequence structure */
OM_BG_STAGE_ST        *om_bg_stage_st;       /* Stage data structure */

/* ===========================================================================
	Global Variable Declarations
 =========================================================================== */

#endif

#endif

