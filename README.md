# sonmat-crilayla

A part of 손맛 햅틱 / Sonmat Haptics (`sonmat`): crilayla.dll.

Unpacks CRILAYLA-compressed files (CRI archives) for the sound preparation (NieR's cutscene banks). The public algorithm (CriPakTools).

The app does not carry this in its installer for good: it fetches the release zip that `sonmat-games/parts.json` names
(address + SHA-256) into `<app>/parts/crilayla/` when a game needs it (`sonmat/tools/partfetch.py`). A change here reaches
players when a new tag has been released and `parts.json` points at it.

Build: Visual Studio C++ tools (x64): build.bat crilayla. Every push to main builds it on GitHub (artifact); a tag `v*` makes the release.
