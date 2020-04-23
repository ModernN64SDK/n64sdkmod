:/ This is a batch file to make all the samples of ultra.
cd autofill
make
if errorlevel 1 goto done
cd ..
cd blockmonkey
make
if errorlevel 1 goto done
cd ..
cd bumpmap
make
if errorlevel 1 goto done
cd ..
cd chrome
make
if errorlevel 1 goto done
cd ..
cd ci8fb
make
if errorlevel 1 goto done
cd ..
cd detail
make
if errorlevel 1 goto done
cd ..
cd eeptest
make
if errorlevel 1 goto done
cd ..
cd fault
make
if errorlevel 1 goto done
cd ..
cd flash
make
if errorlevel 1 goto done
cd ..
cd fogworld
make
if errorlevel 1 goto done
cd ..
cd gbpak
make
if errorlevel 1 goto done
cd ..
cd ginv
make
if errorlevel 1 goto done
cd ..
cd ground
make
if errorlevel 1 goto done
cd ..
cd gs2dex
make
if errorlevel 1 goto done
cd ..
cd lights
make
if errorlevel 1 goto done
cd ..
cd mipmap
make
if errorlevel 1 goto done
cd ..
cd morphcube
make
if errorlevel 1 goto done
cd ..
cd morphfaces
make
if errorlevel 1 goto done
cd ..
cd motortest
make
if errorlevel 1 goto done
cd ..
cd nosPak
call makenos
if errorlevel 1 goto done
cd ..
cd onetri
make
if errorlevel 1 goto done
cd ..
cd onetri-fpal
make
if errorlevel 1 goto done
cd ..
cd overlay
make
if errorlevel 1 goto done
cd ..
cd pfs
make
if errorlevel 1 goto done
cd ..
cd playseq
make
if errorlevel 1 goto done
cd ..
cd playseq.naudio
make
if errorlevel 1 goto done
cd ..
cd polyline
make
if errorlevel 1 goto done
cd ..
cd soundmonkey
make
if errorlevel 1 goto done
cd ..
cd spgame
make
if errorlevel 1 goto done
cd ..
cd spritemonkey
make
if errorlevel 1 goto done
cd ..
cd spritemonkeyEX
make
if errorlevel 1 goto done
cd ..
cd sramtest
make
if errorlevel 1 goto done
cd ..
cd terrain
make
if errorlevel 1 goto done
cd ..
cd texlight
make
if errorlevel 1 goto done
cd ..
cd threadmonkey
make
if errorlevel 1 goto done
cd ..
cd tile_rect2d
make
if errorlevel 1 goto done
cd ..
cd tile_rect3d
make
if errorlevel 1 goto done
cd ..
cd topgun
make
if errorlevel 1 goto done
cd ..
cd tron
make
if errorlevel 1 goto done
cd ..
cd voice
make
if errorlevel 1 goto done
cd ..
cd ddspgame
make
if errorlevel 1 goto done
cd ..
cd reboot
make
if errorlevel 1 goto done
cd ..
:done

