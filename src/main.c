/*===============================================================================================*
 *
 * The MAIN file. Render Engine entrypoint.
 *
 *===============================================================================================*/

#include <math.h>
#include "../include/globals.h"
#include <stdio.h>
#include <stdio.h>

FILE *log_file;

double factor          = 0.1f;
double rotation_factor = 0.0f;

double circle_function(double x)
{

        double radius = 100.0f;

        // Check if x is within the circle's range
        if (x > (int)-radius && x < (int)radius)
                return sqrt(radius * radius - x * x);

        return SCREEN_HEIGHT + 1;
}

double linear_function(double x)
{
        double m = 1.0f;

        double b = 0;

        double y = m * x + b;

        return y;
}

double sin_function(double x)
{
        double y = sin(factor * x) / factor;

        return y;
}

double log_function(double x)
{
        if (x < 0)
                return SCREEN_HEIGHT + 1;

        double y = log(x) / factor;

        return y;
}

double reciprocal_function(double x)
{
        double y = 0.0f;

        if (x != 0)
                y = (1.0f / x) / (factor * factor * factor);

        return y;
}

double quadratic_function(double x)
{
        double y = (factor * x) * (factor * x);
        return y;
}

double cubic_function(double x)
{

        double y = (factor * x) * (factor * x) * (factor * x);
        return y;
}

double e_function(double x)
{
        double y = exp(factor * x);

        return y;
}

void rebase(int *x, double *y)
{
        // This scaling factor is just the individual
        // Terminal Character height / width:
        //
        // I don't really know what that is and it seems the change
        // depending on the scaling of the terminal itself (like
        // the zoom of the terminal).
        //
        // Also the line height plays a role in this aswell.
        //
        // I'm just approximating it to 2, as in a character is
        // twice the height of it's width.
        *x = *x * 2.0f;

        *y += SCREEN_HEIGHT / 2.0f;

        *x += SCREEN_WIDTH / 2.0f;

        *y = SCREEN_HEIGHT - *y - 1;

        if (*y > SCREEN_HEIGHT || *y < 0)
                return;
}

void draw_pixel(double x, double y)

{
        int _x = (int)x;

        rebase(&_x, &y);

        if (y < 0 || y > SCREEN_HEIGHT - 1)
                return;

        if (_x < 0 || _x > SCREEN_WIDTH - 1)
                return;

        buffer[(int)y][_x] = full_block;
}

/**
 * General 2D Counter Clockwise Rotation Matrix:
 *
 *	| x' |   | cos(a)  -sin(a) |   |  x   |
 *	|    | = |	           | * |      |
 *	| y' |   | sin(a)   cos(a) |   | f(x) |
 */
void rotate_function(double (*fn)(double), float theta, int x, int *out_x, int *out_y)
{

        double y         = fn(x); // f(x)

        double cos_theta = cosf(theta); // cos(a)
        double sin_theta = sinf(theta); // sin(a)

        // x' = | cos(a)  -sin(a) | * |  x   |
        //                            |      |
        //                            | f(x) |
        double rotated_x = x * cos_theta - y * sin_theta;

        // y' = | sin(a)   cos(a) | * |  x   |
        //                            |      |
        //                            | f(x) |
        double rotated_y = x * sin_theta + y * cos_theta;

        // Cast to ints:
        *out_x = (int)(rotated_x);
        *out_y = (int)(rotated_y);
}

/**
 *	SW = SCREEN_WIDTH
 *	SH = SCREEN_HEIGHT
 *
 *
 *      (-SW/2, -SH/2)                                       (0,0)
 *                      |                                                         |
 *                      |                                                         |
 *                      |                                                         |
 *                      |                                                         |
 *                      |                                                         |
 *                      |                                                         |
 *	<---------------+--------------->         -->         <-------------------+------------------>
 *                      |                                                         |
 *                      |                                                         |
 *                      |                                                         |
 *                      |                                                         |
 *                      |                                                         |
 *                      |                                                         |
 *                              (SW/2, SH/2)                                                    (SW, SH)
 *
 */

void draw_function(double (*fn)(int), int x)
{
        double y = fn(x);

        rebase(&x, &y);

        fprintf(log_file, "\n x: %i, y: %f, _y: %i", x, y, (int)y);

        buffer[(int)y][x] = full_block;
}

int main()
{
        log_file = fopen("logs.txt", "a");

        int rotated_x, rotated_y;

        float theta = rotation_factor * PI; // 45-degree rotation

        Core.Terminal.hide_cursor();

        Core.Terminal.init_console();

        Core.Utils.init_measurement();

        while (1)
        {
                Core.Utils.measure_start();

                Core.Terminal.clear_buffer();

                Core.Utils.draw_coordinate_system();

                for (int x = -SCREEN_WIDTH / 2; x < SCREEN_WIDTH / 2; x++)
                {
                        // draw_function(e_function, x);
                        // draw_function(log_function, x);
                        // draw_function(linear_function, x);

                        rotate_function(sin_function, theta, x, &rotated_x, &rotated_y);

                        // draw_function(reciprocal_function, x);

                        draw_pixel(rotated_x, rotated_y);

                        // draw_function(circle_function, x);

                        // rotate_function(circle_function, theta, x, &rotated_x, &rotated_y);

                        // draw_pixel(rotated_x, rotated_y);
                }

                Core.Utils.draw_stats();

                Core.Utils.draw_edges('|', '-', '+');

                Core.Terminal.render_buffer();

                Core.Utils.measure_end();

                Core.Utils.measure_diff();

                // factor -= 0.000005f;

                rotation_factor += 0.001f;

                theta = rotation_factor * PI;
        }

        Core.Terminal.show_cursor();

        fclose(log_file);

        return 0;
}
