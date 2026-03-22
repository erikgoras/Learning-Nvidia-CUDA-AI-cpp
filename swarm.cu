
__global__ void updateSwarm(float2* pos, float2* vel, int n) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i < n) {
        // Uppdatera positionen
        pos[i].x += vel[i].x;
        pos[i].y += vel[i].y;

        // Studsa mot väggarna (så de inte försvinner!)
        if (pos[i].x > 1.0f || pos[i].x < -1.0f) vel[i].x *= -1.0f;
        if (pos[i].y > 1.0f || pos[i].y < -1.0f) vel[i].y *= -1.0f;
    }
}


// Denna funktion blir bryggan som din C++-kod anropar
void launch_update_swarm(float2* pos, float2* vel, int n) {
    int threadsPerBlock = 256;
    int blocksPerGrid = (n + threadsPerBlock - 1) / threadsPerBlock;
    
    updateSwarm<<<blocksPerGrid, threadsPerBlock>>>(pos, vel, n);
    
    // Vänta på att GPU:n ska bli klar
    cudaDeviceSynchronize();
}
