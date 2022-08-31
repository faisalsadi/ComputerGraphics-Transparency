#include <gui/mesh.h>
#include <algorithm>
#include <glm/gtx/quaternion.hpp>
namespace crl {
namespace gui {

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
           std::map<TextureType, std::vector<Texture>> textures)
    : vertices(vertices), indices(indices), textures(textures) {
    //glEnable(GL_BLEND);
    //glDisable(GL_DEPTH_TEST);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glEnable(GL_CULL_FACE);
    //glFrontFace(GL_CCW);
    //glEnable(GL_DEPTH_TEST);
    setupMesh();
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
    : vertices(vertices), indices(indices) {
    //glEnable(GL_BLEND);
    //glDisable(GL_DEPTH_TEST);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glEnable(GL_CULL_FACE);
    //glFrontFace(GL_CCW);
    //glEnable(GL_DEPTH_TEST);
    setupMesh();
}

void Mesh::draw(Shader shader, const V3D &color, float alpha)const  {
    shader.setVec3("objectColor", toGLM(color));
    //shader.setVec3("objectColor", glm::vec3(1,0,0));
    shader.setFloat("alpha", alpha);
    std::map<float, glm::vec3> sorted;
    if (textures.find(DIFFUSE) != textures.end()) {
        shader.setBool("use_textures", true);
        // bind appropriate textures
        for (unsigned int i = 0; i < textures.at(DIFFUSE).size(); i++) {


            glActiveTexture(GL_TEXTURE0 +
                            i);  // active proper texture unit before binding
            // retrieve texture number (the N in diffuse_textureN)

            const Texture &texture = textures.at(DIFFUSE)[i];

            // now set the sampler to the correct texture unit

            glUniform1i(
                glGetUniformLocation(
                    shader.ID,
                    ("texture_diffuse" + std::to_string(i + 1)).c_str()),
                i);
            // and finally bind the texture
            glBindTexture(GL_TEXTURE_2D, texture.id);
        }
    } else {
        // there are no textures... indicate as much...
        shader.setBool("use_textures", false);
    }

    
    // draw mesh
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT,
                   nullptr);
    glBindVertexArray(0);

    // always good practice to set everything back to defaults once configured.
    glActiveTexture(GL_TEXTURE0);
}

void Mesh::draw(Shader shader, float alpha) const {
    draw(shader, this->defaultColor, alpha);
}
float Mesh::calc_dist(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
{
    glm::vec3 face_normal, face_center,u,v,s,res;
    
    u = p2 - p1;
	v = p3 - p1;
    s = p0 - p1;
	face_normal.x = (u.y * v.z) - (u.z * v.y);
	face_normal.y = (u.z * v.x) - (u.x * v.z);
	face_normal.z = (u.x * v.y) - (u.y * v.x);
    face_normal = glm::cross(u, v);
    face_normal = glm::normalize(face_normal);
    float cos = glm::dot(s, face_normal) / (glm::length(s) * glm::length(face_normal));
    float dt = glm::dot(s, face_normal);
    res.x = s.x - face_normal.x * dt;
    res.y = s.y - face_normal.y * dt;
    res.z = s.z - face_normal.z * dt;
    float len = sqrt(res.x * res.x + res.y * res.y + res.z * res.z);
    return len ;
	/*face_center.x = (p1.x + p2.x + p3.x) / 3;
	face_center.y = (p1.y + p2.y + p3.y) / 3;
	face_center.z = (p1.z + p2.z + p3.z) / 3;
	
	face_normal = face_normal + face_center;*/
}
void Mesh::sort_tr(glm::vec3 camera)
{
    std::map<float, std::vector< glm::vec3>> sorted; //map table between distence from camera to face 

    for (  int i = 0;i < indices.size();i+=3)// face cordinates(each 3 is a face)
    {
        int first = i,second=i+1,third=i+2; 

        glm::vec3 center = glm::vec3((vertices[indices[first]].position +
            vertices[indices[second]].position + vertices[indices[third]].position) ); // calc the center of face
        center /= 3;
        float distance = glm::length2(camera - center); //distance between camera position and the center of the face
        //distance = calc_dist(camera, vertices[indices[first]].position, vertices[indices[second]].position, vertices[indices[third]].position);
        sorted[distance].push_back(glm::vec3(indices[first], indices[second], indices[third]));
        //sorted[distance] = glm::vec3( indices[first],indices[second],indices[third]); // add the 3 indices of the face in relation with their distance
    }
     int i = 0;
    for (std::map<float, std::vector< glm::vec3>>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); it++)
    //    for (std::map<float, std::vector< glm::vec3>>::iterator it = sorted.begin(); it != sorted.end(); it++)
    {
        //int first = i, second = i + 1, third = i + 2;
        for (glm::vec3 p : it->second) 
        {
            indices[i] = p[0];
            indices[static_cast<std::vector<unsigned int, std::allocator<unsigned int>>::size_type>(i)+1] = p[1];
            indices[static_cast<std::vector<unsigned int, std::allocator<unsigned int>>::size_type>(i)+2] = p[2];
            i += 3;
        }
    }
    //std::cout << i << std::endl;
    reinitialize(vertices, indices); // opengl buffers 
}
void Mesh::draw_with_depthpeeling(Shader shader, const V3D& color, float alpha) {

    shader.setVec3("current_color", toGLM(color));
    for (int i = 0; i < indices.size(); i++) {
        shader.setVec3("pixelCoord",glm::vec3((vertices[indices[i]].position)));


    }


}



void Mesh::reinitialize(std::vector<Vertex> vertices,
                        std::vector<unsigned int> indices) {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    this->vertices = vertices;
    this->indices = indices;
    //glEnable(GL_BLEND);
    //glDisable(GL_CULL_FACE);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    setupMesh();
    
}


void Mesh::storedepth(Shader shader, const V3D& color, float alpha) {
    int buffersize = indices.size();

    Stohistic* mybuffer = (Stohistic*)malloc(buffersize * sizeof(Stohistic));
    for (int i = 0; i < indices.size(); i += 3) {
        for (int j = 0; j < indices.size(); j++) {

            mybuffer[i / 3].alpha = alpha;
            //mybuffer[i / 3].z = indices[i].position.z;
        }

    }

}
void Mesh::CalcVis(Stohistic BufferD) {



    for (int i = 0; i < indices.size(); i++) {


    }

}

void Mesh::setupMesh() {
    // TODO: should the arrays generated here be destroyed/deleted at some
    // point? If so, care must be taken with copy constructors/operators and
    // all that...

    // create buffers/arrays
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
  
 /*   for (unsigned int i = 0; i < indices.size(); i++)
    {
        float distance = glm::length(camera.Position - indices[i]);
        sorted[distance] = indices[i];
    }*/

    //std::cout << 1;
    glBindVertexArray(VAO);
    // load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // A great thing about structs is that their memory layout is sequential
    // for all its items. The effect is that we can simply pass a pointer to
    // the struct and it translates perfectly to a glm::vec3/2 array which
    // again translates to 3/2 floats which translates to a byte array.
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
                 &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                 &indices[0], GL_STATIC_DRAW);
    
    // set the vertex attribute pointers
    // vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)nullptr);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)offsetof(Vertex, normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)offsetof(Vertex, texCoords));

    glBindVertexArray(0);
}

}  // namespace gui
}  // namespace crl