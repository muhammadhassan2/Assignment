/*
 * GLUT Shapes Demo
 *
 * Written by Nigel Stewart November 2003
 *
 * This program is test harness for the sphere, cone
 * and torus shapes in GLUT.
 *
 * Spinning wireframe and smooth shaded shapes are
 * displayed until the ESC or q key is pressed.  The
 * number of geometry stacks and slices can be adjusted
 * using the + and - keys.
 */
#include <GL/glut.h>
#include <FreeImage.h>
#include <iostream>

// Function to load an image using FreeImage
FIBITMAP* LoadImage(const char* filename) {
    FIBITMAP* image = FreeImage_Load(FIF_ANY, filename, 0);
    return image;
}

// Function to save an image using FreeImage
void SaveImage(FIBITMAP* image, const char* filename) {
    FreeImage_Save(FIF_PNG, image, filename, 0);
}

// Fragment shader to detect edges and color them red
const char* fragmentShaderSource = R"(
    #version 330 core
    uniform sampler2D inputTexture;
    in vec2 TexCoord;
    out vec4 FragColor;
    void main() {
        vec3 color = texture(inputTexture, TexCoord).rgb;
        vec3 edgeColor = vec3(1.0, 0.0, 0.0); // Red color for edges
        float threshold = 0.1; // Adjust this threshold for edge detection sensitivity
        vec3 dx = dFdx(color);
        vec3 dy = dFdy(color);
        float edge = length(dx) + length(dy);
        if (edge > threshold) {
            FragColor = vec4(edgeColor, 1.0);
        } else {
            FragColor = vec4(color, 1.0);
        }
    }
)";

// Display function
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glutSwapBuffers();
}

int main(int argc, char** argv) {
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutCreateWindow("Edge Detection and Coloring");

    // Load the image
    FIBITMAP* image = LoadImage("input.png");
    if (!image) {
        std::cerr << "Failed to load image." << std::endl;
        return 1;
    }

    // Get image dimensions
    int width = FreeImage_GetWidth(image);
    int height = FreeImage_GetHeight(image);

    // Create a OpenGL texture from the image
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, FreeImage_GetBits(image));
    glBindTexture(GL_TEXTURE_2D, 0);

    // Create and compile the fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Create and link the shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    // Set up a full-screen quad for rendering
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex2f(-1.0, -1.0);
    glTexCoord2f(1.0, 0.0); glVertex2f( 1.0, -1.0);
    glTexCoord2f(1.0, 1.0); glVertex2f( 1.0,  1.0);
    glTexCoord2f(0.0, 1.0); glVertex2f(-1.0,  1.0);
    glEnd();

    // Save the result back to an image
    FIBITMAP* resultImage = FreeImage_Allocate(width, height, 24);
    glBindTexture(GL_TEXTURE_2D, texture);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_BGR, GL_UNSIGNED_BYTE, FreeImage_GetBits(resultImage));
    SaveImage(resultImage, "output.png");

    // Cleanup
    FreeImage_Unload(resultImage);
    FreeImage_Unload(image);
    glDeleteTextures(1, &texture);
    glDeleteProgram(shaderProgram);
    glDeleteShader(fragmentShader);

    return 0;
}
