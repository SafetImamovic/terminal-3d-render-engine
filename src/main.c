#include "../include/core/core.h"

int main()
{
        CORE.TERMINAL.hide_cursor();

        CORE.TERMINAL.init_console();

        // TODO: Pretify this whole thing.
#ifdef _WIN32

        LARGE_INTEGER frequency, start, end;

        QueryPerformanceFrequency(&frequency);

#else

        struct timespec start, end;

        // Set up the signal handler for SIGWINCH
        struct sigaction sa;

        sa.sa_flags = 0;

        sigemptyset(&sa.sa_mask);

        if (sigaction(SIGWINCH, &sa, NULL) == -1)
        {
                perror("sigaction");

                return 1;
        }

#endif

        double elapsedTime = 0.0f;

        while (1)
        {

#ifdef _WIN32

                QueryPerformanceCounter(&start);

#else

                clock_gettime(CLOCK_MONOTONIC, &start);

#endif

                CORE.TERMINAL.clear_buffer();

                CORE.UTILS.draw_coordinate_system();

                CORE.UTILS.draw_stats(elapsedTime);

                CORE.UTILS.draw_edges('|', '-', '+');

                CORE.TERMINAL.render_buffer();

#ifdef _WIN32

                QueryPerformanceCounter(&end);

                elapsedTime = (double)(end.QuadPart - start.QuadPart) * 1000.0 / frequency.QuadPart;

#else

                clock_gettime(CLOCK_MONOTONIC, &end);

                elapsedTime = (end.tv_sec - start.tv_sec) * 1000.0 + (end.tv_nsec - start.tv_nsec) / 1000000.0;

#endif
        }

        CORE.TERMINAL.show_cursor();

        return 0;
}
