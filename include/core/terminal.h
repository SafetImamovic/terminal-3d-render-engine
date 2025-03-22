/*===============================================================================================*
 *
 * File: terminal.h
 *
 * Description: Handles terminal-related operations for rendering and cursor control.
 *              Supports both Windows and Unix-like systems (Or atleast I tried my best =p).
 *
 *===============================================================================================*/

#ifndef TERMINAL_H
#define TERMINAL_H

#include "../../include/constants.h"
#include "../../include/globals.h"

#ifdef _WIN32
#include <conio.h>
#else
#include <sys/ioctl.h>
#include <signal.h>
#include <fcntl.h>
#include <stdio.h>
#include <time.h>
#endif

/**
 * Function: init_console
 *
 * Initializes the console for rendering. On Windows, enables virtual terminal
 * processing for ANSI escape code support.
 */
void init_console();

/**
 * Function: hide_cursor
 *
 * Hides the terminal cursor to prevent flickering during rendering.
 */
void hide_cursor();

/**
 * Function: show_cursor
 *
 * Restores visibility of the terminal cursor.
 */
void show_cursor();

/**
 * Function: render_buffer
 *
 * Renders the screen buffer to the console. On Windows, uses WriteConsoleOutputW
 * for direct buffer manipulation. On Unix, prints the buffer using ANSI sequences.
 */
void render_buffer();

/**
 * Function: clear_buffer
 *
 * Clears the screen buffer by filling it with spaces.
 */
void clear_buffer();

#endif
