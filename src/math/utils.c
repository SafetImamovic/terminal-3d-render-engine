#include "../../include/math/utils.h"
#include "../../include/globals.h"

void bla(int x0, int y0, int x1, int y1)
{
        int dx    = abs(x1 - x0);

        int sx    = (x0 < x1) ? 1 : -1;

        int dy    = -abs(y1 - y0);

        int sy    = (y0 < y1) ? 1 : -1;

        int error = dx + dy;

        while (1)
        {
                if (x0 < 0 || x0 >= SCREEN_WIDTH || y0 < 0 || y0 >= SCREEN_HEIGHT)
                {
                        return;
                }

                buffer[y0][x0] = '$';

                int e2         = 2 * error;

                if (e2 >= dy)
                {
                        if (x0 == x1)
                                break;

                        error = error + dy;

                        x0    = x0 + sx;
                }

                if (e2 <= dx)
                {
                        if (y0 == y1)
                                break;

                        error = error + dx;

                        y0    = y0 + sy;
                }
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
