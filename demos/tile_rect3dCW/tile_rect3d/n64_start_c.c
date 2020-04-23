// N64_start.c
#include <__rts_info_t__.h>

#ifdef __cplusplus
extern "C" asm void __N64start(void);
#else
asm void __N64start(void);
#endif

void boot();

asm void __N64start(void) 
{
	.set	noreorder
	la		t0,_fbss
	la		t1,_bss_size	/* .bss size					*/
loop:
	sw		$0, 0(t0)
	sw		$0, 4(t0)
	addi	t0, t0, 8
	addi	t1, t1, -8
	bne		t1, 0, loop
	nop
	la		t2, boot		/* load boot address */
	la		sp, _stack_addr
	
	jr		t2
	nop
}
