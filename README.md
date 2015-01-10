
#Pidgin Notifier Plugin#

IRC NOTICE notification plugin.  When a user sends a notice to a channel this plugin will play a sound, highlight the text, and log the notice in a sperate tab.

This plugin also will only notify when it's a NOTICE from the server and not just someone type (notice) in their message.


##Installation##

Use the Notifier_Installer.exe or copy notifier.dll into your pigdin plugins dir(default is %APPDATA%/.purple/plugins)

Pre-built binaries can be found here: [Releases](https://github.com/MeTwentyFive/Pidgin_Notifier_Plugin/releases)

**WARNING**

You'll need to shutdown pidgin if you are upgrading this plugin.


##Build##

1.) Setup the pidgin build environment

2.) Edit .pidgin_dev.bashrc

3.) Source .pidgin_dev.bashrc

4.) make -f Makefile.mingw

5.) make -f Makefile.mingw install

