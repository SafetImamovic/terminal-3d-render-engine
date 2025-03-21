#include "../include/globals.h"

int main()
{
        CORE.TERMINAL.hide_cursor();

        CORE.TERMINAL.init_console();

        CORE.UTILS.init_measurement();

        while (1)
        {

                CORE.UTILS.measure_start();

                CORE.TERMINAL.clear_buffer();

                CORE.UTILS.draw_coordinate_system();

                CORE.UTILS.draw_stats();

                CORE.UTILS.draw_edges('|', '-', '+');

                CORE.TERMINAL.render_buffer();

                CORE.UTILS.measure_end();

                CORE.UTILS.measure_diff();
        }

        CORE.TERMINAL.show_cursor();

        return 0;
}
