# EzAL (This is just the library part, which is why this repo is called JustEzAL)

### A powerful audio library designed to abstract away writing audio code at 0 cost. Spend time coding your dream project, not the audio.

The default audio library for this is openal-soft.

To choose a different audio library change 
target_compile_definitions(EzAL PUBLIC OPENAL)
on EzAL/EzAL/CMakeLists.txt line 8 to the one you want. (Sadly, EzAL only supports openal-soft, but I'm glad to accept additions to EzAL for new audio libraries, among other things.

The current available audio libraries:<br/>
openal-soft

EzAL currently only works with .ogg files, but I'd be glad to accept support for different audio file types into EzAL.

EzAL's structure is based on OpenAL.

Thanks to Code, Tech, and Tutorials for some of the code!<br/>
You can find their channel here: https://www.youtube.com/c/CodeTechandTutorials<br/>
the video for it here: https://www.youtube.com/watch?v=kWQM1iQ1W0E<br/>
their github here: https://github.com/codetechandtutorials<br/>
and their code here: https://github.com/codetechandtutorials/openal-impl/releases/tag/vid1
