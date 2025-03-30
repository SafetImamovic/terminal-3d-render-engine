/*===============================================================================================*
 *
 * File: utils.c
 *
 * Description: Provides utility functions for drawing, performance measurement, and system-specific
 *              operations.
 *
 *              Supports both Windows and Unix-like systems (Or at least I tried my best =p).
 *
 *===============================================================================================*/

#include <time.h>
#include <stdio.h>
#include "../../include/core/utils.h"

#ifdef _WIN32
#else
#include <signal.h>
#endif

/**
 * Function: draw_edges
 *
 * Draws the edges of the buffer using the specified characters.
 */
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

/**
 * Function: draw_coordinate_system
 *
 * Draws a coordinate system at the center of the buffer.
 */
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

/**
 * Function: init_measurement
 *
 * Initializes the timing mechanism for performance measurement.
 */
void init_measurement()
{
#ifdef _WIN32
        QueryPerformanceFrequency(&frequency);
#else
        sa.sa_flags = 0;
        sigemptyset(&sa.sa_mask);
        if (sigaction(SIGWINCH, &sa, NULL) == -1)
        {
                perror("sigaction");
                return;
        }
#endif
}

/**
 * Function: measure_start
 *
 * Starts the performance measurement.
 */
void measure_start()
{
#ifdef _WIN32
        QueryPerformanceCounter(&start);
#else
        clock_gettime(CLOCK_MONOTONIC, &start);
#endif
}

/**
 * Function: measure_end
 *
 * Ends the performance measurement.
 */
void measure_end()
{
#ifdef _WIN32
        QueryPerformanceCounter(&end);
#else
        clock_gettime(CLOCK_MONOTONIC, &end);
#endif
}

/**
 * Function: measure_diff
 *
 * Calculates the time difference between the start and end measurements.
 */
void measure_diff()
{
#ifdef _WIN32
        elapsed_time = (double)(end.QuadPart - start.QuadPart) * 1000.0 / frequency.QuadPart;
#else
        elapsed_time = (end.tv_sec - start.tv_sec) * 1000.0 + (end.tv_nsec - start.tv_nsec) / 1000000.0;
#endif
}

/**
 * Function: draw_stats
 *
 * Displays the elapsed time and FPS on the first row of the buffer.
 */
void draw_stats()
{
        double fps = 1000.0 / true_elapsed_time;

        // Ensure the string is exactly WIDTH characters
        swprintf(buffer[0], SCREEN_WIDTH, L"Elapsed Time: %.3f ms | FPS: %.2f", true_elapsed_time, fps);

        // Manually fill the remaining space with spaces
        size_t len = wcslen(buffer[0]);
        for (size_t i = len; i < SCREEN_WIDTH; i++)
        {
                buffer[0][i] = L' ';
        }
}
