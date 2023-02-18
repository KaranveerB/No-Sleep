# No Sleep (for Windows Laptops)
A simple way to disable sleep (or whatever lid close behaviour you have) when closing your laptop lid.

Useful for certain things where you want to close your laptop lid without your laptop stopping whatever its doing.
For example, a large download, using an external non-blind drawing tablet, using external monitors, hosting an RDP session.

This program only supports Windows (specfically tested on 10). If you're on Linux and have systemd, you can use `systemd-inhibit --what=handle-lid-switch sleep 1d` (or some other timespan),
keep it running in the background, and kill it with ctrl+c when you're done as an alternative to this program.

## Installation
Download the latest release, unzip, and move the folder to the desired location. Make sure to keep the contents of the folder together.

Optionally, you can create a shortcut to the app and add it to the Windows start menu at `C:\Users\USERNAME\AppData\Roaming\Microsoft\Windows\Start Menu\Programs`.

## Usage
To use the app, double click `No Sleep.exe` and there should be a tray icon (click the `^` on the taskbar on Windows) and the app will automatically disable sleep on lid close.

To disable it and restore sleep, simply click the tray icon to toggle it, or right click and press disable. Similarly for re-enabling it.

To quit the program and restore normal sleep behaviour, simply right click the tray icon and press quit to stop it early,
or (if you're done using your computer) simply shutdown the entire computer normally and normal behaviour should be restored.

### What to do if you have issues

If there is ever any issues (for example as a result of opening multiple instances (TODO: FIXME by preventing multiple instances), force-killing the app, shutdown from power lose or holding the power button),
quit and re-open the program such that there is only a single instance running, and press the "Force enable sleep" button to force re-enable sleep on lid close.

Note that force enable sleep only force enables sleep on lid close. If you had different behaviour (e.g., hibernate or shutdown on lid close, or having different settings for battery and plugged in),
you will need to manually change that in Window's Edit power plan.
