#include "../../include/engine/mesh.h"
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
        m->tris     = (Triangle *)malloc(initial_capacity * sizeof(Triangle)); // Allocate memory

        m->size     = 0;

        m->capacity = initial_capacity;
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
        printf("  Byte size: %zu bytes\n", m->size);
}
