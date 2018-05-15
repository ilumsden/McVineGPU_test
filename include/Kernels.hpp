__device__ void calculate_time(float* ts, float x, float y, float z,
                               float va, float vb, float vc,
                               const float A, const float B, const int offset);

__global__ void intersectRectangle(float* rx, float* ry, float* rz,
                                   float* vx, float* vy, float* vz,
                                   const float X, const float Y, const float Z,
                                   const int N, float* ts);