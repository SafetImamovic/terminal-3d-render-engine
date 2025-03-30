#include "../../include/math/utils.h"
#include "../../include/engine/mesh.h"
#include "../globals.h"

typedef struct
{
        Mat4x4 **transformations;
        size_t   size;
        size_t   capacity;
} Pipeline;

void pipeline_init(Pipeline *p, size_t c);

void pipeline_free(Pipeline *p);

void pipeline_push(Pipeline *p, Mat4x4 *m);

Mat4x4 *pipeline_pop(Pipeline *p);

void pipeline_clear(Pipeline *p);

Triangle pipeline_pass(Pipeline *p, Triangle *in);
