/*===============================================================================================*
 *
 * The MAIN file. Render Engine entrypoint.
 *
 *===============================================================================================*/

#ifdef _WIN32
#include <conio.h>
#endif

#include "../include/globals.h"

#include "../include/engine/mesh.h"
#include "../include/core/terminal.h"
#include "../include/math/utils.h"
#include "../include/engine/pipeline.h"

#include <stdio.h>

FILE *log_file;

int main()
{
        Pipeline p;
        p.size = 0;

        pipeline_init(&p, 2);

        Mesh m, m_tetrahedron;

        const int num_tris             = sizeof(cube) / sizeof(Triangle);

        const int num_tris_tetrahedron = sizeof(tetrahedra) / sizeof(Triangle);

        mesh_init(&m, num_tris);

        mesh_init(&m_tetrahedron, num_tris_tetrahedron);

        // Bulk add triangles
        for (int i = 0; i < num_tris; i++)
        {
                mesh_add(&m, cube[i]);
        }

        for (int i = 0; i < num_tris_tetrahedron; i++)
        {
                mesh_add(&m_tetrahedron, tetrahedra[i]);
        }

        mesh_info(&m);

        log_file = fopen("logs.txt", "a");

        Core.Terminal.hide_cursor();

        Core.Terminal.init_console();

        Core.Utils.init_measurement();

        Mat4x4 proj;

        projection_matrix_init(&proj);

        Triangle tri_init = {0};

        Mat4x4 rotate_z = {0}, rotate_x = {0}, rotate_y = {0}, translate = {0}, translate_neg_3 = {0},
               translate_ez      = {0};

        float fAlpha             = 0.0f;

        Mat4x4 translate_neg_025 = {0};

        translation_matrix_init(&translate_neg_025, -0.5f, 0.0f, -0.289f);

        translation_matrix_init(&translate_neg_3, 0.0f, 0.0f, 4.0f);

        Mat4x4 translate_pos_025 = {0};

        translation_matrix_init(&translate_pos_025, +0.5f, 0.0f, +0.289f);

        while (1)
        {
                Core.Utils.measure_start();

                Core.Terminal.clear_buffer();

                Core.Utils.draw_coordinate_system();

                if (FPS_LIMIT != 0)
                        fAlpha += 4.0f / (float)FPS_LIMIT;
                else
                        fAlpha += 0.001f;

                rotation_matrix_x_init(&rotate_x, fAlpha);

                rotation_matrix_y_init(&rotate_y, fAlpha);

                for (int i = 0; i < (int)m_tetrahedron.size; i++)
                {

                        Triangle t;
                        t = (m_tetrahedron).tris[i];

                        pipeline_clear(&p);

                        pipeline_push(&p, &translate_neg_025);

                        pipeline_push(&p, &rotate_y);

                        pipeline_push(&p, &rotate_x);

                        pipeline_push(&p, &translate_pos_025);

                        pipeline_push(&p, &translate_neg_3);

                        pipeline_push(&p, &translate_neg_025);

                        pipeline_push(&p, &translate_ez);

                        pipeline_push(&p, &proj);

                        Triangle last = {0};

                        last          = pipeline_pass(&p, &t);

                        translation_matrix_init(&translate_ez, trans_x, trans_y, trans_z);

                        terminal_character_correction(&last);

                        triangle_rebase_and_draw(&last);
                }

                Core.Utils.draw_edges('|', '-', '+');

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

                Core.Utils.draw_stats();

                Core.Terminal.render_buffer();

                Core.Utils.measure_end();

                Core.Utils.measure_diff();

                calculate_true_elapsed_time();
        }

        mesh_free(&m);

        mesh_info(&m);

        pipeline_free(&p);

        Core.Terminal.show_cursor();

        fclose(log_file);

        return 0;
}
