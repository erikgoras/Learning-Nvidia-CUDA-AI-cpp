#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h> // För att kunna räkna ut sinus och cosinus



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

	float vinkel = 0.0f;

    // 4. Den stora loopen (Här händer grafiken!)
    while (!glfwWindowShouldClose(window)) {
        // Ställ in färgen (Space Blue: R=0.05, G=0.05, B=0.2)
        glClearColor(0.05f, 0.05f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Här kommer vi senare lägga in din CUDA-swarm!

		// Sätt färgen till vit
		glColor3f(1.0f, 1.0f, 1.0f);

		// Rita en punkt i mitten (0,0 är mitten i OpenGL)
		glPointSize(10.0f); // Gör punkten lite större så vi ser den
		glBegin(GL_POINTS);
		
		vinkel += 0.01f; // Öka vinkeln för varje bildruta
		glVertex2f(cos(vinkel) * 0.5f, sin(vinkel) * 0.5f);
		
		
		glEnd();



        // Byt bildruta och kolla efter knapptryck
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
