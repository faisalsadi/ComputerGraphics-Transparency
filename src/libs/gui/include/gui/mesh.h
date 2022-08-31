#pragma once

#include <gui/guiMath.h>
#include <gui/shader.h>

// possible loss of data in conversion between double and float
#pragma warning(disable : 4244)
// deprecated/unsafe functions such as fopen
#pragma warning(disable : 4996)

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include <map>
#include <vector>

namespace crl {
namespace gui {

struct Vertex {
    glm::vec3 position = glm::vec3(0, 0, 0);
    glm::vec3 normal = glm::vec3(0, 0, 0);
    glm::vec2 texCoords = glm::vec2(0, 0);

    bool operator==(const Vertex &other) const {
        return position == other.position && normal == other.normal &&
               texCoords == other.texCoords;
    }
};

struct Texture {
    unsigned int id;
    std::string path;
};

struct Stohistic {
    float alpha;
    float z_value;
};




class Mesh {
public:
    enum TextureType { DIFFUSE, SPECULAR, NORMAL, AMBIENT };
    typedef std::map<TextureType, std::vector<Texture>> TextureMap;

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    TextureMap textures;
    // the color of the mesh
    V3D defaultColor = V3D(0.9, 0.9, 0.9);
    //dual depth peeling FBO id
    GLuint dualDepthFBOID;
    //back texture colour attachment IDs
    GLuint backTexID[2];
    //front texture colour attachment IDs
    GLuint texID[2];
    //back texture depth attachment IDs
    GLuint depthTexID[2];
    //colour blending FBO ID
    GLuint colorBlenderFBOID;
    //colour blend FBO colour attachment texture ID
    GLuint colorBlenderTexID;



private:
    unsigned int VAO, VBO, EBO;
    unsigned int A_BUF;

public:
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
         std::map<TextureType, std::vector<Texture>> textures);

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);

    /**
     * render the mesh using shader and specified color
     */
    void draw(Shader shader, const V3D &color, float alpha = 1.0)const ;


    void draw_with_depthpeeling(Shader shader, const V3D& color, float alpha);
    /**
     * render the mesh using shader and default color
     */
    void draw(Shader shader, float alpha = 1.0) const;

    void storedepth(Shader shader, const V3D& color, float alpha);

    void CalcVis(Stohistic BufferD);

    void reinitialize(std::vector<Vertex> vertices,
                      std::vector<unsigned int> indices);
    void sort_tr(glm::vec3 p);
    float calc_dist(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);

private:
    // initializes all the buffer objects/arrays
    void setupMesh();
};

}  // namespace gui
}  // namespace crl