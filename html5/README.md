    MMMMMMMMMMMM77$7777$ZZZZ777MMMMMMMMMMMMM
    MMMMMMMMMZZZ7777777777ZZZ77777MMMMMMMMMM
    MMMMMMM$77$ZZ77777777ZZZZZZZ7777MMMMMMMM
    MMMMMZ777ZZZZZZZZ77.ZZZZZZZZZZ7777MMMMMM
    MMMMZZZZZZZZZZZZZ,. ..ZZZZZZZZZZZZZMMMMM
    MMN7ZZ777ZZZZZZZ...   .ZZZZZZZ777ZZZDMMM
    MM777777777ZZZ..       =ZZZZZ777777ZZMMM
    M7$Z77777777Z.          .7Z77777777ZZZMM
    ZZZZ7777777..Z7.        . .$77777777ZZZM
    7ZZZZ7777..   ~7..I.    .   O.7777777Z7M
    7ZZZZZ77..      Z77... ........I77777$7M
    7ZZZZZ..       ZZ.      .      ..77ZZZ7$
    7ZZZZI.        ..  .  .Z.       .:ZZZZ77
    7ZZ7777.        .    IZ..      .7ZZZZZ7D
    7$7777777...        7.~O     .I777ZZZZ7M
    ZZZ77777777Z     .     .7Z. .777777ZZZ7M
    MZZI777777777+          ..$77777777ZZ7MM
    MDZZ7777777ZZZ7...      ..ZZ7777777I7ZMM
    MMOZZZ777$ZZZZZ...    . ZZZZZZ7777Z7$MMM
    MMMOZZZ7ZZZZZZZZ$  .. 7ZZZZZZZZ7ZZZZMMMM
    MMMMMZI7IZZZZZZZZZ,..$ZZZZZZZZZ77ZMMMMMM
    MMMMMM77777ZZZZZZZZ777777ZZZZ777ZMMMMMMM
    MMMMMMMM877777ZZZI777777777ZZZ$MMMMMMMMM
    MMMMMMMMMMM87777ZZZ777777$7ZMMMMMMMMMMMM
    MMMMMMMMMMMMMMMMDZZZZZ8MMMMMMMMMMMMMMMMM

                          __   _
      ___ ________ ____  / /  (_)______
     / _ `/ __/ _ `/ _ \/ _ \/ / __(_-<
     \_, /_/  \_,_/ .__/_//_/_/\__/___/
    /___/        /_/
       __            __
      / /  ___ _____/ /__ ___ ___  ___ ________
     / _ \/ _ `/ __/  '_/(_-</ _ \/ _ `/ __/ -_)
    /_//_/\_,_/\__/_/\_\/___/ .__/\_,_/\__/\__/
                           /_/


# An Introduction to WebGL via the Medium of RayMarching #

This short sequence of webpages should provide you with a basis for procedural graphics using modern hardware and software. It starts with WebGL recognition in a browser and finishes with a raymarching shader.

There are 8 segments, each with a webpage that deals with a topic. Each page has some javascript inside it and some scripts that are loaded externally.


## Requirements ##

  * A good web-browser - Basically, the latest Chrome or Firefox
  * A machine capable of running WebGL - **Check with http://get.webgl.org**
  * A good text editor such as Vim, Emacs, Sublime, Textmate, Notepad++ etc
  * A personal webserver such as python's simple http server
  * Some possible extra tools like Chrome WebGL Plugin, Firebug or similar
  * A lust for graphics fun!

The 8 parts are as follows:

  * Part1 - Detecting WebGL in your browser
  * Part2 - Creating a context
  * Part3 - Drawing your first triangle
  * Part4 - A Screen Aligned Quad
  * Part5 - Loading our Shaders from external files.
  * Part6 - Animation
  * Part7 - Basic Sphere Raymarching
  * Part8 - Prettier Sphere Raymarching

This assumes working knowledge of javascript and a little knowledge of OpenGLES and GLSL though the latter two are not necessary at the beginning (you'll know a fair bit by the end!). 

The focus is getting to the point where we get our shaders to do most of the work on the graphics side, drawing things using the Raymarching algorithm. This is the precursor to the raymarched distance field which is (was?) the hot hot topic on the demoscene - see http://www.pouet.net/topic.php?which=8177&page=1

It's not the prettiest of outcomes but the theory can be expanded and played with to do a lot more interesting things. Its a naive approach but its a start.


## Thanks ##

  * The London Hackspace - http://london.hackspace.org.uk
  * Katie Eagleton
  * Inigo Quilezles - http://iquilezles.org
  * Mozilla Foundation
  * Antonio García Castañeda
  * Chrome Development Team
  * The people who come to the Graphics Hackspace


## Resources ##

  * http://www.pouet.net/topic.php?which=8177&page=1
  * http://www.khronos.org/opengles/sdk/docs/man/xhtml/glUniform.xml
  * https://developer.mozilla.org/en-US/docs/DOM/window.requestAnimationFrame
  * http://benvanik.github.com/WebGL-Inspector/
  * http://glsl.heroku.com/
  * http://www.postronic.org/h3/
  * http://iquilezles.org/prods/
  * http://www.ibiblio.org/e-notes/webgl/gpu/contents.htm
  * http://blog.jaxgl.com/
  * http://js1k.com/
  * http://acko.net/
  * http://www.everyday3d.com/
  * https://github.com/toji
  * http://demotrip.blogspot.gr/
  * http://code.google.com/p/webgl-loader/
  * http://www.glge.org/
  * http://code.google.com/p/o3d/
  * http://spidergl.org/
  * http://www.chromeexperiments.com/
  * http://sylvester.jcoglan.com/
  * http://www.khronos.org/webgl
  * http://learningwebgl.com/blog/?page_id=1217
  * http://html5boilerplate.com/


## Books ##
 
The Orange Book
The Red Book

... will add more!

## Contacts ##

Ben Blundell - ben@graphicshackspace.org | ben@section9.co.uk