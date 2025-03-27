#include "../globals.h"
#include "../math/utils.h"

void mesh_init(Mesh *m, size_t initial_capacity);

void mesh_add(Mesh *m, Triangle t);

void mesh_free(Mesh *m);

void mesh_info(const Mesh *m);

void projection_matrix_init(Mat4x4 m);

void multiply_matrix_vector(Vec3D *i, Vec3D *o, Mat4x4 *proj);

void draw_triangle(Triangle *t);
