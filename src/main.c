#include "../include/core/core.h"

int main()
{
        CORE.TERMINAL.hide_cursor();

        CORE.TERMINAL.init_console();

        while (1)
        {
                CORE.TERMINAL.clear_buffer();

                CORE.UTILS.draw_coordinate_system();

                CORE.UTILS.draw_stats(0.0f);

                CORE.UTILS.draw_edges('|', '-', '+');

                CORE.TERMINAL.render_buffer();
        }

        CORE.TERMINAL.show_cursor();

        return 0;
}
