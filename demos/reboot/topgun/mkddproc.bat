@echo off
set DDPROC=..\ddproc
if ""=="%ROOT%" goto error2
if ""=="%DDPROC%" goto error1
rem touch %DDPROC%\video_core.c
if ""=="%1" goto normal

cd   %DDPROC%
make START_FROM_CASSETTE=YES FINAL=YES
cd   ..\topgun
copy %DDPROC%\ddproc.o .

goto end

:normal

cd   %DDPROC%
make START_FROM_CASSETTE=YES
cd   ..\topgun
copy %DDPROC%\ddproc.o .

goto end
:error1
echo The DDPROC environment variable has not been set.
goto end
:error2
echo This is not the N64 program development environment.
goto end
:end
@echo on
