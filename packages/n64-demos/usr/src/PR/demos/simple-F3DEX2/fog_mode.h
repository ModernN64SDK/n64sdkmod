/*---------------------------------------------------------------------
  File : fog_mode.h

  Coded     by Yoshitaka Yasumoto.   Sep 18, 1996.
  Copyright by Nintendo, Co., Ltd.           1998.
  ---------------------------------------------------------------------*/

/* ZBuffer'ed fog mode for 1 cycle rendering ???? */

#define	G_RM_ZB_1C_FOG_SHADE_A		\
        Z_CMP | Z_UPD | CVG_DST_FULL | ALPHA_CVG_SEL | ZMODE_OPA |	\
        GBL_c1(G_BL_CLR_FOG, G_BL_A_SHADE, G_BL_CLR_IN, G_BL_1MA)

#define	G_RM_ZB_1C_FOG_SHADE_A2		\
        CVG_DST_FULL | FORCE_BL | ZMODE_OPA |	\
        GBL_c2(G_BL_CLR_FOG, G_BL_A_SHADE, G_BL_CLR_IN, G_BL_1MA)

/* End of fog_mode.h */
