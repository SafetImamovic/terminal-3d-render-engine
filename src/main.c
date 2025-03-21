#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#else
// #include <time.h>
#include <unistd.h>
#include <sys/ioctl.h>
// #include <signal.h>
#include <fcntl.h>
#endif

#include "../include/core/core.h"

int main()
{
        CORE.TERMINAL.hide_cursor();

        while (1)
        {
                CORE.TERMINAL.clear_buffer();

                CORE.TERMINAL.draw_coordinate_system();

                CORE.TERMINAL.draw_edges('|', '-', '+');

                CORE.TERMINAL.render_buffer();
        }

        CORE.TERMINAL.show_cursor();

        return 0;
}
