/*===============================================================================================*
 *
 * File: terminal.c
 *
 * Description: Handles terminal-related operations for rendering and cursor control.
 *              Supports both Windows and Unix-like systems (Or atleast I tried my best =p).
 *
 *===============================================================================================*/

#include "../../include/core/terminal.h"

#ifdef _WIN32

HANDLE     hConsole;
CHAR_INFO  consoleBuffer[SCREEN_HEIGHT * SCREEN_WIDTH];
COORD      bufferSize  = {SCREEN_WIDTH, SCREEN_HEIGHT};
COORD      bufferCoord = {0, 0};
SMALL_RECT writeRegion = {0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1};

#else

#include <stdio.h>

#endif

/**
 * Function: init_console
 *
 * Initializes the console for rendering. On Windows, enables virtual terminal
 * processing for ANSI escape code support.
 */
void init_console()
{
#ifdef _WIN32
        hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleMode(hConsole, ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif
}

/**
 * Function: hide_cursor
 *
 * Hides the terminal cursor to prevent flickering during rendering.
 */
void hide_cursor()
{
#ifdef _WIN32
        HANDLE              hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO cursorInfo;
        GetConsoleCursorInfo(hConsole, &cursorInfo);
        cursorInfo.bVisible = FALSE; // Hide cursor
        SetConsoleCursorInfo(hConsole, &cursorInfo);
#else
        printf("\033[?25l");
#endif
}

/**
 * Function: show_cursor
 *
 * Restores visibility of the terminal cursor.
 */
void show_cursor()
{
#ifdef _WIN32
        HANDLE              hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO cursorInfo;
        GetConsoleCursorInfo(hConsole, &cursorInfo);
        cursorInfo.bVisible = TRUE; // Show cursor
        SetConsoleCursorInfo(hConsole, &cursorInfo);
#else
        printf("\033[?25h");
#endif
}

/**
 * Function: render_buffer
 *
 * Renders the screen buffer to the console. On Windows, uses WriteConsoleOutputW
 * for direct buffer manipulation. On Unix, prints the buffer using ANSI sequences.
 */
void render_buffer()
{
#ifdef _WIN32
        CHAR_INFO *ptr = consoleBuffer;
        for (int y = 0; y < SCREEN_HEIGHT; y++)
        {
                for (int x = 0; x < SCREEN_WIDTH; x++)
                {
                        ptr->Char.UnicodeChar = buffer[y][x];
                        ptr->Attributes       = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
                        ptr++;
                }
        }
        WriteConsoleOutputW(hConsole, consoleBuffer, bufferSize, bufferCoord, &writeRegion);
#else
        printf("\033[H"); // Reset cursor position
        for (int y = 0; y < SCREEN_HEIGHT; y++)
        {
                for (int x = 0; x < SCREEN_WIDTH; x++)
                {
                        putwchar(buffer[y][x]);
                }
                putwchar(L'\n'); // Move to next line
        }
        // fflush(stdout); <- Might introduce latency, should profile it later.
#endif
}

/**
 * Function: clear_buffer
 *
 * Clears the screen buffer by filling it with spaces.
 */
void clear_buffer()
{
        for (int i = 0; i < SCREEN_HEIGHT; i++)
        {
                wmemset(buffer[i], L' ', SCREEN_WIDTH);
        }
}
