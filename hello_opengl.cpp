#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h> // För att kunna räkna ut sinus och cosinus
#include <cuda_runtime.h>

// Vi definierar float2 här så C++ förstår samma format som CUDA
// Vi definierar hur många partiklar vi vill ha totalt
const int num_particles = 1000;



// Detta är "efterlysningen". Vi säger att denna funktion 
// finns i en annan fil (vår .o-fil)
extern void launch_update_swarm(float2* pos, float2* vel, int n);


int main() {
    // 1. Initiera GLFW (skapar fönsterhanteraren)
    if (!glfwInit()) return -1;

    // 2. Skapa ett fönster (800x600 pixlar)
    GLFWwindow* window = glfwCreateWindow(800, 600, "Jetson Space Blue", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    // 3. Gör fönstrets innehåll till det vi ritar på
    glfwMakeContextCurrent(window);
    glewInit(); // Initiera GLEW efter att vi har en context



	float2 h_pos[num_particles]; // Positioner på CPU
	float2 h_vel[num_particles]; // Hastigheter på CPU

	for(int i = 0; i < num_particles; i++) {
		h_pos[i] = {0.0f, 0.0f}; // Alla börjar i mitten
		h_vel[i] = {((rand()%100)/5000.0f)-0.01f, ((rand()%100)/5000.0f)-0.01f}; // Slumpmässig fart
	}

	// Skapa minne på GPU:n
	float2 *d_pos, *d_vel;
	cudaMalloc(&d_pos, num_particles * sizeof(float2));
	cudaMalloc(&d_vel, num_particles * sizeof(float2));

	// Kopiera startvärden till GPU
	cudaMemcpy(d_pos, h_pos, num_particles * sizeof(float2), cudaMemcpyHostToDevice);
	cudaMemcpy(d_vel, h_vel, num_particles * sizeof(float2), cudaMemcpyHostToDevice);


    // 4. Den stora loopen (Här händer grafiken!)
    while (!glfwWindowShouldClose(window)) {
        // Ställ in färgen (Space Blue: R=0.05, G=0.05, B=0.2)
        glClearColor(0.05f, 0.05f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Här kommer vi senare lägga in din CUDA-swarm!


		
		// 1. Uppdatera alla 1000 partiklar samtidigt med CUDA!
		launch_update_swarm(d_pos, d_vel, num_particles);

		// 2. Hämta tillbaka resultatet för att rita (vi optimerar detta till "Zero Copy" senare!)
		cudaMemcpy(h_pos, d_pos, num_particles * sizeof(float2), cudaMemcpyDeviceToHost);

		// 3. Rita svärmen
		glPointSize(2.0f);
		glBegin(GL_POINTS);
		for(int i = 0; i < num_particles; i++) {
			glVertex2f(h_pos[i].x, h_pos[i].y);
		}
		glEnd();
		
		




        // Byt bildruta och kolla efter knapptryck
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
