@echo off

set DBGARG=%1
set OUTPUT=soloud.o


IF "%DBGARG%"=="-dbg" goto debug_build 
IF "%DBGARG%"=="-rel" goto release_build
IF "%DBGARG%"=="" goto release_build

:release_build
echo Soloud release build
set BUILD="-O2"
goto build_picked

:debug_build
echo Soloud debug build
set BUILD="-g4"
set OUTPUT="soloud-d.o"
:build_picked


call emcc ^
    src\audiosource\monotone\soloud_monotone.cpp ^
    src\audiosource\openmpt\soloud_openmpt_dll.c ^
    src\audiosource\openmpt\soloud_openmpt.cpp ^
    src\audiosource\sfxr\soloud_sfxr.cpp ^
    src\audiosource\speech\darray.cpp ^
    src\audiosource\speech\klatt.cpp ^
    src\audiosource\speech\resonator.cpp ^
    src\audiosource\speech\soloud_speech.cpp ^
    src\audiosource\speech\tts.cpp ^
    src\audiosource\tedsid\sid.cpp ^
    src\audiosource\tedsid\soloud_tedsid.cpp ^
    src\audiosource\tedsid\ted.cpp ^
    src\audiosource\vic\soloud_vic.cpp ^
    src\audiosource\vizsn\soloud_vizsn.cpp ^
    src\audiosource\wav\stb_vorbis.c ^
    src\audiosource\wav\dr_impl.cpp ^
    src\audiosource\wav\soloud_wav.cpp ^
    src\audiosource\wav\soloud_wavstream.cpp ^
    src\backend\sdl2_static\soloud_sdl2_static.cpp ^
    src\core\soloud.cpp ^
    src\core\soloud_audiosource.cpp ^
    src\core\soloud_bus.cpp ^
    src\core\soloud_core_3d.cpp ^
    src\core\soloud_core_basicops.cpp ^
    src\core\soloud_core_faderops.cpp ^
    src\core\soloud_core_filterops.cpp ^
    src\core\soloud_core_getters.cpp ^
    src\core\soloud_core_setters.cpp ^
    src\core\soloud_core_voicegroup.cpp ^
    src\core\soloud_core_voiceops.cpp ^
    src\core\soloud_fader.cpp ^
    src\core\soloud_fft.cpp ^
    src\core\soloud_fft_lut.cpp ^
    src\core\soloud_file.cpp ^
    src\core\soloud_filter.cpp ^
    src\core\soloud_queue.cpp ^
    src\core\soloud_thread.cpp ^
    src\filter\soloud_bassboostfilter.cpp ^
    src\filter\soloud_biquadresonantfilter.cpp ^
    src\filter\soloud_dcremovalfilter.cpp ^
    src\filter\soloud_echofilter.cpp ^
    src\filter\soloud_fftfilter.cpp ^
    src\filter\soloud_flangerfilter.cpp ^
    src\filter\soloud_lofifilter.cpp ^
    src\filter\soloud_robotizefilter.cpp ^
    src\filter\soloud_waveshaperfilter.cpp ^
    -Iinclude ^
    -DWITH_SDL2_STATIC ^
    -D_stricmp=strcmp ^
    %BUILD% ^
    -s USE_SDL=2 ^
    -r ^
    -o %OUTPUT%
