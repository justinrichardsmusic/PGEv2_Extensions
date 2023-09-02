# Audio Listener / Source PGEX

What is it?
-----------

Adds the ability to play sounds / music using the olc::PixelGameEngine.
The Audio Listener functions as a set of ears to hear sounds in the game
and its position can be adjusted away or closer to Audio Sources so that
panning / volume can be adjusted accordingly.

How does it work?
-----------------

It makes use of the SoLoud cross platform audio library which you can find
out more about here:

https://sol.gfxile.net/soloud/

SoLoud provides several backend implementation options across many platforms
and best of all, it can be configured to compile with emscripten to run in the
web browser as well :-)

Download Instructions
------------------

Unlike my other PGEX options, this one does have some dependencies and will require
some additional setup.

First things first, you'll need to download the source files to SoLoud.  You can find
them here:

https://sol.gfxile.net/soloud/downloads.html

Extract them somewhere on your drive so you can access them when needed (they don't need
to be in your project directory in most situations so keep them separate for now).

Building a Backend for Windows
------------------------------

In order to NOT have to include all of the SoLoud source files in our project we will need
to first build a backend (static library) to include instead.  This will make things much
simpler in our project as we don't need to modify any of the SoLoud source code anyway.

Open a command prompt (as admininstrator) and browse to the \build folder of the SoLoud
installation.  Now run 

    Genie /?

to see a list of the various options you have for backends
etc...

--------------------------------------------

[NOTE] It has come to my attention that the most recent updates for SoLoud no longer include
the GENie.exe file... You will now need to download it from the github below (see download
section of the Readme file there)

https://github.com/bkaradzic/GENie

Once downloaded, place the GENie.exe file in the /build folder specified above and continue on...

---------------------------------------------

You can of course choose one that suits your needs, however I'm going to go with SDL2 as it
is the most common / tested backend available and it is cross platform and works in the
web browser also.

(If you don't have SDL2 installed you will need to download and install it somewhere first!)
(Note: If you aren't intending on building for Emscripten then you can use MiniAudio which
 doesn't require any additional downloads, and skip the SDL2 parts)

Before running the Genie command you will need to first edit the genie.lua file to point to
where you have your installation of SDL2. For example;

    local sdl2_root      = "/libraries/sdl2/"
    local sdl2_lib_x86   = "/libraries/sdl2/lib/x86/"
    local sdl2_lib_x64   = "/libraries/sdl2/lib/x64/"

Save the lua file and return to the command prompt.

Now run Genie again as follows (for Windows using vs2019) - SDL2 BACKEND

    genie --with-sdl2static-only vs2019
    
-or-

Run Genie as follows (for Windows using vs2019) - MINIAUDIO BACKEND (not for use with Emscripten!)

    genie --with-miniaudio-only vs2019

This will generate the necessary project files in the build directory in a new \vs2019
directory.

Now open the "SoloudStatic.vcxproj" in Visual Studio.  Set it to Release Mode (x86) and build the
static version (right click the soloudStatic project and select Build).  You should now
see a \lib folder in the main SoLoud directory. This will now contain the soloud_static.lib
file.  This is the file that will be needed as a replacement for the SoLoud source code.

(Note: x86 build confirmed working, x64 may not be compatible at this stage)

Building and Compiling SoLoud / SDL Backend for Linux or MacOS environment (Optional)
---------------------------------------------------------------------------------------------

Thanks to the hard work of Moros1138 and Xylit on the discord server for providing detailed (verified)
instructions for getting this working on Linux and MacOS :-)

If you are building for linux please follow the instructions in the Readme_Linux.md file in this repo
instead of the Windows version above and then continue on once successfully completed...

If you are building for MacOS please follow the instructions in the Readme_MacOS.md file in this repo
instead of the Windows version above and then continue on once successfully completed...

Configuring Visual Studio for Desktop Use with SoLoud
-----------------------------------------------------

I happen to use Visual Studio on Windows, however these instructions can be adapted to suit
other environments also.

Firstly, create a bare bones PGE project.

Next, open the project properties (make sure you have All Configurations and All Platforms
selected before making the next changes).

Under C/C++ in the Additional Include Directories field, add in the location of your
SoLoud \include folder.  This will allow the Audio Listener to access the SoLoud header
information.

Under Linker/Input in the Additional Dependencies field, add in 

        soloud_static.lib; sdl2.lib; sdl2main.lib; sdl2_mixer.lib;
        
This will link the compiled soloud library at runtime as well as the various sdl libraries
needed for desktop development.

However, we still need to place a copy of the soloud_static.lib file in our project directory.
Copy the library file from your SoLoud installation over to your project directory (usually
the same directory as your main.cpp file).

And don't forget to add the SDL libraries listed above as well (my bad, thanks tgd from the discord)

You should now be setup for using SoLoud in your PGE project.

Try to build and run your project, you should not get any errors and it should run at this point
(although it won't do anything yet...)

Adding the Audio Listener and Audio Sources to your project
-----------------------------------------------------------

So now we have a project that can link to SoLoud and compile.  Awesome.  Next we need to actually
start using the Audio Listener and Sources (the whole point of going to this effort!)...

Add the following files to your project:

    olcPGEX_AudioListener.h
    olcPGEX_AudioSource.h
 
Below your inclusion of the PGE header in your main.cpp add the following defines and includes:

    #define AUDIO_LISTENER_IMPLEMENTATION
    #include "olcPGEX_AudioListener.h"
    
    #define AUDIO_SOURCE_IMPLEMENTATION
    #include "olcPGEX_AudioSource.h"

Great, now we can start using the AudioPGEXs.

In your declarations section add an instance of a listener and a source:

    olcPGEX_AudioListener AL;
    olcPGEX_AudioSource AS_Test;

In your OnUserCreate function add the following initialisations:

    AL.AudioSystemInit();
    AS_Test.AL = &AL;
    AS_Test.LoadAudioSample(0, "./assets/test.wav");

(This assumes you have an assets folder with a wav file named "test.wav")
(Also note, web based app paths are CASE SENSITIVE - test.wav != Test.wav)

In the OnUserUpdate function add the following code:

    // Input
    if (GetKey(olc::Key::P).bPressed)
    AS_Test.Play();

Now compile and run... when you press the P key, your sound should play!

Since we have the vs2019 desktop version working we can now move on to 
getting our project setup to work with Emscripten in the browser also...


Configuring Visual Studio for use with Emscripten and SoLoud
------------------------------------------------------------

In order to be able to compile with Emscripten and SoLoud in our project we will need to make
a few changes.

First of all, any additional include options we set previously will no longer be work.  So we
will need to copy the headers inside the SoLoud \include folder into our project folder.

We also need the soloud_static.lib file which should already be in the project folder (just
mentioning it in case it isn't)

(Note - the SDL library files are not needed to compile for emscripten, but if you intend to
test in a desktop environment you will still need them)

Great, that's the first part done :-)

Now we need to compile a ".o" file which contains the object symbols for the SoLoud header
files which we can use at compile time with emscripten, instead of manually compiling all
of the SoLoud headers every time we compile our program...

MaGetzUb from the discord was gracious enough to provide a batch file which will do exactly
this for us (Many Thanks!).

Run the emcompile.bat file from the root folder in your SoLoud directory and it should
hopefully produce a soloud.o file as a result.  The batch file is located in this repository.

(NOTE: for linux or macOS builds use emcompile.sh provided by Moros1138 instead)

Now copy the soloud.o file into your project directory.

Next step is to create a convenience text document in your project folder called
"em_cmd_line.txt".  We will configure our compiler flags here to make it easier to compile
later on.

Add the following text to the file:

    em++ -std=c++17 -O2 -s ALLOW_MEMORY_GROWTH=1 -s MAX_WEBGL_VERSION=2 -s MIN_WEBGL_VERSION=2 -s USE_LIBPNG=1 -s USE_SDL_MIXER=2 -s STACK_SIZE=5MB
    
Next we need to add the files we wish to compile... assuming your main project file is called "main.cpp" we can use the following
additional text:

    main.cpp soloud.o -o pge.html --preload-file ./assets

The entire file should now contain the following:

    em++ -std=c++17 -O2 -s ALLOW_MEMORY_GROWTH=1 -s MAX_WEBGL_VERSION=2 -s MIN_WEBGL_VERSION=2 -s USE_LIBPNG=1 -s USE_SDL_MIXER=2 -s STACK_SIZE=5MB main.cpp soloud.o -o pge.html --preload-file ./assets
    
This will be our compile command to use with emscripten.  Save the text document and continue.

(For linux and MacOS - please refer to the compile instructions in the readme_linux / readme_macOS files in
this repo instead)

Compiling with Emscripten for Web
---------------------------------

Go to your installion of Emscripten and run "emcmdprompt.bat". This will start a command prompt with the Emscripten environment
variables set.

Now navigate to your project folder in the command prompt window. Open the em_cmd_line.txt file created in the previous step and copy
the text, then paste the command into the emcommand prompt and run. If all instructions were followed correctly you should hopefully 
end up with a compiled version of your project that will work with Emscripten.

You should see our output file in your project folder "pge.html" (along with some other files as well). Go back to your
emcommandprompt and run your newly compiled project:

    emrun pge.html
    
Your project should now be running inside your browser - happy days!

That's it... If you got this far, congratulations :-)

Note: Each time you make changes to your project in visual studio and want to run in the browser you will need to follow the steps
in this section again from beginning to end.

Enjoy...
