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
#include <stdlib.h>

FILE *log_file;

typedef struct
{
        Mat4x4 **transformations;
        size_t   size;
        size_t   capacity;
} Pipeline;

void pipeline_init(Pipeline *p, size_t c)
{
        p->size            = 0;
        p->capacity        = c;

        p->transformations = (Mat4x4 **)malloc(sizeof(Mat4x4 *) * c);

        if (!p->transformations)
        {
                perror("Failed to allocate memory");

                exit(EXIT_FAILURE);
        }

        for (size_t i = 0; i < c; i++)
        {
                p->transformations[i] = NULL;
        }
}

void pipeline_free(Pipeline *p)
{
        if (p->transformations)
        {
                for (size_t i = 0; i < p->size; i++)
                {
                        free(p->transformations[i]);
                }

                free(p->transformations);

                p->transformations = NULL;
        }

        p->size     = 0;
        p->capacity = 0;
}

void pipeline_push(Pipeline *p, Mat4x4 *m)
{
        if (!p->transformations || !m)
        {
                return;
        }

        if (p->size >= p->capacity)
        {
                p->capacity *= 2;

                p->transformations = (Mat4x4 **)realloc(p->transformations, sizeof(Mat4x4 *) * p->capacity);

                if (!p->transformations)
                {
                        perror("Memory reallocation failed");

                        exit(EXIT_FAILURE);
                }
        }

        p->transformations[p->size++] = m;
}

Mat4x4 *pipeline_pop(Pipeline *p)
{
        if (p->size == 0)
        {
                return NULL;
        }

        Mat4x4 *last                    = p->transformations[p->size - 1];

        p->transformations[p->size - 1] = NULL;

        p->size--;

        if (p->size > 0 && p->size <= p->capacity / 4)
        {
                p->capacity /= 2;

                p->transformations = (Mat4x4 **)realloc(p->transformations, sizeof(Mat4x4 *) * p->capacity);

                if (!p->transformations)
                {
                        perror("Memory reallocation failed");

                        exit(EXIT_FAILURE);
                }
        }

        return last;
}

void pipeline_clear(Pipeline *p)
{
        for (int i = 0; i < p->size; i++)
        {
                p->transformations[i] = NULL;

                p->size               = 0;
        }
}

Mat4x4 identity_matrix()
{
        Mat4x4 mat = {0};

        for (int i = 0; i < 4; i++)
        {
                mat.matrix[i][i] = 1.0f;
        }

        return mat;
}

Mat4x4 multiply_matrices(Mat4x4 *a, Mat4x4 *b)
{
        Mat4x4 result = {0}; // Initialize to zero

        for (int row = 0; row < 4; row++)
        {
                for (int col = 0; col < 4; col++)
                {
                        for (int k = 0; k < 4; k++)
                        {
                                result.matrix[row][col] += a->matrix[row][k] * b->matrix[k][col];
                        }
                }
        }

        return result;
}

Triangle pipeline_pass(Pipeline *p, Triangle *in)
{
        Mat4x4 final_transform = identity_matrix(); // Start with identity matrix

        // Accumulate transformations
        for (int i = 0; i < p->size; i++)
        {
                final_transform = multiply_matrices(&final_transform, p->transformations[i]);
        }

        // Apply the final transformation to the input triangle
        Triangle out = {0};

        for (int j = 0; j < 3; j++)
        {
                multiply_matrix_vector(&in->points[j], &out.points[j], &final_transform);
        }

        return out;
}

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

                fAlpha += 0.005f;

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

        pipeline_free(&p);

        Core.Terminal.show_cursor();

        fclose(log_file);

        return 0;
}
