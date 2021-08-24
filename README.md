# PGEv2_Extensions
olcPGEX extenstions that I find useful and I think might be useful to others

You can see a short demo of them all in use here: https://youtu.be/GbyLmkikm30
(Driver Unhinged Alpha Demo Footage) ~25 seconds into the video

Also a specific demo just for the animator here: https://youtu.be/zr-wD1DFrog
(olcPGEX_Animator2D Offical Demo)

olcPGEX_SplashScreen.h
----------------------

Self explanatory really... Easily display a smoothly animated olc::PixelGameEngine
logo at the beginning of ANY pixel game engine project with a measly 3 lines of code

Check the comments at the top of the header file for a simple demo implementation...

A short visual demonstration is available here:
https://youtu.be/h1rgw5Nom3c

olcPGEX_ResourceManager.h
-------------------------

Using lots of decals in your project?  Need to reference them outside your main class?
If so, you can use this very simple (single function) extension to keep them organised
and easily accessible, whilst being sure that you never load the same image into memory
more than once...

Check the comments at the top of the header file for more detailed info...


olcPGEX_Camera2D.h
------------------

Quickly add smooth camera scrolling to your 2D games with this simple extension.  Aside
from smooth scrolling, it also offers smooth boundary clamping as well.

Check the comments at the top of the header file for some example code and more detailed
info...


olcPGEX_RayCast2D.h
-------------------

Need raycasting for your 2D game?  I did, so I made this extension for others to use as
well.

All you need to use this ray caster are the beginning and end points along 2 lines
that you wish to check for intersection.  It returns a boolean collision value and also
the pixel location on screen of where the collision occured as well as the distance along
the ray so you can calculate which collision is closest.

Check the comments at the top of the header file for some example code and more detailed
info...


olcPGEX_Animator2D.h
--------------------

Animation can be the most tedious and difficult part of creating games, and as a result
can deter a lot of people early on in project development.  This extension allows for
fast and simple animation with very little setup or upkeep.

I always try to find a balance between rich features and overal simplicity - I don't like
a lot of the complex repetitive code to exist in my main program.

The extension allows you to play lots and lots of animations independently of each other
in an easy to manage way and offers support for varying speeds, static animation, billboard
animation, rotation, offsets, scaling, tinting, mirroring, reverse playback, ping-pong, and
more... Sounds complicated?  No, the usage has been simplified so the programmer need not worry
about the complexities, just feed in some initial values and you're good to go :-)

Simplified it works like this;

[SomeObject] - [AnimatorController] - [Animation][Animation][Animation][...]

Simply add an olcPGEX_Animator2D instance to any object in your game (players, enemies,
weapons, pickups, etc... or even the main class for HUDs etc).

This is your animator controller.

Then add animations to it using any of the 3 types currently available. You just need a
spritesheet, number of frames, size of each frame and a duration (plus some extra options
for more fancy animations if you require it).

Setup done :-)

Now, you can simply 'control' the animations by telling them when to Play, Stop, Update, and Draw.

It's really that simple.

For some example code and more detailed info on what types of animations are supported check
the comments section of the header file...


olcPGEX_ScrollingTile.h
-----------------------

If you need a tiled backdrop that seems to go forever and ever, then this extension does just
that.  Draws a tileset that fills the screen and culls what is not visible.  As you move your
camera around, the tiles will adjust their positions accordingly.


olcPGEX_Interactable.h
----------------------

GUIs are famously... well, a pain in the bottom :-(

Every now and then you just need some simple buttons or menus to add to your mini-game and
creating an entire framework each time becomes tedious and annoying... That is where this
PGEX comes to the rescue!

Need a button?  Easy...
Need many buttons?  Also easy...
Need those many buttons organised into menus of buttons?  You can do that to :-)

Example code is in the comments section of the header.
