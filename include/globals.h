/*===============================================================================================*
 *
 * The GLOBALS HEADER file.
 *
 * Holds various types of global declarations of different variables and structs.
 *
 *===============================================================================================*/

#ifndef GLOBALS_H
#define GLOBALS_H

#include <wchar.h>
#include "constants.h"

#ifdef _WIN32

#include <windows.h>

#endif

/*===============================================================================================*
 *
 * Main global buffer declaration. The SCREEN_HEIGHT and SCREEN_WIDTH
 * are inherited from `constants.h`.
 *
 *===============================================================================================*/

extern wchar_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH];

/*===============================================================================================*
 *
 * Different Platform Dependant variables and structs which are needed for time
 * measurements.
 *
 *===============================================================================================*/

extern const wchar_t full_block;

extern double elapsed_time;

#ifdef _WIN32

extern LARGE_INTEGER frequency, start, end;

#else

extern struct timespec start, end;

// Set up the signal handler for SIGWINCH
extern struct sigaction sa;

#endif

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
 * The `terminal` struct holds pointers to terminal functions.
 */
typedef struct
{
        /**
         * Function: init_console
         *
         * Initializes the console for rendering. On Windows, enables virtual terminal
         * processing for ANSI escape code support.
         */
        void (*init_console)(void);

        /**
         * Function: hide_cursor
         *
         * Hides the terminal cursor to prevent flickering during rendering.
         */
        void (*hide_cursor)(void);

        /**
         * Function: show_cursor
         *
         * Restores visibility of the terminal cursor.
         */
        void (*show_cursor)(void);

        /**
         * Function: render_buffer
         *
         * Renders the screen buffer to the console. On Windows, uses WriteConsoleOutputW
         * for direct buffer manipulation. On Unix, prints the buffer using ANSI sequences.
         */
        void (*render_buffer)(void);

        /**
         * Function: clear_buffer
         *
         * Clears the screen buffer by filling it with spaces.
         */
        void (*clear_buffer)(void);

} terminal;

/**
 * The `utils` struct holds pointers to various utility functions.
 */
typedef struct
{
        /**
         * Function: draw_edges
         *
         * Draws the edges of the buffer using the specified characters.
         */
        void (*draw_edges)(wchar_t, wchar_t, wchar_t);

        /**
         * Function: draw_coordinate_system
         *
         * Draws a coordinate system at the center of the buffer.
         */
        void (*draw_coordinate_system)(void);

        /**
         * Function: init_measurement
         *
         * Initializes the timing mechanism for performance measurement.
         */
        void (*init_measurement)(void);

        /**
         * Function: measure_start
         *
         * Starts the performance measurement.
         */
        void (*measure_start)(void);

        /**
         * Function: measure_end
         *
         * Ends the performance measurement.
         */
        void (*measure_end)(void);

        /**
         * Function: measure_diff
         *
         * Calculates the time difference between the start and end measurements.
         */
        void (*measure_diff)(void);

        /**
         * Function: draw_stats
         *
         * Displays the elapsed time and FPS on the first row of the buffer.
         */
        void (*draw_stats)(void);

} utils;

/**
 * The `core` struct holds the `Terminal` and `Utils` structs as members.
 */
typedef struct
{
        /**
         * Terminal struct that holds pointers to functions related
         * to terminal interaction like:
         *
         * 	void (*init_console)(void);
         * 	void (*hide_cursor)(void);
         * 	void (*show_cursor)(void);
         * 	void (*render_buffer)(void);
         * 	void (*clear_buffer)(void);
         */
        terminal Terminal;

        /**
         * Utils struct that holds pointers to functions related
         * to various types of utilities like:
         *
         *	void (*draw_edges)(wchar_t, wchar_t, wchar_t);
         *	void (*draw_coordinate_system)(void);
         *	void (*draw_stats)(void);
         *	void (*init_measurement)(void);
         *	void (*measure_start)(void);
         *	void (*measure_end)(void);
         *	void (*measure_diff)(void);
         */
        utils Utils;

} core;

/**
 * Core struct that holds other structs like:
 * 	- Terminal
 * 	- Utils
 */
extern core Core;

/*===============================================================================================*
 *
 * Mathematical Structs.
 *
 * Different mathematical structs like:
 * 	- Vec3D 	<- (x, y, z) position of a point.
 * 	- Triangle 	<- An array of 3 Vec3D points.
 * 	- Mesh 		<- A collection of an arbitrary amount of triangles.
 *
 *===============================================================================================*/

/**
 * Vec3D <- (x, y, z) position of a point.
 */
typedef struct
{
        float x, y, z;
} Vec3D;

/**
 * Triangle <- An array of 3 Vec3D points.
 */
typedef struct
{
        Vec3D points[3];
} Triangle;

/**
 * Mesh	<- A collection of an arbitrary amount of triangles.
 */
typedef struct
{
        size_t    size;
        size_t    capacity;
        Triangle *tris;

} Mesh;

#endif
