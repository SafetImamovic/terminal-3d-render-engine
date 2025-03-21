#pragma once

#include "../../include/constants.h"
#include "../../include/globals.h"

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#endif

void initConsole();

void hide_cursor();

void show_cursor();

void renderBuffer();
