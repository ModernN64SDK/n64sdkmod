
#define __MALLOC_C__

/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	Memory manager
 :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */

#include "pack.h"

/* ===========================================================================
	static variables
 =========================================================================== */
static MEMCTRL globalmem;
static MEMHEADER mallocspace[ MALLOC_SPACE ];

/* ===========================================================================
	public functions
 =========================================================================== */

/* ----------------------------------------------------------------------------
  Initialize memory manager
---------------------------------------------------------------------------- */
void minit(void)
{
    MEMHEADER *p;

    globalmem.topadrs = globalmem.searchtop = p = &mallocspace[0];
    globalmem.endadrs = &mallocspace[MALLOC_SPACE-1];

    p->flag = 0;
    p->length = sizeof(MEMHEADER) * MALLOC_SPACE;
}

/* ----------------------------------------------------------------------------
   reserve memory
   return memory with an 8-byte boundary
---------------------------------------------------------------------------- */
void *amalloc(int want, void *ap)
{
    MEMCTRL *mcp = ap;
    MEMHEADER *p,*ep, *np;
    u32 len8, want8, bytelen;

    /* Detect the head location of data area which is not used in arena area. */
    ep = mcp->endadrs;
    want8 = OCTARY(want) + 1;	/* consider the amount of MEMCTRL */
    for(p=mcp->searchtop; p<ep; p+=len8){
	len8 = OCTARY(p->length);
	if((!BITTEST(p->flag, MHFLG_USED)) && (len8 >= want8))
	  break;	/* detected */
    }
    if(p>=ep){
	/* no free space! */
	return NULL;
    }

    /*Divide memory area */
    if(len8 > want8+1){
	bytelen = want8 * sizeof(MEMHEADER);
	np = &p[want8];
	np->length = p->length - bytelen;
	np->flag = 0;
	p->length = bytelen;
    }

    /* Move detection start point? */
    if(mcp->searchtop == p)
      mcp->searchtop = &p[OCTARY(p->length)];

    /* Activate reserved flag */
    BITSET(p->flag, MHFLG_USED);
    np = &p[OCTARY(p->length)];
    BITSET(np->flag, MHFLG_BEFOREUSED);

    return ++p;	/* next of header is correct memory */
}

/* ----------------------------------------------------------------------------
   Release memory
---------------------------------------------------------------------------- */
void afree( void *ptr, void *ap )
{
    MEMCTRL *mcp = ap;
    MEMHEADER *p, *np;

    if(!ptr) return;
    p = &((MEMHEADER*)ptr)[-1];	 /*  MEMHEADER pointer is just before that */

    /* Is it possible to merge into the block just before? */
    /* This process is expensive.  Something should be done.*/
    if(p != mcp->topadrs && !BITTEST(p->flag, MHFLG_BEFOREUSED)){
	MEMHEADER *tp;
	for(tp=mcp->topadrs; (np = &tp[OCTARY(tp->length)])!=p; tp=np)
	  ;
	tp->length += p->length;
	p = tp;
    }

    /* Is it possible to merge into the block just after? */
    np = &p[OCTARY(p->length)];
    if(!BITTEST(np->flag, MHFLG_USED))
      p->length += np->length;

    /* No change in detection start pointer? */
    if((u32)mcp->searchtop > (u32)p)
      mcp->searchtop = p;

    /*Set a flag */
    BITRESET(p->flag, MHFLG_USED);
    p = &p[OCTARY(p->length)];
    BITRESET(p->flag, MHFLG_BEFOREUSED);
}

/* ----------------------------------------------------------------------------
  Reserve memory
  Return memory with a 8-byte boundary
---------------------------------------------------------------------------- */
void *malloc(int want)
{
    return amalloc( want, &globalmem );
}

/* ----------------------------------------------------------------------------
Release memory
---------------------------------------------------------------------------- */
void free( void *ptr )
{
    afree( ptr, &globalmem );
}

/* ----------------------------------------------------------------------------
   Reserve arena
---------------------------------------------------------------------------- */
void *acreate(int want)
{
    int bytelen, len;
    MEMCTRL *mcp;
    MEMHEADER *p;

    len = OCTARY(want);
    bytelen = len * 8;
    mcp = malloc(sizeof(MEMCTRL));
    mcp->topadrs = mcp->searchtop = p = malloc(bytelen);
    mcp->endadrs = &mcp->topadrs[ len ];

    p->flag = 0;
    p->length = bytelen;

    return mcp;
}

/* ----------------------------------------------------------------------------
Release arena
---------------------------------------------------------------------------- */
void adelete(void *ap)
{
    MEMCTRL *mcp = ap;

    free( mcp->topadrs );
    free( mcp );
}

