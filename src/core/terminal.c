#include "../../include/core/terminal.h"

#include <stdio.h>

#ifdef _WIN32

HANDLE     hConsole;
CHAR_INFO  consoleBuffer[SCREEN_HEIGHT * SCREEN_WIDTH];
COORD      bufferSize  = {SCREEN_WIDTH, SCREEN_HEIGHT};
COORD      bufferCoord = {0, 0};
SMALL_RECT writeRegion = {0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1};

#endif

void init_console()
{

#ifdef _WIN32
        hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        SetConsoleMode(hConsole, ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING);

#endif
}

void hide_cursor()
{

#ifdef _WIN32

        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        CONSOLE_CURSOR_INFO cursorInfo;

        GetConsoleCursorInfo(hConsole, &cursorInfo);

        cursorInfo.bVisible = FALSE; // Hide cursor

        SetConsoleCursorInfo(hConsole, &cursorInfo);
#else

        printf("\033[?25l");

#endif
}

void show_cursor()
{

#ifdef _WIN32

        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        CONSOLE_CURSOR_INFO cursorInfo;

        GetConsoleCursorInfo(hConsole, &cursorInfo);

        cursorInfo.bVisible = TRUE; // Show cursor

        SetConsoleCursorInfo(hConsole, &cursorInfo);
#else

        printf("\033[?25h");

#endif
}

void render_buffer()
{
#ifdef _WIN32
        // Convert wchar_t buffer to CHAR_INFO buffer
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

void clear_buffer()
{
        for (int i = 0; i < SCREEN_HEIGHT; i++)
        {

                wmemset(buffer[i], L' ', SCREEN_WIDTH);
        }
}

void draw_edges(wchar_t vertical, wchar_t horizontal, wchar_t corner)
{
        for (int i = 0; i < SCREEN_WIDTH; i++)
        {
                buffer[SCREEN_HEIGHT - 1][i] = horizontal;
        }

        for (int i = 0; i < SCREEN_HEIGHT; i++)
        {
                buffer[i][SCREEN_WIDTH - 1] = vertical;
        }

        buffer[SCREEN_HEIGHT - 1][SCREEN_WIDTH - 1] = corner;
}

void draw_coordinate_system()
{
        for (int i = 0; i < SCREEN_HEIGHT; i++)
        {
                buffer[i][(SCREEN_WIDTH / 2) - 1] = L'|';
        }

        for (int i = 0; i < SCREEN_WIDTH; i++)
        {
                buffer[(SCREEN_HEIGHT / 2) - 1][i] = L'-';
        }

        buffer[(SCREEN_HEIGHT / 2) - 1][(SCREEN_WIDTH / 2) - 1] = L'+';
}

terminal TERMINAL = {
    .init_console           = init_console,
    .hide_cursor            = hide_cursor,
    .show_cursor            = show_cursor,
    .render_buffer          = render_buffer,
    .clear_buffer           = clear_buffer,
    .draw_edges             = draw_edges,
    .draw_coordinate_system = draw_coordinate_system

};
