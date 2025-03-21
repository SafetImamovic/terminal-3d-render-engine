/*===============================================================================================*
 *
 * The MAIN file. Render Engine entrypoint.
 *
 *===============================================================================================*/

#include "../include/globals.h"

int main()
{
        Core.Terminal.hide_cursor();

        Core.Terminal.init_console();

        Core.Utils.init_measurement();

        while (1)
        {
                Core.Utils.measure_start();

                Core.Terminal.clear_buffer();

                Core.Utils.draw_coordinate_system();

                Core.Utils.draw_stats();

                Core.Utils.draw_edges('|', '-', '+');

                Core.Terminal.render_buffer();

                Core.Utils.measure_end();

                Core.Utils.measure_diff();
        }

        Core.Terminal.show_cursor();

        return 0;
}
