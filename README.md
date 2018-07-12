# BBTAG-Improvement-Mod
Additional features and tweaks for BlazBlue Cross Tag Battle

The goal is to extend the game with community-created content, provide additional graphical options, and improve performance for players with computer specs below the minimal system requirements.

Join the [BB Improvement Mod Discord Server](http://discord.gg/29kExTj) to discuss about the development and the game itself!

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

## Compiling and usage
BBTAG Improvement Mod is coded using Visual Studio 2017 (v141). <br>
To compile, you should only need to load the sln file and compile as-is. No changes should be needed to the solution or source.<br>
Can only be compiled in the Release/Win32 config.<br>
Copy the compiled binary and the settings.ini file from the bin/ folder into the game's root folder.

## Requirements:
- Visual Studio 2017 (v141) (Windows SDK 8.1)
- Detours 1.5
- DirectX SDK
- Steamworks SDK (v1.42)

## Quick file overview
- **TODO.txt** <br>
list of things you can start working on
- **main.h** <br>
defines for logging
- **interfaces.h** <br>
access to all the wrappers and hooked game variables
- **D3D9ExWrapper/** <br>
straightforward D3D wrapping
- **Game/** <br>
player, ingame characters, meters information
- **PaletteManager/** <br>
custom palette loading related class
- **SteamApiWrapper/** <br>
straightforward Steam API wrapping
- **WindowManager/** <br>
implementation of the mod's windows using the ImGui library
- **Hooks/hooks_bbtag** <br>
implements game functions overriding
- **Hooks/hooks_detours** <br>
implements imported functions overriding using the Detours library
- **Hooks/hooks_steamApiWrapper** <br>
implements Steam API overriding


## Support
If you feel like throwing some dollars my way feel free to, it enables me spending more time working on the mod.

[![Donate](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=C8EDR78DJ8PU2)

[![paypal](https://www.paypalobjects.com/en_US/i/btn/btn_donateCC_LG.gif)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=C8EDR78DJ8PU2)

## Thanks to the people who have helped the mod along the way:
* Neptune
* Rouzel
* Dormin
* NeoStrayCat
* Everybody in the BlazBlue PC community that has given support or feedback of any kind!

## Extra thanks to:
Atom0s for their DirectX9.0 Hooking article<br>
Durante for their dsfix source
