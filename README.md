RPGMaker
========
[Description]
RPGMaker is a small program that enables you to create small and simple 2D role playing games.
It is inspired by Enterbrain's commercial product RPG Maker XP.
Many of the resources found in RPG Maker XP can be loaded and used by this program.


[Project notes]
The project is developed in C++ and Qt on Microsoft's Windows 7 Professional (x64) and Visual Studio 2010 Professional.
The code is given to you as is, and I do not intend to work on it further.
The code is written in a modular way and most parts should be straightforward.
Although most parts of the code are undocumented, the title of classes/methods gives a good idea of what it is intended to do.

The reason for placing it on GitHub is that the code might be of use to someone, that someone might pick it up and improve upon it.
The code certainly isn't perfect and could use much improvement.

[Known issues]
- Although the necessary Qt libraries are shipped with the installer (if you build the installer), not all functions will work as intended.
  For unknown reasons, the libraries that are unpacked by the installer are not enough (or is somehow bugged) and will cause some images to not be loaded correctly and music will not play at all.
  The solution for this is to install the Qt libraries that you used to compile the project on the target machine (i.e. if you used Qt v4.8.2 to build RPGMaker, you must install Qt v4.8.2 on the target machine as well in order to let the program function correctly).