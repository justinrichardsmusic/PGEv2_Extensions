# PGEv2_Extensions
olcPGEX extenstions that I find useful and I think might be useful to others
#


olcPGEX_ResourceManager.h
~~~~~~~~~~~~~~~~~~~~~~~~~

Using lots of decals in your project?  Need to reference them outside your main class?
If so, you can use this very simple (single function) extension to keep them organised
and easily accessible, whilst being sure that you never load the same image into memory
more than once...

Check the comments at the top of the header file for more detailed info...


olcPGEX_Camera2D.h
~~~~~~~~~~~~~~~~~~

Quickly add smooth camera scrolling to your 2D games with this simple extension.  Aside
from smooth scrolling, it also offers smooth boundary clamping as well.

Check the comments at the top of the head file for some example code and more detailed
info...


olcPGEX_Animator2D.h
~~~~~~~~~~~~~~~~~~~~

Animation can be the most tedious and difficult part of creating games, and as a result
can deter a lot of people early on in project development.  This extension allows for
fast and simple animation with very little setup or upkeep.

I always try to find a balance between rich features and overal simplicity - I don't like
a lot of the complex repetitive code to exist in my main program.

The extension allows you to play lots and lots of animations independently of each other
in an easy to manage way and offers support for varying speeds, static animation, billboard
animation, rotation, offsets, and more... Sounds complicated?  No, the usage has been simplified
so the programmer need not worry about the complexities, just feed in some initial values
and you're good to go :-)

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
