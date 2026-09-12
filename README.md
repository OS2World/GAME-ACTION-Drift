# Drift for OS/2

A space shoot-em-up game for OS/2 Presentation Manager (Asteroids style).
Original version 1.02beta by Dirk Vandenheuvel (1995).

![Drift ScreenShot](/doc/Drift.png)

## Version

1.03 (OpenWatcom build, 2026)

## License

GNU GPLv3 or later

## Build

Requires Open Watcom C/C++ and the OS/2 Toolkit 4.5 on ArcaOS or OS/2 Warp.

```
compile-wat.cmd
```

This runs `wmake clean` then `wmake all`. Output goes to `bin\`.

## Changes in 1.03

- Compiled with Open Watcom (wcc386)
- Ctrl+X exits the game immediately
- Window size enforced to 1024x768
- BLDLEVEL string embedded in the executable
- Detail submenu moved inside Options
- Language submenu: English, Spanish, Dutch, German, French, Italian
- Source in `/src`, binaries in `/bin`

## Authors

- Dirk Vandenheuvel (original game, 1995)
- Released as Open Source (GPL) in 2002

## Links

- http://www.os2world.com/games/index.php/native-games/action-2/82-drift
- https://github.com/OS2World/GAME-ACTION-Drift
