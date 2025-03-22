/*===============================================================================================*
 *
 * File: utils.h
 *
 * Description: Provides utility functions for drawing, performance measurement, and system-specific
 *              operations.
 *
 *              Supports both Windows and Unix-like systems (Or at least I tried my best =p).
 *
 *===============================================================================================*/

#ifndef UTILS_H
#define UTILS_H

#include "../../include/constants.h"
#include "../../include/globals.h"

/**
 * Function: draw_edges
 *
 * Draws the edges of the buffer using the specified characters.
 */
void draw_edges(wchar_t vertical, wchar_t horizontal, wchar_t corner);

/**
 * Function: draw_coordinate_system
 *
 * Draws a coordinate system at the center of the buffer.
 */
void draw_coordinate_system();

/**
 * Function: init_measurement
 *
 * Initializes the timing mechanism for performance measurement.
 */
void init_measurement();

/**
 * Function: measure_start
 *
 * Starts the performance measurement.
 */
void measure_start();

/**
 * Function: measure_end
 *
 * Ends the performance measurement.
 */
void measure_end();

/**
 * Function: measure_diff
 *
 * Calculates the time difference between the start and end measurements.
 */
void measure_diff();

/**
 * Function: draw_stats
 *
 * Displays the elapsed time and FPS on the first row of the buffer.
 */
void draw_stats();

#endif
