# albert-plugin-mediaremote

## Features

- Control playback with 'Play', 'Pause', 'Next', 'Previous' items.
- Items match by localized and english titles.
- Items can also be matched by the player name.

## Platforms

- macOS
- UNIX

## Technical notes

### Public API

Exposes several functions related to players and playback.

### macOS 

Uses private API (MediaRemote.framework).

### UNIX

Uses MPRIS DBus interface.

