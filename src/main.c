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

float trans_x = 0.0f;
float trans_y = 0.0f;
float trans_z = 0.0f;

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
            {{{0.0f, 0.0f, 0.0f}, {0.0f, 0.5f, 0.0f}, {0.5f, 0.5f, 0.0f}}},
            {{{0.0f, 0.0f, 0.0f}, {0.5f, 0.5f, 0.0f}, {0.5f, 0.0f, 0.0f}}},

            // EAST
            {{{0.5f, 0.0f, 0.0f}, {0.5f, 0.5f, 0.0f}, {0.5f, 0.5f, 0.5f}}},
            {{{0.5f, 0.0f, 0.0f}, {0.5f, 0.5f, 0.5f}, {0.5f, 0.0f, 0.5f}}},

            // NORTH
            {{{0.5f, 0.0f, 0.5f}, {0.5f, 0.5f, 0.5f}, {0.0f, 0.5f, 0.5f}}},
            {{{0.5f, 0.0f, 0.5f}, {0.0f, 0.5f, 0.5f}, {0.0f, 0.0f, 0.5f}}},

            // WEST
            {{{0.0f, 0.0f, 0.5f}, {0.0f, 0.5f, 0.5f}, {0.0f, 0.5f, 0.0f}}},
            {{{0.0f, 0.0f, 0.5f}, {0.0f, 0.5f, 0.0f}, {0.0f, 0.0f, 0.0f}}},

            // TOP
            {{{0.0f, 0.5f, 0.0f}, {0.0f, 0.5f, 0.5f}, {0.5f, 0.5f, 0.5f}}},
            {{{0.0f, 0.5f, 0.0f}, {0.5f, 0.5f, 0.5f}, {0.5f, 0.5f, 0.0f}}},

            // BOTTOM
            {{{0.5f, 0.0f, 0.5f}, {0.0f, 0.0f, 0.5f}, {0.0f, 0.0f, 0.0f}}},
            {{{0.5f, 0.0f, 0.5f}, {0.0f, 0.0f, 0.0f}, {0.5f, 0.0f, 0.0f}}}};

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

        projection_matrix_init(&proj);

        // orthogonal_projection_matrix_init(&proj);

        Triangle tri_projd = {0}, tri_trans = {0};

        while (1)
        {
                Core.Utils.measure_start();

                Core.Terminal.clear_buffer();

                Core.Utils.draw_coordinate_system();

                for (int i = 0; i < (int)m.size; i++)
                {
                        tri_trans = *(m.tris + i);

                        // All of the `trans_variable` are shifts that increment
                        // or decrement by 0.1f, they are all 0.0f by default
                        // so they don't affect the initial position, scale or rotation.
                        tri_trans.points[0].z += trans_z;
                        tri_trans.points[1].z += trans_z;
                        tri_trans.points[2].z += trans_z;

                        tri_trans.points[0].x += trans_x;
                        tri_trans.points[1].x += trans_x;
                        tri_trans.points[2].x += trans_x;

                        tri_trans.points[0].y += trans_y;
                        tri_trans.points[1].y += trans_y;
                        tri_trans.points[2].y += trans_y;

                        // Scaling the x axis by 2 because of the characters
                        // height:width ratio in the terminal.
                        tri_trans.points[0].x *= 2;
                        tri_trans.points[1].x *= 2;
                        tri_trans.points[2].x *= 2;

                        multiply_matrix_vector(&(tri_trans).points[0], &(tri_projd).points[0], &proj);
                        multiply_matrix_vector(&(tri_trans).points[1], &(tri_projd).points[1], &proj);
                        multiply_matrix_vector(&(tri_trans).points[2], &(tri_projd).points[2], &proj);

                        float x0 = tri_projd.points[0].x;
                        float y0 = tri_projd.points[0].y;
                        float x1 = tri_projd.points[1].x;
                        float y1 = tri_projd.points[1].y;
                        float x2 = tri_projd.points[2].x;
                        float y2 = tri_projd.points[2].y;

                        x0 += 1.0f;
                        y0 += 1.0f;
                        x1 += 1.0f;
                        y1 += 1.0f;
                        x2 += 1.0f;
                        y2 += 1.0f;

                        x0 *= 0.5f * (float)SCREEN_WIDTH;
                        y0 *= 0.5f * (float)SCREEN_HEIGHT;
                        x1 *= 0.5f * (float)SCREEN_WIDTH;
                        y1 *= 0.5f * (float)SCREEN_HEIGHT;
                        x2 *= 0.5f * (float)SCREEN_WIDTH;
                        y2 *= 0.5f * (float)SCREEN_HEIGHT;

                        int _x0 = (int)x0;
                        int _y0 = (int)y0;
                        int _x1 = (int)x1;
                        int _y1 = (int)y1;
                        int _x2 = (int)x2;
                        int _y2 = (int)y2;

                        bla(_x0, _y0, _x1, _y1);
                        bla(_x1, _y1, _x2, _y2);
                        bla(_x2, _y2, _x0, _y0);
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
                        else if (c == 'a')
                                trans_x += 0.1f;
                        else if (c == 'd')
                                trans_x -= 0.1f;
                        else if (c == 'w')
                                trans_z += 0.1f;
                        else if (c == 's')
                                trans_z -= 0.1f;
                        else if (c == 'e')
                                trans_y += 0.1f;
                        else if (c == 'q')
                                trans_y -= 0.1f;
                }
        }

        mesh_free(&m);

        mesh_info(&m);

        Core.Terminal.show_cursor();

        fclose(log_file);

        return 0;
}
