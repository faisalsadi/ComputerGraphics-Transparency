#include <gui\application.h>
#include <gui/renderer.h>
#include <gui/camera.h>
#include <gui/model.h>
#include <gui/shader.h>
#include <gui/shadow_casting_light.h>
#include <gui/shadow_map_fbo.h>
#include <imgui_widgets/ImGuizmo.h>
#include <imgui_widgets/imGuIZMOquat.h>
#include <glm/glm.hpp>


enum BufferNames {
    COUNTER_BUFFER = 0,
    LINKED_LIST_BUFFER
};

struct ListNode {
    glm::vec4 color;
    GLfloat depth;
    GLuint next;
};

namespace crl {
namespace gui {


class Fancy3DApp : public Application {
public:
    Fancy3DApp(const char* title = "Shadows demo",
        std::string iconPath = DATA_FOLDER
        "/crl_icon_grey.png")
        : Application(title, iconPath) {
        camera.aspectRatio = float(width) / height;

        if (!shadowMapFBO.Init(this->width, this->height)) {
            std::cout << "Shadow map initialization failed\n";
            exit(0);
        }

        glEnable(GL_DEPTH_TEST);


        //generate 2 FBO
        glGenFramebuffers(2, fbo);
        //The FBO has two colour attachments
        glGenTextures(2, texID);
        //The FBO has two depth attachments
        glGenTextures(2, depthTexID);

        //glCreateFramebuffers( 1, NULL);

        //for each attachment
        for (int i = 0; i < 2; i++) {
            //first initialize the depth texture
            glBindTexture(GL_TEXTURE_RECTANGLE, depthTexID[i]);
            glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, 0x2900);
            glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, 0x2900);
            glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_DEPTH_COMPONENT32F, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

            //second initialize the colour attachment
            glBindTexture(GL_TEXTURE_RECTANGLE, texID[i]);
            glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, 0x2900);
            glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, 0x2900);
            glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, NULL);

            //bind FBO and attach the depth and colour attachments
            glBindFramebuffer(GL_FRAMEBUFFER, fbo[i]);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_RECTANGLE, depthTexID[i], 0);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_RECTANGLE, texID[i], 0);
        }

        //Now setup the colour attachment for colour blend FBO
        glGenTextures(1, &colorBlenderTexID);
        glBindTexture(GL_TEXTURE_RECTANGLE, colorBlenderTexID);
        glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, 0x2900);
        glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, 0x2900);
        glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, 0);

        //generate the colour blend FBO ID
        glGenFramebuffers(1, &colorBlenderFBOID);
        glBindFramebuffer(GL_FRAMEBUFFER, colorBlenderFBOID);

        //set the depth attachment of previous FBO as depth attachment for this FBO
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_RECTANGLE, depthTexID[0], 0);
        //set the colour blender texture as the FBO colour attachment 
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_RECTANGLE, colorBlenderTexID, 0);

        //check the FBO completeness status
        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (status == GL_FRAMEBUFFER_COMPLETE)
            printf("FBO setup successful !!! \n");
        else
            printf("Problem with FBO setup");

        //unbind FBO
        glBindFramebuffer(GL_FRAMEBUFFER, 0);


       

        //// FOR DUAL !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\








//-----------------------------------------------------------------]>>>

        ////generate dual depth FBO
        //glGenFramebuffers(1, &dualDepthFBOID);
        ////The FBO has 4 colour attachments
        //glGenTextures(2, texIDdual);
        //glGenTextures(2, backTexID);
        ////The FBO has 2 depth attachments
        //glGenTextures(2, depthTexIDdual);

        ////for each attachment
        //for (int i = 0; i < 2; i++) {
        //    //first initialize the depth texture
        //    glBindTexture(GL_TEXTURE_RECTANGLE, depthTexIDdual[i]);
        //    glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        //    glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        //    glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, 0x2900);
        //    glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, 0x2900);
        //    glTexImage2D(GL_TEXTURE_RECTANGLE, 0, 34951, width, height, 0, GL_RGB, GL_FLOAT, NULL);

        //    //initialize the colour attachment
        //    glBindTexture(GL_TEXTURE_RECTANGLE, texIDdual[i]);
        //    glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        //    glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        //    glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, 0x2900);
        //    glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, 0x2900);
        //    glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, NULL);

        //    //initialize the back colour attachment
        //    glBindTexture(GL_TEXTURE_RECTANGLE, backTexID[i]);
        //    glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        //    glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        //    glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, 0x2900);
        //    glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, 0x2900);
        //    glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
        //}

        ////GL_CHECK_ERRORS

        //    //Now setup the colour attachment for colour blend FBO
        //glGenTextures(1, &colorBlenderTexIDdual);
        //glBindTexture(GL_TEXTURE_RECTANGLE, colorBlenderTexIDdual);
        //glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, 0x2900);
        //glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, 0x2900);
        //glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        //glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        //glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT, 0);

        ////generate the colour blend FBO ID
        //glGenFramebuffers(1, &colorBlenderFBOIDdual);
        //glBindFramebuffer(GL_FRAMEBUFFER, colorBlenderFBOIDdual);
        ////set the colour blender texture as the FBO colour attachment 
        //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_RECTANGLE, colorBlenderTexIDdual, 0);

        ////bind the dual depth FBO
        //glBindFramebuffer(GL_FRAMEBUFFER, dualDepthFBOID);

        ////bind the six colour attachments for this FBO
        //for (int i = 0; i < 2; i++) {
        //    glFramebufferTexture2D(GL_FRAMEBUFFER, attachID[i], GL_TEXTURE_RECTANGLE, depthTexIDdual[i], 0);
        //    glFramebufferTexture2D(GL_FRAMEBUFFER, attachID[i] + 1, GL_TEXTURE_RECTANGLE, texIDdual[i], 0);
        //    glFramebufferTexture2D(GL_FRAMEBUFFER, attachID[i] + 2, GL_TEXTURE_RECTANGLE, backTexID[i], 0);
        //}

        ////set the colour blender texture as the 7th attachment
        //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT6, GL_TEXTURE_RECTANGLE, colorBlenderTexIDdual, 0);

        ////check the FBO completeness status
        //GLenum status2 = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        //if (status2 == GL_FRAMEBUFFER_COMPLETE)
        //    printf("FBO setup successful !!! \n");
        //else
        //    printf("Problem with FBO setup");

        ////unbind FBO
        //glBindFramebuffer(GL_FRAMEBUFFER, 0);

        //// remove all up



        ////////////////////////
        //////////////////////////////////////////////////////////////////////////////////



        glGenBuffers(2, buffers);
        GLuint maxNodes = 20 * width * height;
        GLint nodeSize = 5 * sizeof(GLfloat) + sizeof(GLuint); // The size of a linked list node

        // Our atomic counter
        glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 0, buffers[COUNTER_BUFFER]);
        glBufferData(GL_ATOMIC_COUNTER_BUFFER, sizeof(GLuint), NULL, GL_DYNAMIC_DRAW);

        // The buffer for the head pointers, as an image texture
        glGenTextures(1, &headPtrTex);
        glBindTexture(GL_TEXTURE_2D, headPtrTex);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_R32UI, width, height);
        glBindImageTexture(0, headPtrTex, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32UI);

        // The buffer of linked lists
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, buffers[LINKED_LIST_BUFFER]);
        glBufferData(GL_SHADER_STORAGE_BUFFER, maxNodes* nodeSize, NULL, GL_DYNAMIC_DRAW);

        oitshader.setuInt("MaxNodes",maxNodes);
       



        GLuint programHandle = oitshader.ID;
        glLinkProgram(programHandle);
        pass1Index = glGetSubroutineIndex(programHandle, GL_FRAGMENT_SHADER, "pass1");
        pass2Index = glGetSubroutineIndex(programHandle, GL_FRAGMENT_SHADER, "pass2");
        glUniform1ui(glGetUniformLocation(programHandle, "MaxNodes"), maxNodes);

       /* std::cout << pass1Index<<std::endl;
        std::cout << pass2Index<<std::endl;
        std::cout << programHandle<<std::endl;*/


         //generate the quad vertices
        glm::vec2 quadVerts[4];
        quadVerts[0] = glm::vec2(0, 0);
        quadVerts[1] = glm::vec2(1, 0);
        quadVerts[2] = glm::vec2(1, 1);
        quadVerts[3] = glm::vec2(0, 1);

        //generate quad indices
        GLushort quadIndices[] = { 0,1,2,0,2,3 };

        //generate quad  vertex array and vertex buffer objects
        glGenVertexArrays(1, &quadVAOID);
        glGenBuffers(1, &quadVBOID);
        glGenBuffers(1, &quadIndicesID);

        glBindVertexArray(quadVAOID);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBOID);
        //pass quad vertices to buffer object memory
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVerts), &quadVerts[0], GL_STATIC_DRAW);

        //GL_CHECK_ERRORS

            //enable vertex attribute array for position
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

        //pass the quad indices to the element array buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadIndicesID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), &quadIndices[0], GL_STATIC_DRAW);
        glBindVertexArray(0);




        // Set up a  VAO for the full-screen quad
        float wid = width / 2,hi=height/2;
        GLfloat verts[] = { -wid, -hi, 0.0f, wid, -hi, 0.0f,
          wid, hi, 0.0f, -wid, hi, 0.0f };
        GLuint bufHandle;
        glGenBuffers(1, &bufHandle);
        glBindBuffer(GL_ARRAY_BUFFER, bufHandle);
        glBufferData(GL_ARRAY_BUFFER, 4 * 3 * sizeof(GLfloat), verts, GL_STATIC_DRAW);

        // Set up the vertex array object
        glGenVertexArrays(1, &fsQuad);
        glBindVertexArray(fsQuad);

        glBindBuffer(GL_ARRAY_BUFFER, bufHandle);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);  // Vertex position

        glBindVertexArray(0);



        std::vector<GLuint> headPtrClearBuf(width* height, 0xffffffff);
        glGenBuffers(1, &clearBuf);
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, clearBuf);
        glBufferData(GL_PIXEL_UNPACK_BUFFER, headPtrClearBuf.size() * sizeof(GLuint),
           &headPtrClearBuf[0], GL_STATIC_COPY);
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, NULL);


       



    }

    GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path) {

        // Create the shaders
        GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
        GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

        // Read the Vertex Shader code from the file
        std::string VertexShaderCode;
        std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
        if (VertexShaderStream.is_open()) {
            std::stringstream sstr;
            sstr << VertexShaderStream.rdbuf();
            VertexShaderCode = sstr.str();
            VertexShaderStream.close();
        }
        else {
            printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
            getchar();
            return 0;
        }

        // Read the Fragment Shader code from the file
        std::string FragmentShaderCode;
        std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
        if (FragmentShaderStream.is_open()) {
            std::stringstream sstr;
            sstr << FragmentShaderStream.rdbuf();
            FragmentShaderCode = sstr.str();
            FragmentShaderStream.close();
        }

        GLint Result = GL_FALSE;
        int InfoLogLength;


        // Compile Vertex Shader
        printf("Compiling shader : %s\n", vertex_file_path);
        char const* VertexSourcePointer = VertexShaderCode.c_str();
        glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
        glCompileShader(VertexShaderID);

        // Check Vertex Shader
        glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
        glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if (InfoLogLength > 0) {
            std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
            glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
            printf("%s\n", &VertexShaderErrorMessage[0]);
        }



        // Compile Fragment Shader
        printf("Compiling shader : %s\n", fragment_file_path);
        char const* FragmentSourcePointer = FragmentShaderCode.c_str();
        glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
        glCompileShader(FragmentShaderID);

        // Check Fragment Shader
        glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
        glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if (InfoLogLength > 0) {
            std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
            glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
            printf("%s\n", &FragmentShaderErrorMessage[0]);
        }



        // Link the program
        printf("Linking program\n");
        GLuint ProgramID = glCreateProgram();
        glAttachShader(ProgramID, VertexShaderID);
        glAttachShader(ProgramID, FragmentShaderID);
        glLinkProgram(ProgramID);

        // Check the program
        glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
        glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if (InfoLogLength > 0) {
            std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
            glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
            printf("%s\n", &ProgramErrorMessage[0]);
        }


        glDetachShader(ProgramID, VertexShaderID);
        glDetachShader(ProgramID, FragmentShaderID);

        glDeleteShader(VertexShaderID);
        glDeleteShader(FragmentShaderID);

        return ProgramID;
    }

	virtual void resizeWindow(int width, int height) override;

    bool mouseMove(double xpos, double ypos) override;

    bool scrollWheel(double xoffset, double yoffset) override;

    void draw() override;

    virtual void prepareToDraw() {}

	virtual void drawAuxiliaryInfo();

	virtual void drawImGui();

    void shadowPass();

    void renderPass();

    // objects drawn with a shadowMapRenderer (during shadow pass) will cast a
    // shadow
    virtual void drawShadowCastingObjects() = 0;

    // objects drawn with a shadowShader (during the render pass) will have
    // shadows cast on them
    virtual void drawObjectsWithShadows() = 0;

    // objects drawn with basic shadowShader (during the render pass) will not
    // have shadows cast on them
    virtual void drawObjectsWithoutShadows() = 0;
    //virtual void stohastic_main() = 0;

    virtual void drawObjectsWithoutShadows1() = 0;

    virtual void drawObjectsWithoutShadowsdual() = 0;
    virtual void linkedlist_main() = 0;

    TrackingCamera camera;
    ShadowCastingLight light;
    ShadowMapFBO shadowMapFBO;
    float shadowbias = 0.0001f;
    //GLSLProgram prog;
    Shader shadowShader =
        Shader(SHADER_FOLDER "/basic_lighting.vert",
            SHADER_FOLDER "/basic_shadow_lighting.frag");
    Shader shadowMapRenderer = Shader(SHADER_FOLDER "/render_shadow.vert",
        SHADER_FOLDER "/render_shadow.frag");
    Shader basicShader = Shader(SHADER_FOLDER "/basic_lighting.vert",
        SHADER_FOLDER "/basic_lighting.frag");
    /// <summary>
    /// 
    /// </summary>
    Shader colorshader = Shader(SHADER_FOLDER "/vshader.vert",
        SHADER_FOLDER "/fshader.frag");
    Shader frontpeelshader = Shader(SHADER_FOLDER "/basic_lighting.vert",
        SHADER_FOLDER "/frontpeel.frag");
    Shader blendshader = Shader(SHADER_FOLDER "/blend.vert",
        SHADER_FOLDER "/blend.frag");
    Shader finalshader = Shader(SHADER_FOLDER "/blend.vert",
        SHADER_FOLDER "/final.frag");
    /// <summary>
    /// 
    /// </summary>

    Shader initshader = Shader(SHADER_FOLDER "/basic_lighting.vert",
        SHADER_FOLDER "/dual_init.frag");
    Shader dualPeelShader = Shader(SHADER_FOLDER "/basic_lighting.vert",
        SHADER_FOLDER "/dual_peel.frag");
    Shader blendshaderdual = Shader(SHADER_FOLDER "/blend.vert",
        SHADER_FOLDER "/blend.frag");
    Shader finalshaderdual = Shader(SHADER_FOLDER "/blend.vert",
        SHADER_FOLDER "/finaldual.frag");
    Shader oitshader = Shader(SHADER_FOLDER"/basic_lighting.vert", 
        SHADER_FOLDER"/oit3.frag");

    Shader oitshader2 = Shader(SHADER_FOLDER"/blend.vert",
        SHADER_FOLDER"/oit3.frag");

	bool drawSelfShadows = false;
	Model ground = getGroundModel();


    //GLbitfield GL_SHADER_STORAGE_BARRIER_BIT;
   // int width, height;
    GLuint buffers[2], fsQuad, headPtrTex;
    GLuint pass1Index, pass2Index;
    GLuint clearBuf;
    //GLuint GL_ATOMIC_COUNTER_BUFFER;
    //FBO id
    GLuint fbo[2];
    //FBO colour attachment IDs
    GLuint texID[2];
    //FBO depth attachment IDs
    GLuint depthTexID[2];

    //colour blending FBO ID
    GLuint colorBlenderFBOID;
    //colour blend FBO colour attachment texture ID
    GLuint colorBlenderTexID;

    //occlusion query ID
    GLuint queryId;

    //fullscreen quad vao and vbos
    GLuint quadVAOID;
    GLuint quadVBOID;
    GLuint quadIndicesID;

    //cube vertex array and vertex buffer object IDs
    GLuint cubeVBOID;
    GLuint cubeVAOID;
    GLuint cubeIndicesID;

    //shaders for cube, front to back peeling, blending and final rendering
    //GLSLShader cubeShader, frontPeelShader, blendShader, finalShader;

    //total number of depth peeling passes
    int NUM_PASSES = 10;

    //flag to use occlusion queries
    bool bUseOQ = true;

    //flag to use depth peeling 
    bool bShowDepthPeeling = true;


    const float MAX_DEPTH = 1.0f;

    ////////////// for dual   ------------------------------------------------!!!!!!!!!!!!!!
    // dual depth peeling FBO id
    GLuint dualDepthFBOID;
    //back texture colour attachment IDs
    GLuint backTexID[2];
    //front texture colour attachment IDs
    GLuint texIDdual[2];
    //back texture depth attachment IDs
    GLuint depthTexIDdual[2];
    //colour blending FBO ID
    GLuint colorBlenderFBOIDdual;
    //colour blend FBO colour attachment texture ID
    GLuint colorBlenderTexIDdual;

    GLuint StohisticBuffer;

    //occlusion query ID
    //GLuint queryId;
    GLuint Visarr;
    ////fullscreen quad vao and vbos
    //GLuint quadVAOID;
    //GLuint quadVBOID;
    //GLuint quadIndicesID;

    ////cube vertex array and vertex buffer object IDs
    //GLuint cubeVBOID;
    //GLuint cubeVAOID;
    //GLuint cubeIndicesID;

    //shaders for cube, initialization, dual depth peeling, blending and final rendering
    //GLSLShader cubeShader, initShader, dualPeelShader, blendShader, finalShader;

    //total number of depth peeling passes
    //const int NUM_PASSES = 4;

    //flag to use occlusion queries
   // bool bUseOQ = true;

    //flag to use dual depth peeling
    //bool bShowDepthPeeling = true;

    //blending colour alpha
    float alpha = 0.6f;

    //background colour
    glm::vec4 bg = glm::vec4(0, 0, 0, 0);

    //colour attachment IDs
    GLenum attachID[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT3 };

    //draw buffer attachments
    GLenum drawBuffers[7] = { GL_COLOR_ATTACHMENT0,
                               GL_COLOR_ATTACHMENT1,
                               GL_COLOR_ATTACHMENT2,
                               GL_COLOR_ATTACHMENT3,
                               GL_COLOR_ATTACHMENT4,
                               GL_COLOR_ATTACHMENT5,
                               GL_COLOR_ATTACHMENT6
    };


    ///////////
    float transparency = 0.5;
    int maxfrags = 5;

private:
    int PCF_mode=0;
    int PCF_samples_num=25;
    float shadow_spread=0.002;
    float light_size = 1;
    



};


}  // namespace gui
}  // namespace crl