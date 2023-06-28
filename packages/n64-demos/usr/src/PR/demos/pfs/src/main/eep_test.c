
/*---------------------------------------------------------------------*
        Copyright (C) 1997 Nintendo. (Originated by SGI)
        
        $RCSfile: eep_test.c,v $
        $Revision: 1.3 $
        $Date: 1998/09/25 21:50:23 $
 *---------------------------------------------------------------------*/

#include	<ultra64.h>
#include	"thread.h"
#include        "textlib.h"

extern OSMesgQueue retraceMessageQ;
extern OSMesgQueue  pifMesgQueue;
extern u8 err_mes[];


void	
eep_test(void)
{	
	s32 ret = 0;
	int i;
	u8 long_buffer[EEPROM_MAXBLOCKS * EEPROM_BLOCK_SIZE];
	int error;

	mes_init(err_mes);
	for (i  = 0; i < EEPROM_BLOCK_SIZE * EEPROM_MAXBLOCKS; i++) {
		long_buffer[i] = i & 0xff;
	}
	ret = osEepromLongWrite(&pifMesgQueue, 0, long_buffer,
				EEPROM_BLOCK_SIZE * EEPROM_MAXBLOCKS);
	if (ret == 0) {
		mes_add("EEPROM long write success \n");
	} else {
		mes_add("EEPROM long write fail: \n");
	}
	ret = osEepromLongRead(&pifMesgQueue, 0, long_buffer,
			       EEPROM_BLOCK_SIZE * EEPROM_MAXBLOCKS);
	if (ret == 0) {
		error = 0;
		for (i = 0; i < EEPROM_BLOCK_SIZE * EEPROM_MAXBLOCKS; i++) {
			if (long_buffer[i] != (i & 0xff)) {
				error++;
			}
		}
		if (error == 0) {
		        mes_add("\tEEPROM long read success \n");
		} else {
			mes_add("\tEEPROM long read error \n");
		      }
	} else {
		mes_add("\tEEPROM long read fail: \n");
	}
#ifdef jean
	for(no = 0 ; no < EEPROM_MAXBLOCKS ; no++) {
		for (i = 0 ; i < EEPROM_BLOCK_SIZE; i++)
			test_buffer[i] = no;
		ret = osEepromWrite(&pifMesgQueue, no, 
				test_buffer);
		if (ret == 0) {
			setcolor(0xa0,0x20,0x80);
			draw_puts("\t\t\twrite success: ");
			draw_ushort(no);
			draw_char('\n');
		} else {
			setcolor(0xa0,0x20,0x80);
			draw_puts("\t\t\twrite fail: ");
			draw_ushort(no);
			draw_char('\n');
		}
	}
	for(no = 0 ; no < EEPROM_MAXBLOCKS ; no++) {
		int error = 0;

		ret = 0;
		ret = osEepromRead(&pifMesgQueue, no, 
				test_buffer);
		if (ret == 0) {
			error = 0;
			for (i = 0 ; i < EEPROM_BLOCK_SIZE; i++){
				if (test_buffer[i] != no) {
					error++;
				}
			}
			setcolor(0xa0,0x20,0x80);
			if (error == 0) {
				draw_puts("\t\t\t\t\t\tread success: ");
				draw_ushort(no);
				draw_char('\n');
			} else {
				draw_puts("\t\t\t\t\t\tread error: ");
				draw_ushort(no);
				draw_char('\n');
			}
		} else {
			setcolor(0xa0,0x20,0x80);
			draw_puts("\t\t\t\t\t\t\read fail: ");
			draw_ushort(no);
			draw_char('\n');
		}
	}
#endif
	draw_puts("\t\n");
}
