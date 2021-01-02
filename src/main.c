/* main.c - v0.0.0
 * Copyright (C) 2020-2021 Robert Coffey
 * Released under the GPLv2 license */

#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <rtb_log.h>

int main(void)
{
    GLFWwindow *win;

    if (!glfwInit())
        return EXIT_FAILURE;

    win = glfwCreateWindow(640, 480, "QUARK", NULL, NULL);
    if (!win) {
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(win);

    GLenum err = glewInit();
    if (GLEW_OK != err)
        rtb_elogf("Failed to initialize GLEW: %s\n", glewGetErrorString(err));
    rtb_logf("OpenGL version: %s\n", glGetString(GL_VERSION));

    float pos_arr[] = {
        -0.5f, -0.5f,
         0.0f,  0.5f,
         0.5f, -0.5f
    };

    unsigned int buf_id;
    glGenBuffers(1, &buf_id);
    glBindBuffer(GL_ARRAY_BUFFER, buf_id);
    glBufferData(GL_ARRAY_BUFFER, 6*sizeof(float), pos_arr, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), 0);

    while (!glfwWindowShouldClose(win)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(win);
        glfwPollEvents();
    }

    glfwTerminate();
    return EXIT_SUCCESS;
}
