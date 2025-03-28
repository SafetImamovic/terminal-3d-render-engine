#include "../../include/engine/mesh.h"
#include <math.h>
#include <stdio.h>

/**
 * Function that initializes the mesh.
 *
 * Allocates enough memory for the mesh and sets the `size` and `capacity`.
 *
 * `initial_capacity` is the total number of triangles.
 *
 */
void mesh_init(Mesh *m, size_t initial_capacity)
{
        m->size     = 0;
        m->capacity = initial_capacity;
        m->tris     = (Triangle *)malloc(initial_capacity * sizeof(Triangle)); // FIX: Multiply by sizeof(Triangle)
        if (!m->tris)
        {
                printf("Memory allocation failed!\n");
                exit(1);
        }
}

void mesh_add(Mesh *m, Triangle t)
{
        // Check if the capacity is reached
        if (m->size == m->capacity)
        {
                // Double the capacity using realloc
                m->capacity *= 2;

                m->tris = (Triangle *)realloc(m->tris, m->capacity * sizeof(Triangle));

                if (!m->tris) // Check if realloc failed
                {
                        fprintf(stderr, "Memory allocation failed!\n");

                        exit(1);
                }
        }

        // Add the triangle
        m->tris[m->size] = t;

        m->size++;
}

void mesh_free(Mesh *m)
{
        free(m->tris);

        m->tris     = NULL; // Set pointer to NULL after freeing to avoid dangling pointer

        m->size     = 0;
        m->capacity = 0;
}

void mesh_info(const Mesh *m)
{
        if (m == NULL)
        {
                printf("Invalid Mesh reference!\n");
                return;
        }

        printf("Mesh Information:\n");
        printf("  Number of triangles: %zu\n", m->size);
        printf("  Capacity: %zu triangles\n", m->capacity);
        printf("  Byte size: %zu bytes\n", sizeof(*m->tris));
}

/**
 *	Perspective Projection Matrix:
 *
 * 	[ x  y  z  1 ] * | (h/w) * (1/tan(a/2))      0                     0                 0  |
 *                       |                                                                      |
 *                       |           0           1/tan(a/2)                0                 0  |
 *                       |                                                                      |
 *                       |           0               0           Zfar / (Zfar - Znear)       1  |
 *                       |                                                                      |
 *                       |           0               0       -Zfar * Znear / (Zfar - Znear)  0  |
 *
 *
 */
void projection_matrix_init(Mat4x4 *m)
{
        float fNear        = 0.5f;
        float fFar         = 5.0f;
        float fFov         = 90.0f;
        float fAspectRatio = (float)SCREEN_WIDTH / SCREEN_HEIGHT;
        float fFovRad      = 1.0f / tanf(fFov * 0.5f * (PI / 180.0f));

        // Fill in the projection matrix
        m->matrix[0][0] = fFovRad / fAspectRatio;
        m->matrix[0][1] = 0.0f;
        m->matrix[0][2] = 0.0f;
        m->matrix[0][3] = 0.0f;

        m->matrix[1][0] = 0.0f;
        m->matrix[1][1] = fFovRad;
        m->matrix[1][2] = 0.0f;
        m->matrix[1][3] = 0.0f;

        m->matrix[2][0] = 0.0f;
        m->matrix[2][1] = 0.0f;
        m->matrix[2][2] = fFar / (fFar - fNear);
        m->matrix[2][3] = -fFar * fNear / (fFar - fNear);

        m->matrix[3][0] = 0.0f;
        m->matrix[3][1] = 0.0f;
        m->matrix[3][2] = 1.0f;
        m->matrix[3][3] = 0.0f;
}

void orthogonal_projection_matrix_init(Mat4x4 *m)
{

        float fAspectRatio = (float)SCREEN_WIDTH / SCREEN_HEIGHT;
        float left         = 0.0f;
        float right        = 1.0f * fAspectRatio;
        float bottom       = 0.0f;
        float top          = 1.0f;
        float fNear        = 0.01f;
        float fFar         = 5.0f;

        float fX           = 2.0f / (right - left);
        float fY           = 2.0f / (top - bottom);
        float fF           = 2.0f / (fFar - fNear);

        float _fX          = -(right + left) / (right - left);
        float _fY          = -(top + bottom) / (top - bottom);
        float _fF          = -(fFar + fNear) / (fFar - fNear);

        m->matrix[0][0]    = fX;
        m->matrix[0][1]    = 0.0f;
        m->matrix[0][2]    = 0.0f;
        m->matrix[0][3]    = 0.0f;

        m->matrix[1][0]    = 0.0f;
        m->matrix[1][1]    = fY;
        m->matrix[1][2]    = 0.0f;
        m->matrix[1][3]    = 0.0f;

        m->matrix[2][0]    = 0.0f;
        m->matrix[2][1]    = 0.0f;
        m->matrix[2][2]    = fF;
        m->matrix[2][3]    = 0.0f;

        m->matrix[3][0]    = _fX;
        m->matrix[3][1]    = _fY;
        m->matrix[3][2]    = _fF;
        m->matrix[3][3]    = 1.0f;
}

void multiply_matrix_vector(Vec3D *i, Vec3D *o, Mat4x4 *m)
{
        if (!i || !o || !m)
        {
                printf("Error: NULL pointer passed to multiply_matrix_vector.\n");

                return;
        }

        o->x    = i->x * m->matrix[0][0] + i->y * m->matrix[1][0] + i->z * m->matrix[2][0] + m->matrix[3][0];

        o->y    = i->x * m->matrix[0][1] + i->y * m->matrix[1][1] + i->z * m->matrix[2][1] + m->matrix[3][1];

        o->z    = i->x * m->matrix[0][2] + i->y * m->matrix[1][2] + i->z * m->matrix[2][2] + m->matrix[3][2];

        float w = i->x * m->matrix[0][3] + i->y * m->matrix[1][3] + i->z * m->matrix[2][3] + m->matrix[3][3];

        if (w != 0.0f)
        {
                o->x /= w;

                o->y /= w;

                o->z /= w;
        }
}

void draw_triangle(Triangle *t)
{

        if (t == NULL)
        {
                printf("Error: NULL Triangle Pointer!\n");
                return;
        }
}
