#include "../../include/engine/pipeline.h"
#include <stdio.h>

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
        }

        p->size = 0;
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
