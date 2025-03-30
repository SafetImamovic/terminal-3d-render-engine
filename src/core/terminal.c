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

void calculate_true_elapsed_time()
{
        if (FPS_LIMIT == 0)
        {
                true_elapsed_time = elapsed_time;

                return;
        }

        // How long each frame takes in ms in a span of 1 second for a given frame rate.
        // FPS_LIMIT = 60; time_per_frame = 1000ms/60frames > 16.7ms
        time_per_frame = 1000.0f / (float)FPS_LIMIT;

        // Remaining frame time after the loop pass
        remaining_time_per_frame = time_per_frame - elapsed_time;

        if (remaining_time_per_frame <= 0)
        {
                return;
        }

        Sleep(remaining_time_per_frame);

        true_elapsed_time = remaining_time_per_frame + elapsed_time;
}

#ifdef _WIN32
#else
int _kbhit(void)
{
        struct termios oldt, newt;
        int            ch;
        int            oldf;

        tcgetattr(STDIN_FILENO, &oldt); // Get current terminal attributes

        newt = oldt;

        newt.c_lflag &= ~(ICANON | ECHO); // Disable canonical mode & echo

        tcsetattr(STDIN_FILENO, TCSANOW, &newt);

        oldf = fcntl(STDIN_FILENO, F_GETFL, 0);

        fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK); // Set non-blocking mode

        ch = getchar(); // Read a single character

        tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // Restore terminal settings

        fcntl(STDIN_FILENO, F_SETFL, oldf); // Restore flags

        if (ch != EOF)
        {
                ungetc(ch, stdin); // Put character back into buffer

                return 1;
        }

        return 0;
}

// Function to get a single character
char _getch(void)
{
        struct termios oldt, newt;

        char ch;

        tcgetattr(STDIN_FILENO, &oldt);

        newt = oldt;

        newt.c_lflag &= ~(ICANON | ECHO);

        tcsetattr(STDIN_FILENO, TCSANOW, &newt);

        ch = getchar();

        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

        return ch;
}
#endif
