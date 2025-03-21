#include "../include/core/core.h"

int main()
{
        CORE.TERMINAL.hide_cursor();

        CORE.TERMINAL.init_console();

        // TODO: Pretify this whole thing.

        CORE.UTILS.init_measurement();

#ifdef _WIN32
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

        while (1)
        {

                CORE.UTILS.measure_start();

#ifdef _WIN32

#else

                clock_gettime(CLOCK_MONOTONIC, &start);

#endif

                CORE.TERMINAL.clear_buffer();

                CORE.UTILS.draw_coordinate_system();

                CORE.UTILS.draw_stats();

                CORE.UTILS.draw_edges('|', '-', '+');

                CORE.TERMINAL.render_buffer();

                CORE.UTILS.measure_end();
#ifdef _WIN32

                CORE.UTILS.measure_diff();

#else

                clock_gettime(CLOCK_MONOTONIC, &end);

                elapsedTime = (end.tv_sec - start.tv_sec) * 1000.0 + (end.tv_nsec - start.tv_nsec) / 1000000.0;

#endif
        }

        CORE.TERMINAL.show_cursor();

        return 0;
}
