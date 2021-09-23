# asskii-player

Ascii Video Player that runs in any modern Console. [W.I.P]

## Usage :-
1. Build the asskii-player with a <s>C++20</s> C++11 Compatible Compiler.
2. FFmpeg should be in Path.
3. Run asskii-player from commandline.

## Build Commands :-
1. `git clone https://github.com/Tanishq-Banyal/asskii-player.git`
2. `cd asskii-player`
3. `g++ -O3 -s -march=native -mtune=native -flto -static --std=c++11 -Wall main.cpp -o asskii-player`

## CLI Help :-
```
~$ ./asskii-player

Usage :-
./asski-player [video path] <optional arguments>

-w & -h   -->  Set Frame Width & Height (defaults are -w 80 -h 45)
--color   -->  Display the Video in 24-bit Colors (Slow on some Terminals)
--block   -->  Specify the character to use as pixel in Color Mode (default is #)
               (ignored in non-color mode)
--shades  -->  Specify the number of ascii shades (default is 32)
               (ignored in color mode)

Note : Unknown Arguments are silently ignored.
```

## Working Idea :-
(✔️=Done, 🕐=Pending)
1. Extract video frames using ffmpeg. ✔️
2. Pipe the frames into the program. ✔️
3. Display the frames as ascii art on console ✔️ with optional --color. ✔️
4. Audio Support with SDL2_Mixer maybe ? 🕐
5. Playback control using keyboard. 🕐
6. Include a rickroll for testing. 🕐 (it was the original aim for this app)
