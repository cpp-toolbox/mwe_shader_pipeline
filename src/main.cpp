#include <glad/glad.h>

#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include <cstdlib>

#include "window/window.hpp"
#include "shader_pipeline/shader_pipeline.hpp"

struct OpenGLDrawingData {
    GLuint vbo_name;
    GLuint ibo_name;
    GLuint vao_name;
};

OpenGLDrawingData prepare_opengl_drawing_data() {
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
            0.5f, 0.5f, 0.0f, // top right
            0.5f, -0.5f, 0.0f, // bottom right
            -0.5f, -0.5f, 0.0f, // bottom left
            -0.5f, 0.5f, 0.0f  // top left
    };
    GLuint indices[] = {
            // note that we start from 0!
            0, 1, 3, // first Triangle
            1, 2, 3  // second Triangle
    };

    // vbo: vertex buffer object
    // vao: vertex array object
    // ibo: index buffer object

    GLuint vbo_name, vao_name, ibo_name;

    glGenVertexArrays(1, &vao_name);
    glGenBuffers(1, &vbo_name);
    glGenBuffers(1, &ibo_name);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and
    // then configure vertex attributes(s).
    glBindVertexArray(vao_name);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_name);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_name);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered
    // vbo_name as the vertex attribute's bound vertex buffer object so afterwards
    // we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // remember: do NOT unbind the ibo_name while a vao_name is active as the
    // bound element buffer object IS stored in the vao_name; keep the ibo_name
    // bound.
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the vao_name afterwards so other vao_name calls won't
    // accidentally modify this vao_name, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't
    // unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    return {vbo_name, ibo_name, vao_name};
}

unsigned int SCREEN_WIDTH = 640;
unsigned int SCREEN_HEIGHT = 480;

int main() {
    LiveInputState live_input_state;
    GLFWwindow *window = initialize_glfw_glad_and_return_window(&SCREEN_WIDTH, &SCREEN_HEIGHT, "shader pipeline", false,
                                                                false, false, &live_input_state);

    // NOTE: OpenGL error checks have been omitted for brevity

    auto [vbo_name, ibo_name, vao_name] = prepare_opengl_drawing_data();

    ShaderPipeline shader_pipeline{};

    shader_pipeline.load_in_shaders_from_file("../../src/shaders/simple.vert", "../../src/shaders/simple.frag");

    int width, height;

    while (!glfwWindowShouldClose(window)) {

        glfwGetFramebufferSize(window, &width, &height);

        glViewport(0, 0, width, height);

        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        glUseProgram(shader_pipeline.shader_program_id);
        glBindVertexArray(vao_name); // seeing as we only have a single VAO there's
        // no need to bind it every time, but we'll do
        // so to keep things a bit more organized
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &vao_name);
    glDeleteBuffers(1, &vbo_name);
    glDeleteBuffers(1, &ibo_name);
    glDeleteProgram(shader_pipeline.shader_program_id);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}