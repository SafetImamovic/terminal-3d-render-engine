/*===============================================================================================*
 *
 * The GLOBALS SOURCE file.
 *
 * This file contains the actual definitions of global variables and
 * initializes function pointers in the `Core` struct.
 *
 *===============================================================================================*/

#include "../include/globals.h"
#include "../include/core/terminal.h"
#include "../include/core/utils.h"

/*===============================================================================================*
 *
 * Main global buffer definition. The SCREEN_HEIGHT and SCREEN_WIDTH
 * are inherited from `constants.h`.
 *
 *===============================================================================================*/

wchar_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH];

/*===============================================================================================*
 *
 * Different Platform Dependant variables and structs which are needed for time
 * measurements.
 *
 *===============================================================================================*/

const wchar_t full_block = L'\u2588';

#ifdef _WIN32

LARGE_INTEGER frequency, start, end;

#else

struct timespec start, end;

// Set up the signal handler for SIGWINCH
struct sigaction sa;

#endif

double elapsed_time = 0.0f;

/*===============================================================================================*
 *
 * Function Structs.
 *
 * Basically something like Interfaces in OOP. These structs hold pointers to functions
 * related to that struct.
 *
 * EXAMPLE:
 *
 * 	CORE.UTILS.init_measurement(); <- Calls the `init_measurement()` function.
 *
 *===============================================================================================*/

/**
 * Initializes the global `Core` struct and assigns function pointers.
 */
core Core = {
    .Terminal =
        {
                   .init_console  = init_console,
                   .hide_cursor   = hide_cursor,
                   .show_cursor   = show_cursor,
                   .render_buffer = render_buffer,
                   .clear_buffer  = clear_buffer,
                   },
    .Utils = {
                   .draw_edges             = draw_edges,
                   .draw_coordinate_system = draw_coordinate_system,
                   .draw_stats             = draw_stats,
                   .init_measurement       = init_measurement,
                   .measure_start          = measure_start,
                   .measure_end            = measure_end,
                   .measure_diff           = measure_diff}
};
