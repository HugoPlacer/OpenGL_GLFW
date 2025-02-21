#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/fwd.hpp"
#include <assimp/material.h>
#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <shader_m.h>
#include <camera.h>
#include <model.h>
#include <mesh.h>

#include <rgb-hsl/hsl_rgb.h>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <iostream>
#include <string>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
unsigned int loadTexture(const char *path);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// lighting
bool flashlight = true;
bool lighting = true;

bool wireframe = false;

int main()
{
  // glfw: initialize and configure
  // ------------------------------
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, WINDOW_NAME, NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    //stbi_set_flip_vertically_on_load(true);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------

    Shader litShader((std::string(COMMON_SOURCE_DIR) + "/Advanced-OpenGL/1.Depth-Testing/lit.vs").c_str(),(std::string(COMMON_SOURCE_DIR) + "/Advanced-OpenGL/1.Depth-Testing/lit.fs").c_str());
    Shader unlitShader((std::string(COMMON_SOURCE_DIR) + "/Advanced-OpenGL/1.Depth-Testing/unlit.vs").c_str(),(std::string(COMMON_SOURCE_DIR) + "/Advanced-OpenGL/1.Depth-Testing/unlit.fs").c_str());
    Shader lightCubeShader((std::string(COMMON_SOURCE_DIR) + "/Advanced-OpenGL/1.Depth-Testing/light_cube.vs").c_str(),(std::string(COMMON_SOURCE_DIR) + "/Advanced-OpenGL/1.Depth-Testing/light_cube.fs").c_str());

    glm::vec3 pointLightPositions[] = {
        glm::vec3( 0.7f,  0.2f,  2.0f),
        glm::vec3( 2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3( 0.0f,  0.0f, -3.0f)
}; 

    // first, configure the cube's VAO (and VBO)
    unsigned int VBO;

    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // we only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need (it's already bound, but we do it again for educational purposes)
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    Model backpack("/home/hugo/Documentos/OpenGL_GLFW/assets/Sponza-master/sponza.obj");

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        //std::cout << int(1 / deltaTime )<< "\n";

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // be sure to activate shader when setting uniforms/drawing objects
        litShader.use();
        litShader.setVec3("viewPos", camera.Position);
        litShader.setFloat("material.shininess", 32.0f);
        litShader.setBool("flashlight", flashlight);

        // directional light
        litShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        litShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        litShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        litShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
        // point light 1
        litShader.setVec3("pointLights[0].position", pointLightPositions[0]);
        litShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        litShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        litShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        litShader.setFloat("pointLights[0].constant", 1.0f);
        litShader.setFloat("pointLights[0].linear", 0.09f);
        litShader.setFloat("pointLights[0].quadratic", 0.032f);
        // point light 2
        litShader.setVec3("pointLights[1].position", pointLightPositions[1]);
        litShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
        litShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
        litShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
        litShader.setFloat("pointLights[1].constant", 1.0f);
        litShader.setFloat("pointLights[1].linear", 0.09f);
        litShader.setFloat("pointLights[1].quadratic", 0.032f);
        // point light 3
        litShader.setVec3("pointLights[2].position", pointLightPositions[2]);
        litShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
        litShader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
        litShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
        litShader.setFloat("pointLights[2].constant", 1.0f);
        litShader.setFloat("pointLights[2].linear", 0.09f);
        litShader.setFloat("pointLights[2].quadratic", 0.032f);
        // point light 4
        litShader.setVec3("pointLights[3].position", pointLightPositions[3]);
        litShader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
        litShader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
        litShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
        litShader.setFloat("pointLights[3].constant", 1.0f);
        litShader.setFloat("pointLights[3].linear", 0.09f);
        litShader.setFloat("pointLights[3].quadratic", 0.032f);
        // spotLight
        litShader.setVec3("spotLight.position", camera.Position);
        litShader.setVec3("spotLight.direction", camera.Front);
        litShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        litShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        litShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        litShader.setFloat("spotLight.constant", 1.0f);
        litShader.setFloat("spotLight.linear", 0.09f);
        litShader.setFloat("spotLight.quadratic", 0.032f);
        litShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        litShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        litShader.setMat4("projection", projection);
        litShader.setMat4("view", view);

        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));	// it's a bit too big for our scene, so scale it down
        litShader.setMat4("model", model);

        unlitShader.use();
        unlitShader.setMat4("projection", projection);
        unlitShader.setMat4("view", view);
        unlitShader.setMat4("model", model);

        lighting ? litShader.use() : unlitShader.use();

        backpack.Draw(litShader);

        // also draw the lamp object
        lightCubeShader.use();
        lightCubeShader.setMat4("projection", projection);
        lightCubeShader.setMat4("view", view);
  
        glBindVertexArray(lightVAO);
        for (unsigned int i = 0; i < 4; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
            lightCubeShader.setMat4("model", model);
            lightCubeShader.setVec3("lightColor", glm::vec3(1.0f));
            //glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);

    static bool f1KeyPressed = false;

    if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS) {
        if (!f1KeyPressed) {
            lighting = !lighting;
            f1KeyPressed = true;
        }
    } else if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_RELEASE) {
        f1KeyPressed = false;
    }

    static bool FKeyPressed = false;

    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
        if (!FKeyPressed) {
            flashlight = !flashlight;
            FKeyPressed = true;
        }
    } else if (glfwGetKey(window, GLFW_KEY_F) == GLFW_RELEASE) {
        FKeyPressed = false;
    }

    static bool f2KeyPressed = false;

    if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS) {
        if (!f2KeyPressed) {
            wireframe = !wireframe;
            wireframe ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            f2KeyPressed = true;
        }
    } else if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_RELEASE) {
        f2KeyPressed = false;
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const * path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    
    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

