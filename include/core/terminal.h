#pragma once

#include "../../include/constants.h"
#include "../../include/globals.h"

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#endif

void init_console();

void hide_cursor();

void show_cursor();

void render_buffer();

void clear_buffer();

void draw_edges(wchar_t vertical, wchar_t horizontal, wchar_t corner);

void draw_coordinate_system();
