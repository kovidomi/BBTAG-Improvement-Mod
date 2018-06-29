BBTAG Improvement Mod (v1.04) by KoviDomi -- README
==========================

What this mod provides
==========================
Options to improve the performance of the game.
More flexibility to change the graphics options.

Requirements
==============
The game must be up-to-date

How to install
==========================
1. Download and extract the latest BBTAG_IM.zip
2. Put dinput8.dll and settings.ini into the game's folder (where BBTAG.exe is located)
(Should be at "..\Steam\SteamApps\common\BBTAG")
3. Adjust the values in settings.ini as desired

Uninstall
==========================
Delete or rename dinput8.dll. The mod makes no permanent changes to anything.

Known issues
==========================
- The custom HUD is not updated while the game's window is not focused.
- The custom HUD does not show player names in online matches.
- The regionlock is still not completely unlocked

Troubleshooting
==========================
First make sure that your issue doesn't persist if you remove/rename dinput8.dll
If the issue still persists, then it has nothing to do with the mod.

1. The mod's UI is very small, unreadable:
This happens if you run the game in windowed or full-window mode with a resolution that's higher than what your monitor supports.
Try changing your display setting to fullscreen, or lowering the resolution ingame.

2. Blackscreen problem:
Make sure that RenderingWidth and RenderingHeight values in the .ini file are correctly set to the resolution of your game. 
If their values are higher than your monitor's native resolution, then you will get blackscreen.

3. Game crashes when launched:
Make sure that the settings.ini file is present in the game's folder.
Check if the problem still occurs after you remove, or rename dinput8.dll
Turn off tools such as MSI Afterburner, Plays.tv, or other recording softwares and overlays that manipulate D3D.
Make sure you are running both Steam and the game with administrator privileges, and remove any steam_appid.txt file if present in the game's directory.
Restore the default settings in the settings.ini file.
Reboot your system.

4. The custom HUD is all over the place, not positioned properly:
The elements of the custom HUD needs to be positioned MANUALLY by the user with the mouse cursor (press F1 by default). The positions are saved upon exiting.
The elements are movable only when the BBTAG_IM mod's main window and the ingame cursor is visible (press F1 by default).
Recommend picking Aegis, Rachel, Yukiko, and Makoto in the tutorial mode for adjusting the custom HUD elements.

5. Some of the custom HUD elements have gone beyond the screen and became unrecoverable:
Press the "Reset custom HUD positions" button on the BBTAG_IM mod's main window to bring all the elements back onto the middle of the screen.

Keep in mind that the mod may partially, or completely stop working whenever a new official patch of the game is released. (Though unlikely)

Changelog
==========================

??-??-2018 -- version 1.04
===============================
- Added option to keep blinking the button prompt layout 

21-06-2018 -- version 1.03
===============================
- Added character specific meters to the custom HUD for Aegis, Chie, Makoto, Naoto, Platinum, Rachel, and Yukiko
- Added CustomHUDScale option
- Added PromptButtonLayout option that prevents the game from blinking the button prompts between PS4 and PC layouts
- Added a tooltip to the "Reset custom HUD positions" button to avoid confusion about its purpose
- The game's window is now always positioned to the middle of the screen when DisplayMode is set to Windowed(1) in the settings.ini
- The mod's main window now automatically disappears within 10 seconds after the title screen
- Fixed custom HUD HP bars not switching immediately after characters die
- Fixed alt tabbing in fullscreen causing all custom HUD elements being thrown into the top left corner

16-06-2018 -- version 1.02
===============================
- Added Custom HUD
- Added ToggleCustomHUDButton option
- Added ForceCustomHUD option
- Added DisplayMode option
- Added FullscreenHz option
- Added "Reset custom HUD positions" button
- Removed palette folders being created in BBTAG_IM folder

07-06-2018 -- version 1.01
===============================
- Added togglable HUD option (temporary solution for performance problems during matches)
- Fixed gamewindow getting keyboard inputs while not in focus

06-06-2018 -- version 1.00
===============================
- Initial release


Special thanks to
==========================
Neptune
Rouzel
Dormin
NeoStrayCat
Everybody in the BlazBlue PC community that has given support or feedback of any kind!

If you like my work then consider feeding me by donating few bucks (helps me stay alive):
https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=C8EDR78DJ8PU2
