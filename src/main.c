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

int main()
{
        Mesh m;

        const int num_tris = sizeof(cube) / sizeof(Triangle);

        mesh_init(&m, num_tris);

        // Bulk add triangles
        for (int i = 0; i < num_tris; i++)
        {
                mesh_add(&m, cube[i]);
        }

        mesh_info(&m);

        log_file = fopen("logs.txt", "a");

        Core.Terminal.hide_cursor();

        Core.Terminal.init_console();

        Core.Utils.init_measurement();

        Mat4x4 proj;

        projection_matrix_init(&proj);

        Triangle tri_init = {0};

        Mat4x4 rotate_z = {0}, rotate_x = {0}, rotate_y = {0}, translate = {0};

        float fAlpha             = 0.0f;

        Mat4x4 translate_neg_025 = {0};

        translation_matrix_init(&translate_neg_025, -0.25f);

        Mat4x4 translate_pos_025 = {0};

        translation_matrix_init(&translate_pos_025, 0.25f);

        while (1)
        {
                Core.Utils.measure_start();

                Core.Terminal.clear_buffer();

                Core.Utils.draw_coordinate_system();

                fAlpha += 0.005f;

                rotation_matrix_x_init(&rotate_x, fAlpha);

                rotation_matrix_y_init(&rotate_y, fAlpha);

                for (int i = 0; i < (int)m.size; i++)
                {
                        tri_init = *(m.tris + i);

                        // All of the `trans_variable` are shifts that increment
                        // or decrement by 0.1f, they are all 0.0f by default
                        // so they don't affect the initial position, scale or rotation.

                        Triangle tri_trans_neg25 = {0};

                        // Translate the points negatively to center them at the origin
                        for (int i = 0; i < 3; i++)
                        {
                                multiply_matrix_vector(
                                    &(tri_init).points[i], &(tri_trans_neg25).points[i], &translate_neg_025);
                        }

                        Triangle tri_rotated = {0};

                        // Apply the rotation after translation
                        for (int i = 0; i < 3; i++)
                        {
                                multiply_matrix_vector(
                                    &(tri_trans_neg25).points[i], &(tri_rotated).points[i], &rotate_y);
                        }

                        Triangle tri_trans_pos25 = {0};

                        // Translate the points back to their original position
                        for (int i = 0; i < 3; i++)
                        {
                                multiply_matrix_vector(
                                    &(tri_rotated).points[i], &(tri_trans_pos25).points[i], &translate_pos_025);
                        }

                        Triangle tri_trans_pos25_2 = {0};

                        // Translate the points back to their original position
                        for (int i = 0; i < 3; i++)
                        {
                                multiply_matrix_vector(
                                    &(tri_trans_pos25).points[i], &(tri_trans_pos25_2).points[i], &translate_neg_025);
                        }

                        // Store the result in tri_trans
                        Triangle tri_trans = tri_trans_pos25_2;

                        translate_add(&tri_trans, &trans_x, &trans_y, &trans_z);

                        // Scaling the x axis by 2 because of the characters
                        //
                        // height:width ratio in the terminal.
                        terminal_character_correction(&tri_trans);

                        Triangle tri_projd = {0};

                        for (int i = 0; i < 3; i++)
                        {
                                multiply_matrix_vector(&(tri_trans).points[i], &(tri_projd).points[i], &proj);
                        }

                        triangle_rebase_and_draw(&tri_projd);
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
