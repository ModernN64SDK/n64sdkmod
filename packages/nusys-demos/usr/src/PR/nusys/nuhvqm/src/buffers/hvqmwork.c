/*
 * HVQM work area
 *         1999/5/6 Yutaka Murakami
 */
#include "hvqm.h"

/*
 * When using a mix of two formats, you must secure a work area for
 * the 4:2:2 format.  However, this time it is for 4:1:1
 */
u16 hvqmWorkspace[(HVQM_MAXWIDTH/8)*(HVQM_MAXHEIGHT/8)*6];

/*
 * PCM data buffer
 *     See hvqm.h for the size
 */
s16 hvqmPcmBuf[HVQM_PCM_BUF_NUM][HVQM_PCM_SAMPLES_MAX];
