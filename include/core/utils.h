#pragma once

#include "../../include/constants.h"
#include "../../include/globals.h"

void draw_edges(wchar_t vertical, wchar_t horizontal, wchar_t corner);

void draw_coordinate_system();

void draw_stats();

void init_measurement();

void measure_start();

void measure_end();

void measure_diff();
