#ifndef _RcpRdp_h_
#define _RcpRdp_h_

#ifdef __cplusplus
extern "C" {
#endif

#define	STATIC_SEGMENT 1

#ifdef _LANGUAGE_C

#ifndef _SGI_MAKEROM_
extern Gfx *init_rcprdp( Gfx *glistp, char *staticSegment, int num_of_cfb );
#endif

#endif /* _LANGUAGE_C */

#ifdef __cplusplus
}
#endif

#endif
