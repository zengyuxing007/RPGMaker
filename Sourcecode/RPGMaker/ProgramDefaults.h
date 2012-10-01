#ifndef LOCATIONPATHS_H
#define LOCATIONPATHS_H

#include "qsize.h"
#include "qstring.h"
#include "qregexp.h"

/**
*	This file contains several global constants.
*	Many constants refer to a specific folder where resources can be found.
*/

// Images
const QString g_iconPath("resources/config/icons/");
const QString g_schemaPath("resources/config/schemas/");
const QString g_charactersPath("resources/config/images/characters/");
const QString g_battlersPath("resources/config/images/battlers/");
const QString g_staticTilesetPath("resources/config/images/tilesets/");
const QString g_autoTilesetPath("resources/config/images/autotiles/");
const QString g_battlebackPath("resources/config/images/battlebacks/");
const QString g_windowskinPath("resources/config/images/windowskins/");

// Sound
const QString g_backgroundMusicPath("resources/config/sound/background/");
const QString g_soundEffectsPath("resources/config/sound/effects/");
const QSize g_playResolution(1024, 768);

// Program values
const int g_layers = 4;
const int g_tilesize = 32;
const int g_buttonsize = 24;
const int g_maxPartysize = 5;
const int g_energyMultiplier = 3;
const int g_healthMultiplier = 5;
const int g_maxBattlegroupsize = 3;
const int g_maxInventoryItemCount = 99;
const double g_characterProgressionRate = 0.3;
const QRegExp g_nameRegex("^([a-zA-Z0-9 ']){1,25}$");

#endif