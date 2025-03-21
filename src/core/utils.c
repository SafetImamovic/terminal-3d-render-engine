#include "../../include/core/utils.h"

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

void init_measurement()
{

#ifdef _WIN32

        QueryPerformanceFrequency(&frequency);

#else

#endif
}

void measure_start()
{

#ifdef _WIN32

        QueryPerformanceCounter(&start);

#else

#endif
}

void measure_end()
{

#ifdef _WIN32

        QueryPerformanceCounter(&end);

#else

#endif
}

void measure_diff()
{

#ifdef _WIN32

        elapsed_time = (double)(end.QuadPart - start.QuadPart) * 1000.0 / frequency.QuadPart;

#else

#endif
}

void draw_stats()
{
        double fps = 1000.0 / elapsed_time;

        // Ensure the string is exactly WIDTH characters
        swprintf(buffer[0], SCREEN_WIDTH, L"Elapsed Time: %.3f ms | FPS: %.2f", elapsed_time, fps);

        // Manually fill the remaining space with spaces
        size_t len = wcslen(buffer[0]);

        for (size_t i = len; i < SCREEN_WIDTH; i++)
        {
                buffer[0][i] = L' ';
        }
}
