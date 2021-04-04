# Make all samples without N_AUDIO

ifeq ($(NAUDIO),1)
default:
	make -C crossfade NAUDIO=1
	make -C effects NAUDIO=1
	make -C fromram NAUDIO=1
	make -C fxchange NAUDIO=1
	make -C fxsong NAUDIO=1
	make -C lookup NAUDIO=1
	make -C marker NAUDIO=1
	make -C nnsched NAUDIO=1
	make -C pause NAUDIO=1
	make -C simple/before
	make -C simple/after NAUDIO=1
	make -C testmenu NAUDIO=1
else
default:
	make -C crossfade
	make -C effects
	make -C fromram
	make -C fxchange
	make -C fxsong
	make -C lookup
	make -C marker
	make -C nnsched
	make -C pause
	make -C simple/before
	make -C simple/after
	make -C testmenu
endif

clean:
	make clean -C crossfade
	make clean -C effects
	make clean -C fromram
	make clean -C fxchange
	make clean -C fxsong
	make clean -C lookup
	make clean -C marker
	make clean -C nnsched
	make clean -C pause
	make clean -C simple/before
	make clean -C simple/after
	make clean -C testmenu
