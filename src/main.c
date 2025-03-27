/*===============================================================================================*
 *
 * The MAIN file. Render Engine entrypoint.
 *
 *===============================================================================================*/

#include <conio.h>
#include <math.h>

#include "../include/globals.h"
#include "../include/math/utils.h"
#include "../include/engine/mesh.h"

#include <stdio.h>

FILE *log_file;

double factor          = 0.1f;
double rotation_factor = 0.0f;
double phase           = 0.0f;
double vert_shift      = 0.0f;

double (*current_fn)(double);

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

int main()
{
        Mesh m;

        /**
         *
         *	UNIT CUBE:
         *                            NORTH
         *                              ^
         *                             /
         *
         *             (0,1,1) x---------------x (1,1,1)
         *           	      /.              /|
         *	     	     / .	     / |
         *	    	    /  .    TOP     /  |
         *	   	   /   .	   /   |
         *	  	  /    .	  /    |
         *	 	 /     .	 /     |
         *	(0,1,0)	x---------------x (1,1,0)
         *		|      .	|      |
         *     WEST <- (0,0,1) x........|......x (1,0,1) -> EAST
         *		|     .         |     /
         *		|    .          |    /
         *		|   .   BOTTOM  |   /
         *		|  .            |  /
         *		| .             | /
         *	        |.              |/
         *	(0,0,0) x---------------x (1,0,0)
         *
         *			/
         *		       V
         *		     SOUTH
         */

        Triangle tris[] = {
            // SOUTH
            {{{0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f}}},
            {{{0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}}},

            // EAST
            {{{1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}}},
            {{{1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 1.0f}}},

            // NORTH
            {{{1.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0f}}},
            {{{1.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}}},

            // WEST
            {{{0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}}},
            {{{0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}}},

            // TOP
            {{{0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}}},
            {{{0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 0.0f}}},

            // BOTTOM
            {{{1.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}}},
            {{{1.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}}}};

        const int num_tris = sizeof(tris) / sizeof(Triangle);

        mesh_init(&m, num_tris);

        // Bulk add triangles
        for (int i = 0; i < num_tris; i++)
        {
                mesh_add(&m, tris[i]);
        }

        // mesh_info(&m);

        mesh_info(&m);

        log_file = fopen("logs.txt", "a");

        Core.Terminal.hide_cursor();

        Core.Terminal.init_console();

        Core.Utils.init_measurement();

        // current_fn = sin_function;

        Mat4x4 proj;

        projection_matrix_init(proj);

        Triangle tri_projd = {
            {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}}
        };

        while (1)
        {
                Core.Utils.measure_start();

                Core.Terminal.clear_buffer();

                Core.Utils.draw_coordinate_system();

                for (int i = 0; i < (int)m.size; i++)
                {
                        // printf(
                        //     "%llu\n\t%f\n\t\%f\n\t\%f\n", sizeof *(m.tris + i), (m.tris + i)->points[0].x,
                        //     (m.tris + i)->points[0].y, (m.tris + i)->points[0].z);

                        multiply_matrix_vector(&(m.tris + i)->points[0], &(tri_projd).points[0], &proj);
                        multiply_matrix_vector(&(m.tris + i)->points[1], &(tri_projd).points[1], &proj);
                        multiply_matrix_vector(&(m.tris + i)->points[2], &(tri_projd).points[2], &proj);

                        Triangle t = *(m.tris + i);

                        float x0   = t.points[0].x * 20 * 2;
                        float y0   = t.points[0].y * 20;

                        float x1   = t.points[1].x * 20 * 2;
                        float y1   = t.points[1].y * 20;

                        float x2   = t.points[2].x * 20 * 2;
                        float y2   = t.points[2].y * 20;

                        bla(x0, y0, x1, y1);
                        bla(x1, y1, x2, y2);
                        bla(x2, y2, x0, y0);
                }

                Core.Utils.draw_stats();

                Core.Utils.draw_edges('|', '-', '+');

                Core.Terminal.render_buffer();

                Core.Utils.measure_end();

                Core.Utils.measure_diff();

                if (_kbhit())
                {
                        char c = _getch();

                        if (c == 'r')
                                break;
                }
        }

        mesh_free(&m);

        mesh_info(&m);

        Core.Terminal.show_cursor();

        fclose(log_file);

        return 0;
}
