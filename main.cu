#include <iostream>
#include <cuda_runtime.h>

// Detta är vår GPU-motor. En kernel som körs på grafikkortet.
__global__ void particleUpdateKernel(float* data, int n) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i < n) {
        data[i] += 1.0f; // Vi låter GPU:n öka värdet på varje "partikel"
    }
}


class Moat {
public:
    static float protect(float value) {
        float limit = 50.0f; // Vår "mur"
        return (value > limit) ? limit : value;
    }
};



// Bjarne älskar klasser som kapslar in resurser (RAII)
class ParticleSystem {
private:
    int numParticles;
    float* deviceData;

public:
    ParticleSystem(int n) : numParticles(n) {
        // Vi ber Jetson om minne direkt på GPU:n
        cudaMalloc(&deviceData, n * sizeof(float));
        cudaMemset(deviceData, 0, n * sizeof(float));
    }

    ~ParticleSystem() {
        // Städa upp efter oss - viktigt i C++!
        cudaFree(deviceData);
    }

    void runSimulation() {
        std::cout << "Simulerar " << numParticles << " partiklar på GPU:n..." << std::endl;
        
        // Vi startar beräkningen. 256 trådar per block är standard.
        int threadsPerBlock = 256;
        int blocksPerGrid = (numParticles + threadsPerBlock - 1) / threadsPerBlock;
        
        particleUpdateKernel<<<blocksPerGrid, threadsPerBlock>>>(deviceData, numParticles);
        
        // Vänta på att GPU:n ska bli klar
        cudaDeviceSynchronize();

		float firstParticleResult = 0;
        cudaMemcpy(&firstParticleResult, deviceData, sizeof(float), cudaMemcpyDeviceToHost);
        std::cout << "Värde för partikel 0 efter GPU-körning: " << firstParticleResult << std::endl;


        
        std::cout << "Simulering klar!" << std::endl;

		std::cout << "Skyddat värde (Moat): " << Moat::protect(firstParticleResult) << std::endl;

        
    }
};

int main() {
    // Vi skapar ett system med 1000000 partiklar
    ParticleSystem myUniverse(1000000);
    
    myUniverse.runSimulation();
    
    return 0;
}




