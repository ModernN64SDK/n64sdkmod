BINFILES = $(wildcard *.bin)
BINOBJECTS = $(BINFILES:.bin=.bino)
WBKFILES = $(wildcard *.wbk)
WBKOBJECTS = $(WBKFILES:.wbk=.wbko)
PTRFILES = $(wildcard *.ptr)
PTROBJECTS = $(PTRFILES:.ptr=.ptro)
BFXFILES = $(wildcard *.bfx)
BFXOBJECTS = $(BFXFILES:.bfx=.bfxo)

AUDIO_OBJ = $(BFXOBJECTS) $(PTROBJECTS) $(WBKOBJECTS) $(BINOBJECTS)

default: $(AUDIO_OBJ)

%.bino: %.bin
	mips-n64-objcopy -I binary -B mips -O elf32-bigmips $< $@

%.wbko: %.wbk
	mips-n64-objcopy -I binary -B mips -O elf32-bigmips $< $@

%.ptro: %.ptr
	mips-n64-objcopy -I binary -B mips -O elf32-bigmips $< $@

%.bfxo: %.bfx
	mips-n64-objcopy -I binary -B mips -O elf32-bigmips $< $@

clean:
	rm -f $(AUDIO_OBJ)