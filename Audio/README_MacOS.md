# Install SDL2 development package (MacOS)

```bash
FEEDBACK REQUIRED... I don't have a Mac to know what should go here. If you're a Mac user who knows, please let me know :)
```

# Editing genie.lua (Step 1)

In ``genie.lua`` find the following code block:

```lua
if (os.is("Windows")) then
	WITH_WINMM = 1
elseif (os.is("macosx")) then
	WITH_COREAUDIO = 1
else
	WITH_ALSA = 1
	WITH_OSS = 1
end
```
* Comment out ``WITH_COREAUDIO = 1``
* Comment out ``WITH_ALSA = 1``
* Comment out ``WITH_OSS = 1``

It should look something like this:

```lua
if (os.is("Windows")) then
	WITH_WINMM = 1
elseif (os.is("macosx")) then
	-- 	WITH_COREAUDIO = 1
else
	-- WITH_ALSA = 1
	-- WITH_OSS = 1
end
```

# Editing genie.lua (Step 2)


In ``genie.lua`` find the following code block:

```lua
local sdl_include       = sdl_root .. "/include"
local sdl2_include      = sdl2_root .. "/include"
local sdl2_lib_x86      = sdl2_root .. "/lib/x86"
local sdl2_lib_x64      = sdl2_root .. "/lib/x64"
local dxsdk_include     = dxsdk_root .. "/include"
local portaudio_include = portaudio_root .. "/include"
local openal_include    = openal_root .. "/include"
```

* Set ``sdl_include`` to the include directory output from ``sdl2-config --cflags``
* Set ``sdl2_include`` to the include directory output from ``sdl2-config --cflags``
* Set ``sdl2_lib_x86`` to an empty string
* Set ``sdl2_lib_x64`` to an empty string

It should look something like this:

```lua
local sdl_include       = "/usr/include/SDL2"
local sdl2_include      = "/usr/include/SDL2"
local sdl2_lib_x86      = ""
local sdl2_lib_x64      = ""
local dxsdk_include     = dxsdk_root .. "/include"
local portaudio_include = portaudio_root .. "/include"
local openal_include    = openal_root .. "/include"
```

# Building the library

```
genie --with-sdl2-only gmake
cd gmake
make SoloudStatic
```

The library is now located the ``lib`` directory of the Soloud repo.


# Using Soloud

The following is an example build command which you can adapt to your needs

```bash
clang++ -arch x86_64 -std=c++17 PixelGameEngineApplicationWithSound.cpp -I/path/to/soloud/include -L/path/to/soloud/lib -framework OpenGL -framework GLUT -framework Carbon -lpng -lsoloud_static -o PixelGameEngineApplicationWithSound
```

# Acknowledgements

I'd like to thank the entirety of the OneLoneCoder community for giving me encouragement and confidence. If it weren't for the community, notably it's founder JavidX9, my interest in programming would probably be limited to making simple web pages. My interest in C/C++ has been rekindled and has spread through my entire being like a wildfire.

Further, I'd like to thank JustinRichardsMusic and Xylit. Without their contributions these instructions would not exist.

Thank you all.
