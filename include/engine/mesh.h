#include "../globals.h"
#include "../math/utils.h"

extern Triangle cube[12];

void mesh_init(Mesh *m, size_t initial_capacity);

void mesh_add(Mesh *m, Triangle t);

void mesh_free(Mesh *m);

void mesh_info(const Mesh *m);

void projection_matrix_init(Mat4x4 *m);

void multiply_matrix_vector(Vec3D *i, Vec3D *o, Mat4x4 *proj);

void orthogonal_projection_matrix_init(Mat4x4 *m);

void rotation_matrix_x_init(Mat4x4 *m, float fAlpha);

void rotation_matrix_y_init(Mat4x4 *m, float fAlpha);

void rotation_matrix_z_init(Mat4x4 *m, float fAlpha);

void translation_matrix_init(Mat4x4 *m, float offset);

void translate_add(Triangle *t, float *x, float *y, float *z);

void terminal_character_correction(Triangle *t);

void triangle_rebase_and_draw(Triangle *t);
