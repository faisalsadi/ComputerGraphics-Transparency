#pragma once
#include <cstdio>
#include <cstdlib>
#include <gui/application.h>
#include <utils/logger.h>
#include <gui/fancy3dapp.h>
#include <glad/glad.h>

using namespace crl::gui;
using std::endl;
using std::cerr;

using glm::vec3;
class App : public Fancy3DApp {
public:
    
    App(const char* title = "Fancy app",
        std::string iconPath = DATA_FOLDER "/crl_icon_grey.png")
        : Fancy3DApp(title, iconPath) {}

     virtual ~App() override {}

    void process() override {
        // Logger::consolePrint(Eigen::Vector3d(1.0, 0.0, 0.0), "Test\n");
    }
  
  
    //function to draw a fullscreen quad
    void  DrawFullScreenQuad() {
        //bind the quad vertex array object
        //colorshader.setUniform("its", false);
        glBindVertexArray(quadVAOID);

        //draw 2 triangles
        //glViewport(0, 0, 1280, 720);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
        //colorShader.setUniform("its", true);
    }

    // objects drawn with a shadowMapRenderer (during shadow pass) will cast a
    // shadow
    virtual void drawShadowCastingObjects() override {
     //   basicShader.setBool("drawtransparency", model.drawtransparency);
        //camera.position();
        model.sort_triangles(camera.position());
       // model.draw(shadowMapRenderer, crl::V3D::V3D(1, 0, 0));
     //   basicShader.setBool("drawtransparency", model2.drawtransparency);
     //   model2.sort_triangles(camera.position());
      //  model2.draw(shadowMapRenderer,0.3);
        model.draw(shadowMapRenderer, crl::V3D::V3D(1, 0,0 ),0.4);
        model2.sort_triangles(camera.position());
        model2.draw(shadowMapRenderer, crl::V3D::V3D(0, 1, 0), 0.4);
        
        model3.sort_triangles(camera.position());
        model3.draw(shadowMapRenderer, crl::V3D::V3D(0, 1, 0), 0.4);

        model4.sort_triangles(camera.position());
        model4.draw(shadowMapRenderer, crl::V3D::V3D(0, 1, 0), 0.4);

        model5.sort_triangles(camera.position());
        model5.draw(shadowMapRenderer, crl::V3D::V3D(0, 1, 0), 0.4);

        model6.sort_triangles(camera.position());
        model6.draw(shadowMapRenderer, crl::V3D::V3D(0, 1, 0), 0.4);
        
        model7.sort_triangles(camera.position());
        model7.draw(shadowMapRenderer, crl::V3D::V3D(0, 0, 0), 0.4);
        
    }

    // objects drawn with a shadowShader (during the render pass) will have
    // shadows cast on them
    virtual void drawObjectsWithShadows() override {
        if (drawSelfShadows == true)
        {
            model.sort_triangles(camera.position());
            model.draw(shadowShader, crl::V3D::V3D(1, 0, 0),0.4);
            model2.sort_triangles(camera.position());
            model2.draw(shadowShader, crl::V3D::V3D(0, 1, 0), 0.4);

            model3.sort_triangles(camera.position());
            model3.draw(shadowShader, crl::V3D::V3D(0, 1, 0), 0.4);

            model4.sort_triangles(camera.position());
            model4.draw(shadowShader, crl::V3D::V3D(0, 1, 0), 0.4);

            model5.sort_triangles(camera.position());
            model5.draw(shadowShader, crl::V3D::V3D(0, 1, 0), 0.4);

            model6.sort_triangles(camera.position());
            model6.draw(shadowShader, crl::V3D::V3D(0, 1, 0), 0.4);


            model7.sort_triangles(camera.position());
            model7.draw(shadowShader, crl::V3D::V3D(0, 0, 0), 0.4);
        }
        ground.draw(shadowShader);
    }

    // objects drawn with basic shadowShader (during the render pass) will not
    // have shadows cast on them
    virtual void drawObjectsWithoutShadows() override {
        if (drawSelfShadows == false) {

            if (NUM_PASSES == 1)
            {
                drawObjectsWithoutShadows1();
            }
            else {
                //
                //clear colour and depth buffer
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                //bind the colour blending FBO
                glBindFramebuffer(GL_FRAMEBUFFER, colorBlenderFBOID);
                //set the first colour attachment as the draw buffer
                glDrawBuffer(GL_COLOR_ATTACHMENT0);
                //clear the colour and depth buffer
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                // 1. In the first pass, we render normally with depth test enabled to get the nearest surface
                glEnable(GL_DEPTH_TEST);
                //
               // basicShader.setBool("drawtransparency", model.drawtransparency);

                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                model.sort_triangles(camera.position());
                model.draw(basicShader, crl::V3D::V3D(1, 0, 0), 0.5);
                //basicShader.setBool("drawtransparency", model2.drawtransparency);
                model2.sort_triangles(camera.position());
                model2.draw(basicShader, crl::V3D::V3D(0, 1, 0), 0.5);
                model3.sort_triangles(camera.position());
                model3.draw(basicShader, crl::V3D::V3D(0, 0, 1), 0.5);
                model4.sort_triangles(camera.position());
                model4.draw(basicShader, crl::V3D::V3D(1, 0, 0), 0.5);
                model5.sort_triangles(camera.position());
                model5.draw(basicShader, crl::V3D::V3D(0, 1, 0), 0.5);
                model6.sort_triangles(camera.position());
                model6.draw(basicShader, crl::V3D::V3D(0, 0, 1), 0.5);


                model1.sort_triangles(camera.position());
                model1.draw(basicShader, crl::V3D::V3D(1, 0, 0), 0.5);
                //basicShader.setBool("drawtransparency", model2.drawtransparency);
                model21.sort_triangles(camera.position());
                model21.draw(basicShader, crl::V3D::V3D(0, 1, 0), 0.5);
                model31.sort_triangles(camera.position());
                model31.draw(basicShader, crl::V3D::V3D(0, 0, 1), 0.5);
                model41.sort_triangles(camera.position());
                model41.draw(basicShader, crl::V3D::V3D(1, 0, 0), 0.5);
                model51.sort_triangles(camera.position());
                model51.draw(basicShader, crl::V3D::V3D(0, 1, 0), 0.5);
                model61.sort_triangles(camera.position());
                model61.draw(basicShader, crl::V3D::V3D(0, 0, 1), 0.5);






                /* model7.sort_triangles(camera.position());
                 model7.draw(basicShader, crl::V3D::V3D(0, 0, 0), 0.5);*/

                 //setTransformFromWidgets(camera.getViewMatrix(), camera.getProjectionMatrix(), model.getTransform());
                 ///
                     // 2. Depth peeling + blending pass
                int numLayers = (NUM_PASSES - 1) * 2;

                //for each pass
                for (int layer = 1; layer < NUM_PASSES; layer++) {
                    int currId = layer % 2;
                    int prevId = 1 - currId;

                    //bind the current FBO
                    glBindFramebuffer(GL_FRAMEBUFFER, fbo[currId]);
                    //set the first colour attachment as draw buffer
                    glDrawBuffer(GL_COLOR_ATTACHMENT0);

                    //set clear colour to black
                    glClearColor(0.8f, 0.8f, 0.8f, 1.00f);

                    //glClearColor(0, 0, 0, 0);
                    //clear the colour and depth buffers
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                    //disbale blending and depth testing
                    glDisable(GL_BLEND);
                    glEnable(GL_DEPTH_TEST);

                    glBeginQuery(35092, queryId);

                    //bind the depth texture from the previous step
                    glBindTexture(GL_TEXTURE_RECTANGLE, depthTexID[prevId]);


                    model.sort_triangles(camera.position());
                    model.draw(frontpeelshader, crl::V3D::V3D(1, 0, 0), 0.5);
                    //basicShader.setBool("drawtransparency", model2.drawtransparency);
                    model2.sort_triangles(camera.position());
                    model2.draw(frontpeelshader, crl::V3D::V3D(0, 1, 0), 0.5);
                    model3.sort_triangles(camera.position());
                    model3.draw(frontpeelshader, crl::V3D::V3D(0, 0, 1), 0.5);
                    model4.sort_triangles(camera.position());
                    model4.draw(frontpeelshader, crl::V3D::V3D(1, 0, 0), 0.5);
                    model5.sort_triangles(camera.position());
                    model5.draw(frontpeelshader, crl::V3D::V3D(0, 1, 0), 0.5);
                    model6.sort_triangles(camera.position());
                    model6.draw(frontpeelshader, crl::V3D::V3D(0, 0, 1), 0.5);

                    model1.sort_triangles(camera.position());
                    model1.draw(frontpeelshader, crl::V3D::V3D(1, 0, 0), 0.5);
                    //basicShader.setBool("drawtransparency", model2.drawtransparency);
                    model21.sort_triangles(camera.position());
                    model21.draw(frontpeelshader, crl::V3D::V3D(0, 1, 0), 0.5);
                    model31.sort_triangles(camera.position());
                    model31.draw(frontpeelshader, crl::V3D::V3D(0, 0, 1), 0.5);
                    model41.sort_triangles(camera.position());
                    model41.draw(frontpeelshader, crl::V3D::V3D(1, 0, 0), 0.5);
                    model51.sort_triangles(camera.position());
                    model51.draw(frontpeelshader, crl::V3D::V3D(0, 1, 0), 0.5);
                    model61.sort_triangles(camera.position());
                    model61.draw(frontpeelshader, crl::V3D::V3D(0, 0, 1), 0.5);






                    /*model7.sort_triangles(camera.position());
                    model7.draw(frontpeelshader, crl::V3D::V3D(0, 0, 0), 0.5);*/


                    ////


                    glEndQuery(35092);
                    //bind the colour blender FBO
                    glBindFramebuffer(GL_FRAMEBUFFER, colorBlenderFBOID);
                    //render to its first colour attachment 
                    glDrawBuffer(GL_COLOR_ATTACHMENT0);

                    //enable blending but disable depth testing
                    glDisable(GL_DEPTH_TEST);
                    glEnable(GL_BLEND);


                    //change the blending equation to add
                    glBlendEquation(GL_FUNC_ADD);
                    //use separate blending function
                    glBlendFuncSeparate(GL_DST_ALPHA, GL_ONE,
                        GL_ZERO, GL_ONE_MINUS_SRC_ALPHA);

                    //bind the result from the previous iteration as texture
                    glBindTexture(GL_TEXTURE_RECTANGLE, texID[currId]);
                    //bind the blend shader and then draw a fullscreen quad
                    blendshader.use();
                    //blendshader.setUniform("its", false);
                    DrawFullScreenQuad();
                    blendshader.unuse();

                    //disable blending
                    glDisable(GL_BLEND);

                    // 3. Final render pass
                    //remove the FBO 
                    glBindFramebuffer(GL_FRAMEBUFFER, 0);
                    //restore the default back buffer
                    glDrawBuffer(GL_BACK_LEFT);
                    //disable depth testing and blending
                    glDisable(GL_DEPTH_TEST);
                    glDisable(GL_BLEND);

                    //bind the colour blender texture
                    glBindTexture(GL_TEXTURE_RECTANGLE, colorBlenderTexID);
                    //bind the final shader
                    finalshader.use();
                    //finalshader.setUniform("its", false);
                    DrawFullScreenQuad();
                    finalshader.unuse();




                }


            }
        }
    }
    
 
  
    virtual void linkedlist_main() {
        //oitshader.use();


        /// <summary>
        /// Re clean buffer  MUST RESOTRE !!
        /// </summary>
        /// 
        GLuint zero = 0;
        glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 0, buffers[COUNTER_BUFFER]);
        glBufferSubData(GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(GLuint), &zero);

        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, clearBuf);
        glBindTexture(GL_TEXTURE_2D, headPtrTex);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RED_INTEGER,
            GL_UNSIGNED_INT, NULL);
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, NULL);
       
        pass1();
        pass2();
        

    }
    virtual void DrawObjects() {
        //model7.orientation.;
        //float x1 = light.position().x;
        oitshader.setVec4("LightPos", glm::vec4(light.position(), 1));
        oitshader.setVec3("LightIntensity", vec3(1.f));
        oitshader.setVec4("Ka", vec4(0.2f, 0.2f, 0.9f, 0.55f));
        oitshader.setVec4("Kd", vec4(0.2f, 0.2f, 0.9f, 0.55f));
        oitshader.setInt("maxfrags", maxfrags);
        //model.sort_triangles(camera.position());
        model.draw(oitshader, crl::V3D::V3D(1, 0, 0), transparency);
        //model2.sort_triangles(camera.position());
        model2.draw(oitshader, crl::V3D::V3D(0, 1, 0), transparency);
        //model3.sort_triangles(camera.position());
        model3.draw(oitshader, crl::V3D::V3D(0, 0, 1), transparency);
        //model4.sort_triangles(camera.position());
        model4.draw(oitshader, crl::V3D::V3D(1, 0, 0), transparency);
        //model5.sort_triangles(camera.position());
        model5.draw(oitshader, crl::V3D::V3D(0, 1, 0), transparency);
        //model6.sort_triangles(camera.position());
        model6.draw(oitshader, crl::V3D::V3D(0, 0, 1), transparency);



        //model1.sort_triangles(camera.position());
        model1.draw(oitshader, crl::V3D::V3D(1, 0, 0), transparency);
        //basicShader.setBool("drawtransparency", model2.drawtransparency);
        //model21.sort_triangles(camera.position());
        model21.draw(oitshader, crl::V3D::V3D(0, 1, 0), transparency);
        //model31.sort_triangles(camera.position());
        model31.draw(oitshader, crl::V3D::V3D(0, 0, 1), transparency);
        //model41.sort_triangles(camera.position());
        model41.draw(oitshader, crl::V3D::V3D(1, 0, 0), transparency);
        //model51.sort_triangles(camera.position());
        model51.draw(oitshader, crl::V3D::V3D(0, 1, 0), transparency);
        //model61.sort_triangles(camera.position());
        model61.draw(oitshader, crl::V3D::V3D(0, 0, 1), transparency);

        //model7.draw(oitshader, crl::V3D::V3D(0, 0, 0), transparency);
        ground.draw(oitshader);



    }

    void pass1() {

        glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &pass1Index);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glDepthMask(GL_FALSE);
        //glPolygonMode(GL_FRONT, GL_FILL);
        // draw scene
        DrawObjects();


        glFinish();
    }

    void pass2() {


        glMemoryBarrier(GL_ALL_BARRIER_BITS);

        glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &pass2Index);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        oitshader.setInt("maxfrags", maxfrags);
        //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glBindVertexArray(fsQuad);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        //glDrawElements(GL_TRIANGLES, 4, GL_UNSIGNED_SHORT, 0);
        glBindVertexArray(0);
        //DrawFullScreenQuad();

    }



    virtual void drawObjectsWithoutShadows1() override {
        if (drawSelfShadows == false) {


           
            model.sort_triangles(camera.position());
            model.draw(basicShader, crl::V3D::V3D(1, 0, 0), 0.5);
            //basicShader.setBool("drawtransparency", model2.drawtransparency);
            model2.sort_triangles(camera.position());
            model2.draw(basicShader, crl::V3D::V3D(0, 1, 0), 0.5);
            model3.sort_triangles(camera.position());
            model3.draw(basicShader, crl::V3D::V3D(0, 0, 1), 0.5);
            model4.sort_triangles(camera.position());
            model4.draw(basicShader, crl::V3D::V3D(1, 0, 0), 0.5);
            model5.sort_triangles(camera.position());
            model5.draw(basicShader, crl::V3D::V3D(0, 1, 0), 0.5);
            model6.sort_triangles(camera.position());
            model6.draw(basicShader, crl::V3D::V3D(0, 0, 1), 0.5);


            model1.sort_triangles(camera.position());
            model1.draw(basicShader, crl::V3D::V3D(1, 0, 0), 0.5);
            //basicShader.setBool("drawtransparency", model2.drawtransparency);
            model21.sort_triangles(camera.position());
            model21.draw(basicShader, crl::V3D::V3D(0, 1, 0), 0.5);
            model31.sort_triangles(camera.position());
            model31.draw(basicShader, crl::V3D::V3D(0, 0, 1), 0.5);
            model41.sort_triangles(camera.position());
            model41.draw(basicShader, crl::V3D::V3D(1, 0, 0), 0.5);
            model51.sort_triangles(camera.position());
            model51.draw(basicShader, crl::V3D::V3D(0, 1, 0), 0.5);
            model61.sort_triangles(camera.position());
            model61.draw(basicShader, crl::V3D::V3D(0, 0, 1), 0.5);
            
            ground.draw(basicShader);

        }
    }





    virtual void drawObjectsWithoutShadowsdual() override {
        if (drawSelfShadows == false) {

            if (NUM_PASSES == 1)
            {
                drawObjectsWithoutShadows1();
            }
            else {
                //clear colour and depth buffer
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


                //disble depth test and enable alpha blending
                glDisable(GL_DEPTH_TEST);
                glEnable(GL_BLEND);

                //bind dual depth FBO
                glBindFramebuffer(GL_FRAMEBUFFER, dualDepthFBOID);

                // Render targets 1 and 2 store the front and back colors
        // Clear to 0.0 and use MAX blending to filter written color
        // At most one front color and one back color can be written every pass
                glDrawBuffers(2, &drawBuffers[1]);
                glClearColor(0.8, 0.8, 0.8, 1);
                glClear(GL_COLOR_BUFFER_BIT);

                // Render target 0 stores (-minDepth, maxDepth)
                glDrawBuffer(drawBuffers[0]);
                //clear the offscreen texture with -MAX_DEPTH
                glClearColor(-MAX_DEPTH, -MAX_DEPTH, 0, 0);
                glClear(GL_COLOR_BUFFER_BIT);
                //enable max blending
                glBlendEquation(GL_MAX);
                //render scene with the initialization shader
                //DrawScene(MVP, initshader);


                model.sort_triangles(camera.position());
                model.draw(initshader, crl::V3D::V3D(1, 0, 0), 0.5);
                //basicShader.setBool("drawtransparency", model2.drawtransparency);
                model2.sort_triangles(camera.position());
                model2.draw(initshader, crl::V3D::V3D(0, 1, 0), 0.5);
                model3.sort_triangles(camera.position());
                model3.draw(initshader, crl::V3D::V3D(0, 0, 1), 0.5);
                model4.sort_triangles(camera.position());
                model4.draw(initshader, crl::V3D::V3D(1, 0, 0), 0.5);
                model5.sort_triangles(camera.position());
                model5.draw(initshader, crl::V3D::V3D(0, 1, 0), 0.5);
                model6.sort_triangles(camera.position());
                model6.draw(initshader, crl::V3D::V3D(0, 0, 1), 0.5);

                model1.sort_triangles(camera.position());
                model1.draw(initshader, crl::V3D::V3D(1, 0, 0), 0.5);
                //basicShader.setBool("drawtransparency", model2.drawtransparency);
                model21.sort_triangles(camera.position());
                model21.draw(initshader, crl::V3D::V3D(0, 1, 0), 0.5);
                model31.sort_triangles(camera.position());
                model31.draw(initshader, crl::V3D::V3D(0, 0, 1), 0.5);
                model41.sort_triangles(camera.position());
                model41.draw(initshader, crl::V3D::V3D(1, 0, 0), 0.5);
                model51.sort_triangles(camera.position());
                model51.draw(initshader, crl::V3D::V3D(0, 1, 0), 0.5);
                model61.sort_triangles(camera.position());
                model61.draw(initshader, crl::V3D::V3D(0, 0, 1), 0.5);


                // 2. Depth peeling + blending pass
                glDrawBuffer(drawBuffers[6]);
                //clear color buffer with the background colour
                glClearColor(0.8,0.8,0.8, 1);
                glClear(GL_COLOR_BUFFER_BIT);

                
                int numLayers = (NUM_PASSES - 1) * 2;

                int currId = 0;
                //for each pass
                for (int layer = 1;  layer < numLayers; layer++) {
                    currId = layer % 2;
                    int prevId = 1 - currId;
                    int bufId = currId * 3;

                    //render to 2 colour attachments simultaneously
                    glDrawBuffers(2, &drawBuffers[bufId + 1]);
                    //set clear color to black and clear colour buffer
                    glClearColor(0.8, 0.8, 0.8, 1);
                    glClear(GL_COLOR_BUFFER_BIT);

                    //alternate the colour attachment for draw buffer
                    glDrawBuffer(drawBuffers[bufId + 0]);
                    //clear the color to -MAX_DEPTH and clear colour buffer
                    glClearColor(-MAX_DEPTH, -MAX_DEPTH, 0, 0);
                    glClear(GL_COLOR_BUFFER_BIT);

                    //Render to three draw buffers simultaneously
                    // Render target 0: RG32F MAX blending
                    // Render target 1: RGBA MAX blending
                    // Render target 2: RGBA MAX blending
                    glDrawBuffers(3, &drawBuffers[bufId + 0]);
                    //enable max blending
                    glBlendEquation(GL_MAX);

                    //bind depth texture to texture unit 0
                    glActiveTexture(GL_TEXTURE0);
                    glBindTexture(GL_TEXTURE_RECTANGLE, depthTexIDdual[prevId]);

                    //bind colour attachment texture to texture unit 1
                    glActiveTexture(GL_TEXTURE1);
                    glBindTexture(GL_TEXTURE_RECTANGLE, texIDdual[prevId]);

                    //draw scene using the dual peel shader 
                    //DrawScene(MVP, dualPeelShader, true, true);




                    model.sort_triangles(camera.position());
                    model.draw(dualPeelShader, crl::V3D::V3D(1, 0, 0), 0.5);
                    //basicShader.setBool("drawtransparency", model2.drawtransparency);
                    model2.sort_triangles(camera.position());
                    model2.draw(dualPeelShader, crl::V3D::V3D(0, 1, 0), 0.5);
                    model3.sort_triangles(camera.position());
                    model3.draw(dualPeelShader, crl::V3D::V3D(0, 0, 1), 0.5);
                    model4.sort_triangles(camera.position());
                    model4.draw(dualPeelShader, crl::V3D::V3D(1, 0, 0), 0.5);
                    model5.sort_triangles(camera.position());
                    model5.draw(dualPeelShader, crl::V3D::V3D(0, 1, 0), 0.5);
                    model6.sort_triangles(camera.position());
                    model6.draw(dualPeelShader, crl::V3D::V3D(0, 0, 1), 0.5);

                    model1.sort_triangles(camera.position());
                    model1.draw(dualPeelShader, crl::V3D::V3D(1, 0, 0), 0.5);
                    //basicShader.setBool("drawtransparency", model2.drawtransparency);
                    model21.sort_triangles(camera.position());
                    model21.draw(dualPeelShader, crl::V3D::V3D(0, 1, 0), 0.5);
                    model31.sort_triangles(camera.position());
                    model31.draw(dualPeelShader, crl::V3D::V3D(0, 0, 1), 0.5);
                    model41.sort_triangles(camera.position());
                    model41.draw(dualPeelShader, crl::V3D::V3D(1, 0, 0), 0.5);
                    model51.sort_triangles(camera.position());
                    model51.draw(dualPeelShader, crl::V3D::V3D(0, 1, 0), 0.5);
                    model61.sort_triangles(camera.position());
                    model61.draw(dualPeelShader, crl::V3D::V3D(0, 0, 1), 0.5);







                    // Full screen pass to alpha-blend the back color
                    glDrawBuffer(drawBuffers[6]);

                    //set the over blending 
                    glBlendEquation(GL_FUNC_ADD);
                    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

                  
                    //glBeginQuery(35092, queryId);
                        //bind the back colour attachment to texture unit 0
                        glActiveTexture(GL_TEXTURE0);
                    glBindTexture(GL_TEXTURE_RECTANGLE, backTexID[currId]);

                    //use blending shader and draw a fullscreen quad
                    blendshaderdual.use();
                    DrawFullScreenQuad();
                    blendshaderdual.unuse();

                    //if we initiated the occlusion query, we end it and get
                    //the query result which is the total number of samples
                    //output from the blending result
                    //glEndQuery(35092);
                   // GL_CHECK_ERRORS




                    //disable alpha blending
                    glDisable(GL_BLEND);

                    // 3. Final render pass
                    //remove the FBO 
                    glBindFramebuffer(GL_FRAMEBUFFER, 0);
                    //restore the default back buffer
                    glDrawBuffer(GL_BACK_LEFT);

                    //bind the depth texture to texture unit 0
                    glActiveTexture(GL_TEXTURE0);
                    glBindTexture(GL_TEXTURE_RECTANGLE, depthTexIDdual[currId]);

                    //bind the depth texture to colour texture to texture unit 1
                    glActiveTexture(GL_TEXTURE1);
                    glBindTexture(GL_TEXTURE_RECTANGLE, texIDdual[currId]);

                    //bind the colour blender texture to texture unit 2
                    glActiveTexture(GL_TEXTURE2);
                    glBindTexture(GL_TEXTURE_RECTANGLE, colorBlenderTexIDdual);

                    //bind the final shader and draw a fullscreen quad
                    finalshaderdual.use();
                    DrawFullScreenQuad();
                    finalshaderdual.unuse();
                }


                //GL_CHECK_ERRORS




            }
        }
    }

    virtual bool drop(int count, const char** fileNames) override {
        model = Model(fileNames[count - 1]);
        model2 = Model(fileNames[count ]);
        return true;
    }

public:

    Model model = Model(DATA_FOLDER "/meshes/sphere.obj");

    Model model2 = Model(DATA_FOLDER "/meshes/sphere.obj");

    Model model3 = Model(DATA_FOLDER "/meshes/sphere.obj");
    Model model4 = Model(DATA_FOLDER "/meshes/sphere.obj");
    Model model5 = Model(DATA_FOLDER "/meshes/sphere.obj");
    Model model6 = Model(DATA_FOLDER "/meshes/sphere.obj");


    Model model1 = Model(DATA_FOLDER "/meshes/sphere.obj");

    Model model21 = Model(DATA_FOLDER "/meshes/sphere.obj");

    Model model31 = Model(DATA_FOLDER "/meshes/sphere.obj");
    Model model41 = Model(DATA_FOLDER "/meshes/sphere.obj");
    Model model51 = Model(DATA_FOLDER "/meshes/sphere.obj");
    Model model61 = Model(DATA_FOLDER "/meshes/sphere.obj");

    Model model7 = Model(DATA_FOLDER "/meshes/3d models/nanosuit.obj");

    ///meshes/cylinder.obj
    //"/meshes/3d models/nanosuit.obj"
    //"/meshes/sphere.obj"
    /// meshes/beethoven.obj
};
