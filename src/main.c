/* --- quark ---
 * Copyright (C) 2020-2021 Robert Coffey
 * Released under the GPLv2 license */

#include <stdlib.h>

#include <GLFW/glfw3.h>

int main(void)
{
    GLFWwindow *win;

    if (!glfwInit())
        return EXIT_FAILURE;

    win = glfwCreateWindow(640, 480, "Hello, World.", NULL, NULL);
    if (!win) {
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(win);

    while (!glfwWindowShouldClose(win)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glBegin(GL_TRIANGLES);
        glVertex2f(-0.5f, -0.5f);
        glVertex2f( 0.0f,  0.5f);
        glVertex2f( 0.5f, -0.5f);
        glEnd();

        glfwSwapBuffers(win);
        glfwPollEvents();
    }

    glfwTerminate();
    return EXIT_SUCCESS;
}
