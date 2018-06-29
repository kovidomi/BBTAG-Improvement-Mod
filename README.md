# BBTAG-Improvement-Mod
Additional features and tweaks for BlazBlue Cross Tag Battle

Join the [BB Improvement Mod Discord Server](http://discord.gg/29kExTj) to discuss about the development, and the game itself.

## Legal
```
BBTAG Improvement Mod is NOT associated with Arc System Works or any of its partners / affiliates.
BBTAG Improvement Mod is NOT intended for malicious use.
BBTAG Improvement Mod is NOT intended to give players unfair advantages in online matches.
BBTAG Improvement Mod is NOT intended to unlock unreleased / unpurchased contents of the game.
BBTAG Improvement Mod should only be used on the official version that you legally purchased and own.

Use BBTAG Improvement Mod at your own risk. 
I, KoviDomi, am not responsible for what happens while using BBTAG Improvement Mod. You take full reponsibility for any outcome that happens to you while using this application.
```

## Compiling
BBTAG Improvement Mod is coded using Visual Studio 2017 (v141).
To compile, you should only need to load the sln file and compile as-is.

Can only be compiled in the Release/Win32 config.

No changes should be needed to the solution or source.

## Requirements:
* Visual Studio 2017 (v141)
* Detours 1.5
* DirectX SDK
* Steamworks SDK (v1.42)

## Quick file overview

* "TODO.txt" contains a list of things you can start working on
* "main.h" file contains the defines for logging
* "containers.h" file contains global variables that are used across the entire project
* "D3D9ExWrapper" folders contain straightforward D3D wrapping
* "Hooks/hooks_bbtag" implements game functions overriding
* "Hooks/hooks_importAddressTable" implements imported functions overriding using the Detours library
* "Hooks/hooks_steamApiWrapper" implements Steam API overriding
* "SteamApiWrapper" folder contains straightforward Steam API wrapping
* "WindowManager" folder contains the implementation of the mod's custom windows using the ImGui library

## Support
Your contributions are speeding up the development. Please consider donating to enable me spending more time working on the mod.

[![Donate](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=C8EDR78DJ8PU2)

[![paypal](https://www.paypalobjects.com/en_US/i/btn/btn_donateCC_LG.gif)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=C8EDR78DJ8PU2)
									   
## Thanks to:
Atom0s for their DirectX9.0 Hooking tut<br>
Durante for their dsfix source
