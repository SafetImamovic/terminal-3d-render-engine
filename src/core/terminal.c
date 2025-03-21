#include "../../include/core/terminal.h"

#ifdef _WIN32

HANDLE     hConsole;
CHAR_INFO  consoleBuffer[SCREEN_HEIGHT * SCREEN_WIDTH];
COORD      bufferSize  = {SCREEN_WIDTH, SCREEN_HEIGHT};
COORD      bufferCoord = {0, 0};
SMALL_RECT writeRegion = {0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1};

void initConsole()
{
        hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        SetConsoleMode(hConsole, ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}

#endif

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

void renderBuffer()
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

        for (int y = 0; y < HEIGHT; y++)
        {
                for (int x = 0; x < WIDTH; x++)
                {
                        putwchar(buffer[y][x]);
                }

                putwchar(L'\n'); // Move to next line
        }

        // fflush(stdout); <- Might introduce latency, should profile it later.
#endif
}
