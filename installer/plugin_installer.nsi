
SetCompressor /SOLID lzma
ShowInstDetails show


Outfile "../bin/Notifier_Installer.exe"
 

# define the directory to install to, the desktop in this case as specified  
# by the predefined $DESKTOP variable
InstallDir "$APPDATA\.purple\plugins"

# default section
Section
 
# define the output path for this file
SetOutPath $INSTDIR

SetOverwrite On

# define what to install and place it in the output path
File ..\bin\notifier.dll

SectionEnd
