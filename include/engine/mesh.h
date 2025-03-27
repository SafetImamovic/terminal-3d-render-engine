#include "../globals.h"

void mesh_init(Mesh *m, size_t initial_capacity);

void mesh_add(Mesh *m, Triangle t);

void mesh_free(Mesh *m);

void mesh_info(const Mesh *m);
