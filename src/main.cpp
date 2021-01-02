/* main.cpp - v0.0.0
 * Copyright (C) 2020-2021 Robert Coffey
 * Released under the GPLv2 license */

#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <rtb_log.h>

static unsigned int compile_shader(
        unsigned int type,
        const std::string& source);

static unsigned int create_shader(
        const std::string& vertex_shader,
        const std::string& fragment_shader);

int main()
{
    GLFWwindow *win;

    if (!glfwInit())
        rtb_elog("Failed to initialize GLFW");

    win = glfwCreateWindow(640, 480, "QUARK", nullptr, nullptr);
    if (!win) {
        glfwTerminate();
        rtb_elog("Failed to create window");
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
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), nullptr);

    std::string vertex_shader =
            "#version 330 core\n"
            "layout(location = 0) in vec4 position;\n"
            "void main() {\n"
            "  gl_Position = position;\n"
            "}\n";

    std::string fragment_shader =
            "#version 330 core\n"
            "layout(location = 0) out vec4 color;\n"
            "void main() {\n"
            "  color = vec4(1.0, 0.0, 0.0, 1.0);\n"
            "}\n";

    unsigned int shader = create_shader(vertex_shader, fragment_shader);
    glUseProgram(shader);

    while (!glfwWindowShouldClose(win)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(win);
        glfwPollEvents();
    }

    glDeleteProgram(shader);
    glfwTerminate();
    return 0;
}

static unsigned int compile_shader(
        unsigned int type,
        const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char *src = source.c_str();

    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if (!result) {
        const char *type_str =
                (type == GL_VERTEX_SHADER) ? "vertex"
                : (type == GL_FRAGMENT_SHADER) ? "fragment"
                : "";

        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char *message = (char *)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);

        rtb_logf("Error: Failed to compile %s shader\n%s\n", type_str, message);
        glDeleteShader(id);
        return 0;
    }

    return id;
}

static unsigned int create_shader(
        const std::string& vertex_shader,
        const std::string& fragment_shader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = compile_shader(GL_VERTEX_SHADER, vertex_shader);
    unsigned int fs = compile_shader(GL_FRAGMENT_SHADER, fragment_shader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}
