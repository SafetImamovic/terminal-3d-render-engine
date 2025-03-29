#include "../../include/engine/mesh.h"
#include <math.h>
#include <stdio.h>

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

Triangle cube[] = {
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

void rotation_matrix_x_init(Mat4x4 *m, float fAlpha)
{
        if (!m)
        {
                return;
        }

        m->matrix[0][0] = 1.0f;
        m->matrix[1][1] = cosf(fAlpha * 0.5f);
        m->matrix[1][2] = sinf(fAlpha * 0.5f);
        m->matrix[2][1] = -sinf(fAlpha * 0.5f);
        m->matrix[2][2] = cosf(fAlpha * 0.5f);
        m->matrix[3][3] = 1.0f;
}

void rotation_matrix_y_init(Mat4x4 *m, float fAlpha)
{
        if (!m)
        {
                return;
        }

        m->matrix[0][0] = cosf(fAlpha * 0.25f);
        m->matrix[0][2] = sinf(fAlpha * 0.25f);
        m->matrix[1][1] = 1.0f;
        m->matrix[2][0] = -sinf(fAlpha * 0.25f);
        m->matrix[2][2] = cosf(fAlpha * 0.25f);
        m->matrix[3][3] = 1.0f;
}

void rotation_matrix_z_init(Mat4x4 *m, float fAlpha)
{
        if (!m)
        {
                return;
        }

        m->matrix[0][0] = cosf(fAlpha);
        m->matrix[0][1] = sinf(fAlpha);
        m->matrix[1][0] = -sinf(fAlpha);
        m->matrix[1][1] = cosf(fAlpha);
        m->matrix[2][2] = 1.0f;
        m->matrix[3][3] = 1.0f;
}

/**
 * |  x  y  z  1  | * |     1        0        0     0  |
 *                    |                                |
 *                    |     0        1        0     0  |
 *                    |                                |
 *                    |     0        0        1     0  |
 *                    |                                |
 *                    |  trans_x  trans_y  trans_z  1  |
 */
void translation_matrix_init(Mat4x4 *m, float offset)
{
        m->matrix[0][0] = 1.0f;
        m->matrix[1][1] = 1.0f;
        m->matrix[2][2] = 1.0f;
        m->matrix[3][3] = 1.0f;

        m->matrix[3][0] = offset;
        m->matrix[3][1] = offset;
        m->matrix[3][2] = offset;
}

void multiply_matrix_vector(Vec3D *i, Vec3D *o, Mat4x4 *m)
{
        if (!i || !o || !m)
        {
                printf("Error: NULL pointer passed to multiply_matrix_vector.\n");
                return;
        }

        o->x    = i->x * m->matrix[0][0] + i->y * m->matrix[1][0] + i->z * m->matrix[2][0] + 1.0f * m->matrix[3][0];
        o->y    = i->x * m->matrix[0][1] + i->y * m->matrix[1][1] + i->z * m->matrix[2][1] + 1.0f * m->matrix[3][1];
        o->z    = i->x * m->matrix[0][2] + i->y * m->matrix[1][2] + i->z * m->matrix[2][2] + 1.0f * m->matrix[3][2];

        float w = i->x * m->matrix[0][3] + i->y * m->matrix[1][3] + i->z * m->matrix[2][3] + 1.0f * m->matrix[3][3];

        if (w != 0.0f)
        {
                o->x /= w;
                o->y /= w;
                o->z /= w;
        }
}

void translate_add(Triangle *t, float *x, float *y, float *z)
{
        t->points[0].z += *z;
        t->points[1].z += *z;
        t->points[2].z += *z;

        t->points[0].x += *x;
        t->points[1].x += *x;
        t->points[2].x += *x;

        t->points[0].y += *y;
        t->points[1].y += *y;
        t->points[2].y += *y;
}

void terminal_character_correction(Triangle *t)
{
        t->points[0].x *= 2;
        t->points[1].x *= 2;
        t->points[2].x *= 2;
}

void triangle_rebase_and_draw(Triangle *t)
{
        float x0 = t->points[0].x;
        float y0 = t->points[0].y;
        float x1 = t->points[1].x;
        float y1 = t->points[1].y;
        float x2 = t->points[2].x;
        float y2 = t->points[2].y;

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
