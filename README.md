
#Pidgin Notifier Plugin#

IRC NOTICE notification plugin.  When a user sends a notice to a channel this plugin will play a sound, highlight the text, and log the notice in a sperate tab.
This plugin also will only notify when it's a NOTICE from the server and not just someone type (notice) in their message.

##Build##

1.) Setup the pidgin build environment using cygwin
2.) Edit .piding_dev.bashrc
3.) make -f Makefile.mingw


##Installation##

Copy notifier.dll into your pigdin plugins dir(default is %APPDATA%/.purple/plugins


