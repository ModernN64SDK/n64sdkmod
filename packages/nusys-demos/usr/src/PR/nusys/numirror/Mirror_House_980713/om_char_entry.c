/*======================================================================*/
/*	Mirror House							*/
/*	om_char_entry.c							*/
/*									*/
/*	Copyright (C) 1998 MONEGI CORPORATION				*/
/*									*/
/*======================================================================*/
#include "pack.h"

#include "nd.h"
#include "nusys.h"

#include "om.h"
#include "om_info.h"
#include "om_data.h"
#include "om_common.h"

/* ========================================================================== 
	define
 ========================================================================== */

/* ===========================================================================
	External Reference Function Declarations 
 =========================================================================== */
extern void om_com_character_cur_instance_change(OM_CHARACTER_DATA *data, int nd_no);
extern void om_com_character_globalresource_change(OM_CHARACTER_DATA *data);
extern OM_CHARACTER_DATA *om_com_charast_chain_search(OM_CHARACTER_ST *st, u32 entry_number);
extern void om_bg_zone_coordinate_get(OM_CHARACTER_DATA *data, u32 *zone_x, u32 *zone_y, u32 *zone_z);

/* ===========================================================================
	Function Prototype Declarations 
 =========================================================================== */
void om_chara_anim_entry(OM_CHARACTER_ST *st, OM_CHARACTER_INFO *inf);
void om_characterst_init(OM_CHARACTER_DATA *st, OM_CHARACTER_INFO_DATA *stinf);
static void om_animation_entry(OM_CHARACTER_ST *st);
static void om_characterst_header_init(OM_CHARACTER_ST *st, OM_CHARACTER_INFO *inf);
static int om_character_entry(OM_CHARACTER_ST *st, OM_CHARACTER_INFO *inf);
void om_global_instance_list_save(OM_NDD_INSTPLIST *instp_list);
void om_global_resource_list_save(OM_NDD_INSTPLIST *instp_list);
void om_resource_entry(OM_NDD_INSTPLIST *instp_list, NDR_RESINFO **ndlist, int nd_list);

/* ===========================================================================
	Function Definitions 
 =========================================================================== */
void om_chara_anim_entry(OM_CHARACTER_ST *st, OM_CHARACTER_INFO *inf)
{
    om_character_entry(st, inf);
    om_animation_entry(st);
    st->header.data_entry_flag = 1;
}

static void om_animation_entry(OM_CHARACTER_ST *st)
{
    u32 i, max_entry;
    OM_CHARACTER_DATA *data;

    data = st->data;
    max_entry = st->header.character_entry;
    for(i=0; i<max_entry; i++)
    {
        om_com_character_cur_instance_change(data, 0);
        om_com_character_globalresource_change(data);
#if 0
        ndAttachAnimation(data->cur_instp->anim_root_instp, data->cur_instp->instp, 1, 0, 0x1000, 2);
#endif
        data = data->next;
    }
}

void om_characterst_init(OM_CHARACTER_DATA *data, OM_CHARACTER_INFO_DATA *stinf)
{
    u32 zone_x, zone_y, zone_z;

    data->xi = data->x = stinf->x;
    data->yi = data->y = stinf->y;
    data->zi = data->z = stinf->z;
    data->rx = stinf->rx;
    data->ry = stinf->ry;
    data->rz = stinf->rz;
    data->r  = stinf->r;
    data->action_flg = 0;
    data->walk_flg = 0;
    data->dsp_status = stinf->dsp_status;
    data->entry_number = stinf->entry_number;
    om_bg_zone_coordinate_get(data, &zone_x, &zone_y, &zone_z);
    data->deform_flg = 0;
}

static void om_characterst_header_init(OM_CHARACTER_ST *st, OM_CHARACTER_INFO *inf)
{
    om_com_memcpy((char*)&st->header, (char*)inf->header, sizeof(OM_HEADER_ST));
}


static int om_character_entry(OM_CHARACTER_ST *st, OM_CHARACTER_INFO *inf)
{
    u32 i, max_entry;
    OM_CHARACTER_DATA *stdata, *stch_top, *stch=(OM_CHARACTER_DATA*)NULL;
    OM_CHARACTER_INFO_DATA *infdata;
    OM_NDD_INSTPLIST       *instp_list;

    om_characterst_header_init(st, inf);
    max_entry = inf->header->character_entry;

    infdata = inf->data;
    for(i=0; i<max_entry;i++)
    {
        if (infdata->ndlist != (NDR_RESINFO**)NULL) {
            stdata = (OM_CHARACTER_DATA*)malloc(sizeof(OM_CHARACTER_DATA));
            if (stdata == (OM_CHARACTER_DATA*)NULL)
                return -1;

            /* Save the next list pointer in the structure OM_CHARACTER_ST */
            if (stch != (OM_CHARACTER_DATA*)NULL)
                stch->next = stdata;
            else
                stch_top = stdata;

            stdata->instp_list = (OM_NDD_INSTPLIST*)malloc(sizeof(OM_NDD_INSTPLIST)*infdata->nd_list);
            instp_list = stdata->instp_list;
            if (instp_list != (OM_NDD_INSTPLIST*)NULL) {
                /* Enter resource data corresponding to the number of resource data items */
                om_resource_entry(instp_list, infdata->ndlist, infdata->nd_list);

                /* Initialize the structure OM_CHARACTER_ST */
                om_characterst_init(stdata, infdata);

                infdata++;
                /* To make a OM_CHARACTER_ST chain beginning from the next loop,*/
                /* save the previous list pointer. */
                stch = stdata;;
            }
        } else {
            break;
        }
    }
    /* NULL stop the pointer following the final list pointer */
    stch->next = (OM_CHARACTER_DATA*)NULL;
    st->data = stch_top;
    return 0;
}

void om_global_instance_list_save(OM_NDD_INSTPLIST *instp_list)
{
    /* Enter the instance pointer nd in the structure OM_CHARACTER_ST */
    /* xxnd holds the following instance pointers globally and can hold only 1 instance pointer at a time.xx  Thus, when multiple characters are displayed, they must be held in separate tables. */
   
    instp_list->root_instp        = root_instp;
    instp_list->anim_root_instp   = anim_root_instp;
    instp_list->obj_root_instp    = obj_root_instp;
    instp_list->deform_root_instp = deform_root_instp;
    instp_list->dynvtx_root_instp = dynvtx_root_instp;
}

void om_global_resource_list_save(OM_NDD_INSTPLIST *instp_list)
{
    /* Save the character object resource list and */
    /* the animation resource list */
    instp_list->objlist       = objlist;
    instp_list->animlist      = animlist;
    instp_list->rawvtxlist    = rawvtxlist;
    instp_list->rawvtxcollist = rawvtxcollist;
    instp_list->rawvtxnvlist  = rawvtxnvlist;
}

void om_resource_entry(OM_NDD_INSTPLIST *instp_list, NDR_RESINFO **ndlist, int nd_list)
{
    int j;
    NDD_RESINFO             *resource;

    for(j=0; j<nd_list; j++)
    {
        ndInit();
        ndResInit( MAX_OBJ, MAX_SHAPE, MAX_ANIM, 
                   MAX_RAWVTX, MAX_RAWVTXCOL, MAX_RAWVTXNV);

        /* Resource entry */
        instp_list->cur_resource = resource = ndResEntry(ndlist[j], NDC_RES_USED_ALL);

        om_global_resource_list_save(instp_list);

        instp_list->instp = ndEntryObject( NULL, 1, resource->obj.ofs,
                                           resource->shape.ofs,
                                           resource->anim.ofs,
                                           (OM_PREVIEW_TYPE & 1)+2);

        /* Save the resource instance pointers reserved in the ND library */
        om_global_instance_list_save(instp_list);
        instp_list++;
    }
}

