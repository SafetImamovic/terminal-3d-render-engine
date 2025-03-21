#include "../include/core/core.h"

int main()
{
        CORE.TERMINAL.hide_cursor();

        CORE.TERMINAL.init_console();

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
