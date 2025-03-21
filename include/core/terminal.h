#pragma once

#include "../../include/constants.h"
#include "../../include/globals.h"

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#else
#include <sys/ioctl.h>
#include <signal.h>
#include <fcntl.h>
#include <stdio.h>
#include <time.h>
#endif

void init_console();

void hide_cursor();

void show_cursor();

void render_buffer();

void clear_buffer();
