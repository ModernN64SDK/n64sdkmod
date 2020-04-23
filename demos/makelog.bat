:/ This is a batch file to make all the samples of ultra.

cd autofill
make > %ROOT%\usr\src\pr\demos\appslog
if errorlevel 1 goto done
cd ..
cd blockmonkey
make >> %ROOT%\usr\src\pr\demos\appslog
if errorlevel 1 goto done
cd ..
cd bumpmap
make >> %ROOT%\usr\src\pr\demos\appslog
if errorlevel 1 goto done
cd ..
cd chrome
make >> %ROOT%\usr\src\pr\demos\appslog
if errorlevel 1 goto done
cd ..
cd ci8fb
make >> %ROOT%\usr\src\pr\demos\appslog
if errorlevel 1 goto done
cd ..
cd detail
make >> %ROOT%\usr\src\pr\demos\appslog
if errorlevel 1 goto done
cd ..
cd eeptest
make >> %ROOT%\usr\src\pr\demos\appslog
if errorlevel 1 goto done
cd ..
cd fault
make >> %ROOT%\usr\src\pr\demos\appslog
if errorlevel 1 goto done
cd ..
cd flash
make >> %ROOT%\usr\src\pr\demos\appslog
if errorlevel 1 goto done
cd ..
cd fogworld
make >> %ROOT%\usr\src\pr\demos\appslog
if errorlevel 1 goto done
cd ..
cd gbpak
make >> %ROOT%\usr\src\pr\demos\appslog
if errorlevel 1 goto done
cd ..
cd ginv
make >> %ROOT%\usr\src\pr\demos\appslog
if errorlevel 1 goto done
cd ..
cd ground
make >> %ROOT%\usr\src\pr\demos\appslog
if errorlevel 1 goto done
cd ..
cd gs2dex
make >> %ROOT%\usr\src\pr\demos\appslog
if errorlevel 1 goto done
cd ..
cd lights
make >> %ROOT%\usr\src\pr\demos\appslog
if errorlevel 1 goto done
cd ..
cd mipmap
make >> %ROOT%\usr\src\pr\demos\appslog
if errorlevel 1 goto done
cd ..
cd morphcube
make >> %ROOT%\usr\src\pr\demos\appslog
if errorlevel 1 goto done
cd ..
cd morphfaces
make >> %ROOT%\usr\src\pr\demos\appslog
if errorlevel 1 goto done
cd ..
cd motortest
make >> %ROOT%\usr\src\pr\demos\appslog
if errorlevel 1 goto done
cd ..
cd nosPak
call makenos >> %ROOT%\usr\src\pr\demos\appslog
if errorlevel 1 goto done
cd ..
cd onetri
make >> %ROOT%\usr\src\pr\demos\appslog
if errorlevel 1 goto done
cd ..
cd onetri-fpal
make >> %ROOT%\usr\src\pr\demos\appslog
if errorlevel 1 goto done
cd ..
cd overlay
make >> %ROOT%\usr\src\pr\demos\appslog
if errorlevel 1 goto done
cd ..
cd pfs
make >> %ROOT%\usr\src\pr\demos\appslog
if errorlevel 1 goto done
cd ..
cd playseq
make >> %ROOT%\usr\src\pr\demos\appslog
if errorlevel 1 goto done
cd ..
cd playseq.naudio
make >> %ROOT%\usr\src\pr\demos\appslog
if errorlevel 1 goto done
cd ..
cd polyline
make >> %ROOT%\usr\src\pr\demos\appslog
if errorlevel 1 goto done
cd ..
cd soundmonkey
make >> %ROOT%\usr\src\pr\demos\appslog
if errorlevel 1 goto done
cd ..
cd spgame
make >> %ROOT%\usr\src\pr\demos\appslog
if errorlevel 1 goto done
cd ..
cd spritemonkey
make >> %ROOT%\usr\src\pr\demos\appslog
if errorlevel 1 goto done
cd ..
cd spritemonkeyEX
make >> %ROOT%\usr\src\pr\demos\appslog
if errorlevel 1 goto done
cd ..
cd sramtest
make >> %ROOT%\usr\src\pr\demos\appslog
if errorlevel 1 goto done
cd ..
cd terrain
make >> %ROOT%\usr\src\pr\demos\appslog
if errorlevel 1 goto done
cd ..
cd texlight
make >> %ROOT%\usr\src\pr\demos\appslog
if errorlevel 1 goto done
cd ..
cd threadmonkey
make >> %ROOT%\usr\src\pr\demos\appslog
if errorlevel 1 goto done
cd ..
cd tile_rect2d
make >> %ROOT%\usr\src\pr\demos\appslog
if errorlevel 1 goto done
cd ..
cd tile_rect3d
make >> %ROOT%\usr\src\pr\demos\appslog
if errorlevel 1 goto done
cd ..
cd topgun
make >> %ROOT%\usr\src\pr\demos\appslog
if errorlevel 1 goto done
cd ..
cd tron
make >> %ROOT%\usr\src\pr\demos\appslog
if errorlevel 1 goto done
cd ..
cd voice
make >> %ROOT%\usr\src\pr\demos\appslog
if errorlevel 1 goto done
cd ..
cd ddspgame
make >> %ROOT%\usr\src\pr\demos\appslog
if errorlevel 1 goto done
cd ..
cd reboot
make >> %ROOT%\usr\src\pr\demos\appslog
if errorlevel 1 goto done
cd ..
:done
