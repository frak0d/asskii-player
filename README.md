---
# asskii-player
Ascii Video Player with **True Color** support that runs in any modern Console.

---
## 📄 Notes :-
1. FFmpeg should be in Path.
2. If you are getting low fps in color mode then try using [kitty](https://github.com/kovidgoyal/kitty) terminal, it's blazing fast!

---
## ⚡ Installation :-
```sh
git clone https://github.com/Tanishq-Banyal/asskii-player
cd asskii-player && cmake . && make -j 4
sudo cp asskii-player /usr/bin/asskii-player
cd .. && rm -r asskii-player
```

---
## 🗑️ Uninstallation :-
```sh
sudo rm /usr/bin/asskii-player
```

---
## 💡 Usage :-
```
./asski-player [video path] <optional arguments>

-w & -h   -->  Set Frame Width & Height (defaults are -w 80 -h 45)
--color   -->  Display the Video in 24-bit Colors (Slow on some Terminals)
--block   -->  Specify the character to use as pixel in Color Mode (default is #)
               (ignored in non-color mode)
--shades  -->  Specify the number of ascii shades (default is 32)
               (ignored in color mode)
--nobg    -->  Does not print the background black.
--noclr   -->  Preserve the last frame after playing.

Note : Unknown Arguments are silently ignored.
```

---
## 📃 Roadmap :-
(✔️=Done, 🕐=Pending)
1. Extract video frames using ffmpeg. ✔️
2. Pipe the frames into the program. ✔️
3. Display the frames as ascii art on console ✔️ with optional --color. ✔️
4. Audio Support with SDL2_Mixer maybe ? 🕐
5. Playback control using keyboard. 🕐
6. Unicode Mode using **Block Characters** for higher resolution. 🕐

---
### ⭐ Stargazers :-
[![Stargazers for @Tanishq-Banyal/asskii-player](https://reporoster.com/stars/Tanishq-Banyal/asskii-player)](https://github.com/Tanishq-Banyal/asskii-player/stargazers)

### 🍴 Forkers :-
[![Forkers for @Tanishq-Banyal/asskii-player](https://reporoster.com/forks/Tanishq-Banyal/asskii-player)](https://github.com/Tanishq-Banyal/asskii-player/network/members)

---
