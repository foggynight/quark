/* main.cpp
 * Copyright (C) 2020-2021 Robert Coffey
 * Released under the GPLv2 license */

// TODO: Error handling using glDebugMessageCallback

#include <fstream>
#include <sstream>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <rtb/log.h>

#define SHADER_SOURCE_PATH "../res/shader/basic.shader"

struct ShaderSource {
    std::string vertexSource;
    std::string fragmentSource;
};

static ShaderSource parse_shader(const std::string& filepath);

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
         0.5,  0.5, // Top-right
         0.5, -0.5, // Bottom-right
        -0.5, -0.5, // Bottom-left
        -0.5,  0.5  // Top-left
    };

    unsigned int ind_arr[] = {
        0, 1, 2, // Bottom-right triangle
        2, 3, 0  // Top-left triangle
    };

    unsigned int vertex_bid;
    glGenBuffers(1, &vertex_bid);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_bid);
    glBufferData(GL_ARRAY_BUFFER, sizeof pos_arr, pos_arr, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);

    unsigned int index_bid;
    glGenBuffers(1, &index_bid);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_bid);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof ind_arr, ind_arr, GL_STATIC_DRAW);

    std::string vertex_shader;
    std::string fragment_shader;

    ShaderSource source = parse_shader(SHADER_SOURCE_PATH);
    unsigned int shader = create_shader(source.vertexSource, source.fragmentSource);
    glUseProgram(shader);

    while (!glfwWindowShouldClose(win)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(win);
        glfwPollEvents();
    }

    glDeleteProgram(shader);
    glfwTerminate();
    return 0;
}

static ShaderSource parse_shader(const std::string& filepath)
{
    std::fstream stream(filepath);

    enum class ShaderType {
        NONE = -1,
        VERTEX,
        FRAGMENT
    } type = ShaderType::NONE;

    std::string line;
    std::stringstream ss[2];

    while (std::getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else {
            ss[(int)type] << line << '\n';
        }
    }

    return { ss[0].str(), ss[1].str() };
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
