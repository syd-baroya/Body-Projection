// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include "PointCloudRenderer.h"


using namespace glm;
using namespace Visualization;

PointCloudVertex testVertices[] =
{
    {vec3(-0.5f, -0.5f, -2.5f), 0, vec4(1.0f, 0.0f, 0.0f, 1.0f), mat4(1), ivec2(10, 0)},
    {vec3(0.5f, -0.5f, -2.5f), 0, vec4(0.0f, 1.0f, 0.0f, 1.0f), mat4(1), ivec2(20, 0)},
    {vec3(-0.5f,  0.5f, -2.5f), 0, vec4(0.0f, 0.0f, 1.0f, 1.0f), mat4(1), ivec2(30, 0)},
    { vec3(0.5f,  0.5f, -2.5f), 0, vec4(1.0f, 1.0f, 0.0f, 1.0f), mat4(1), ivec2(40, 0) },

    { vec3(-0.5f, -0.5f, -3.5f), 0, vec4(0.0f, 1.0f, 1.0f, 1.0f), mat4(1), ivec2(50, 0)},
    { vec3(0.5f, -0.5f, -3.5f), 0, vec4(1.0f, 0.0f, 1.0f, 1.0f), mat4(1), ivec2(60, 0)},
    { vec3(-0.5f,  0.5f, -3.5f), 0, vec4(1.0f, 1.0f, 0.5f, 1.0f), mat4(1), ivec2(70, 0)},
    { vec3(0.5f,  0.5f, -3.5f), 0, vec4(0.5f, 0.5f, 1.0f, 1.0f), mat4(1), ivec2(80, 0)}
};
static const GLfloat billboard_position_data[] = {
 -0.01f, -0.01f, 0.0f,
 0.01f, -0.01f, 0.0f,
 -0.01f, 0.01f, 0.0f,
 0.01f, 0.01f, 0.0f,
};

static const GLfloat billboard_texture_data[] = {
 0.0f, 0.0f,
 1.0f, 0.0f,
 0.0f, 1.0f,
 1.0f, 1.0f,
};

PointCloudRenderer::PointCloudRenderer()
{
    m_view = mat4(1);
    m_projection = mat4(1);

    vertShaderPath += "point_cloud.vert";
    fragShaderPath += "point_cloud.frag";
    computeShaderPath += "compute.glsl";
    blendVertPath += "two_col_blend.vert";
    blendFragPath += "two_col_blend.frag";

    for (int i = 0; i < 320 * 288 / 2; i++)
        ssbo_CPUMEM.outlineIndices[i] = -1;
}

PointCloudRenderer::~PointCloudRenderer()
{
    Delete();
}

void PointCloudRenderer::Create(GLFWwindow* window)
{
    CheckAssert(!m_initialized);
    m_initialized = true;

    m_window = window;
    glfwMakeContextCurrent(window);

    // Context Settings
    glEnable(GL_PROGRAM_POINT_SIZE);

    VSFSProgram = new Program(vertShaderPath, fragShaderPath);
    blendProgram = new Program(blendVertPath, blendFragPath);
    computeProgram = new ComputeProgram(8, 18, 1, computeShaderPath);
    atomicCounterBuff = new AtomicCounterBuffer();
    ssBuffObject = new ShaderStorageBuffer();


    glGenVertexArrays(1, &m_vertexArrayObject);
    glBindVertexArray(m_vertexArrayObject);
    glGenBuffers(1, &m_vertexBufferObject);
    glGenBuffers(1, &m_vertexTrans);
    glGenBuffers(1, &m_outlineBuffer);

    glGenBuffers(1, &billboard_position_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, billboard_position_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(billboard_position_data), billboard_position_data, GL_STATIC_DRAW);
    glGenBuffers(1, &billboard_texture_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, billboard_texture_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(billboard_texture_data), billboard_texture_data, GL_STATIC_DRAW);



    VSFSProgram->addUniform("model");
    VSFSProgram->addUniform("view");
    VSFSProgram->addUniform("projection");
    VSFSProgram->addUniform("enableShading");
    VSFSProgram->addUniform("xyTable");
    VSFSProgram->addUniform("depth");
    VSFSProgram->addUniform("totaltime");
    VSFSProgram->addUniform("texoffset");


    VSFSProgram->bind();
    GLuint TexLoc;
    TexLoc = glGetUniformLocation(VSFSProgram->getPID(), "scene_tex");
    glUniform1i(TexLoc, 0);

    // Deferred Stuff
    //init rectangle mesh (2 triangles) for the post processing
    glGenVertexArrays(1, &VertexArrayIDBox);
    glBindVertexArray(VertexArrayIDBox);

    //generate vertex buffer to hand off to OGL
    glGenBuffers(1, &VertexBufferIDBox);

    //set the current state to focus on our vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, VertexBufferIDBox);

    GLfloat* rectangle_vertices = new GLfloat[18];
    int verccount = 0;

    rectangle_vertices[verccount++] = -1.0, rectangle_vertices[verccount++] = -1.0, rectangle_vertices[verccount++] = 0.0;
    rectangle_vertices[verccount++] = 1.0, rectangle_vertices[verccount++] = -1.0, rectangle_vertices[verccount++] = 0.0;
    rectangle_vertices[verccount++] = -1.0, rectangle_vertices[verccount++] = 1.0, rectangle_vertices[verccount++] = 0.0;
    rectangle_vertices[verccount++] = 1.0, rectangle_vertices[verccount++] = -1.0, rectangle_vertices[verccount++] = 0.0;
    rectangle_vertices[verccount++] = 1.0, rectangle_vertices[verccount++] = 1.0, rectangle_vertices[verccount++] = 0.0;
    rectangle_vertices[verccount++] = -1.0, rectangle_vertices[verccount++] = 1.0, rectangle_vertices[verccount++] = 0.0;

    //actually memcopy the data - only do this once
    glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(float), rectangle_vertices, GL_STATIC_DRAW);

    //we need to set up the vertex array
    glEnableVertexAttribArray(0);
    //key function to get up how many elements to pull out at a time (3)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);



    //generate vertex buffer to hand off to OGL
    glGenBuffers(1, &VertexBufferTex);

    //set the current state to focus on our vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, VertexBufferTex);

    float t = 1. / 100.;

    GLfloat* rectangle_texture_coords = new GLfloat[12];
    int texccount = 0;

    rectangle_texture_coords[texccount++] = 0, rectangle_texture_coords[texccount++] = 0;
    rectangle_texture_coords[texccount++] = 1, rectangle_texture_coords[texccount++] = 0;
    rectangle_texture_coords[texccount++] = 0, rectangle_texture_coords[texccount++] = 1;
    rectangle_texture_coords[texccount++] = 1, rectangle_texture_coords[texccount++] = 0;
    rectangle_texture_coords[texccount++] = 1, rectangle_texture_coords[texccount++] = 1;
    rectangle_texture_coords[texccount++] = 0, rectangle_texture_coords[texccount++] = 1;

    //actually memcopy the data - only do this once
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), rectangle_texture_coords, GL_STATIC_DRAW);

    //we need to set up the vertex array
    glEnableVertexAttribArray(2);

    //key function to get up how many elements to pull out at a time (3)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);


    blendProgram->addAttribute("vertPos");
    blendProgram->addAttribute("vertTex");

    int width, height;
    blendProgram->bind();
    glfwGetFramebufferSize(window, &width, &height);

    glGenFramebuffers(1, &fb);
    glBindFramebuffer(GL_FRAMEBUFFER, fb);


    // Generate Color Texture
    glGenTextures(1, &FBOcol1);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, FBOcol1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_BYTE, NULL);

    // Generate Position Texture
    glGenTextures(1, &FBOcol2);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, FBOcol2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
    glGenerateMipmap(GL_TEXTURE_2D);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, FBOcol1, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, FBOcol2, 0);

    glGenRenderbuffers(1, &depth_rb);
    glBindRenderbuffer(GL_RENDERBUFFER, depth_rb);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);

    //-------------------------

    //Attach depth buffer to FBO
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_rb);

    int Tex1Loc = glGetUniformLocation(blendProgram->getPID(), "color1");
    int Tex2Loc = glGetUniformLocation(blendProgram->getPID(), "color2");
    glUniform1i(Tex1Loc, 0);
    glUniform1i(Tex2Loc, 1);
}

void PointCloudRenderer::Delete()
{
    if (!m_initialized)
    {
        return;
    }

    m_initialized = false;

    glDeleteBuffers(1, &m_vertexBufferObject);
    glDeleteBuffers(1, &m_vertexTrans);
    glDeleteBuffers(1, &m_outlineBuffer);
    glDeleteBuffers(1, &billboard_position_buffer);
    glDeleteBuffers(1, &billboard_texture_buffer);


    VSFSProgram->Delete();
}

void PointCloudRenderer::InitializeDepthXYTable(const float* xyTableInterleaved, uint32_t width, uint32_t height)
{
    m_width = width;
    m_height = height;

    glGenTextures(1, &m_xyTableTextureObject);
    glBindTexture(GL_TEXTURE_2D, m_xyTableTextureObject);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RG32F, m_width, m_height);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, GL_RG, GL_FLOAT, xyTableInterleaved);

    glGenTextures(1, &m_depthTextureObject);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void PointCloudRenderer::InitializeSSBO()
{

    ssBuffObject->create_SSBO<PointCloudRenderer::ssbo_data>(&ssbo_CPUMEM, sizeof(ssbo_CPUMEM));
    atomicCounterBuff->bind();
    atomicCounterBuff->bufferData(sizeof(GLuint) * 1, (const void *) 0);
    atomicCounterBuff->unbind();

}
//
//void PointCloudRenderer::addColor(glm::vec4 color, bool in_point_cloud)
//{
//    if (in_point_cloud)
//        colorIndex[ssbo_index] = 1;
//    else
//        colorIndex[ssbo_index] = 0;
//
//    ssbo_CPUMEM.colorInput[ssbo_index] = glm::vec4(color.x, color.y, color.z, color.a);
//    ssbo_CPUMEM.colorOutput[ssbo_index] = glm::vec4(0,0,0,1);
//    ssbo_index++;
//}
//
//bool PointCloudRenderer::pixelDying(int pixelIndex)
//{
//    return ssbo_CPUMEM.pixelLife[pixelIndex].x <= 0.0f;
//}

void PointCloudRenderer::update(float frametime)
{
    m_elapsedTime += frametime;
}

void PointCloudRenderer::UpdatePointClouds(
    GLFWwindow* window,
    PointCloudVertex* point3ds,
    uint32_t numPoints,
    const uint16_t* depthFrame,
    uint32_t width, uint32_t height, 
    bool drawOnlyPointCloudOutline,
    bool useTestPointClouds)
{
    if (window != m_window)
    {
        Create(window);
    }

    if (m_width != width && m_height != height)
    {
        Fail("Width and Height (%u, %u) does not match the DepthXYTable settings: (%u, %u) are expected!", width, height, m_width, m_height);
    }

    glBindImageTexture(0, m_xyTableTextureObject, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RG32F);

    glBindTexture(GL_TEXTURE_2D, m_depthTextureObject);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_R16UI, m_width, m_height);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, GL_RED_INTEGER, GL_UNSIGNED_SHORT, depthFrame);
    glBindImageTexture(1, m_depthTextureObject, 0, GL_FALSE, 0, GL_READ_ONLY, GL_R16UI);

    std::vector<glm::mat4> transformations;

    if (drawOnlyPointCloudOutline)
    {
        //pointCloudOutline.clear();
        for (int i = 0; i < numPoints; i++)
            ssbo_CPUMEM.colorInput[i] = vec4(point3ds[i].Color.x, point3ds[i].Color.y, point3ds[i].Color.z, point3ds[i].Color.a);

        ssBuffObject->load_SSBO<PointCloudRenderer::ssbo_data>(&ssbo_CPUMEM, 0, sizeof(ssbo_CPUMEM));

        computeProgram->startUpload();

        ssBuffObject->bindBufferBase(0);
        computeProgram->bind();

        //activate atomic counter
        atomicCounterBuff->bind();
        atomicCounterBuff->bindBufferBase(0);
        computeProgram->dispatch();
        ssBuffObject->unbindBufferBase(0);
        computeProgram->unbind();


        ssBuffObject->get_SSBO_back<PointCloudRenderer::ssbo_data>(&ssbo_CPUMEM, sizeof(ssbo_CPUMEM));
        atomicCounterBuff->read_atomic();
        atomicCounterBuff->reset_atomic();

        int outlineIndex = 0;
        int i = 0;
        /*int randNumAnimating = rand() % 10;
        std::vector<int> randPixIndices;
        if(ssbo_CPUMEM.last_index>0 && resetAnimators)
            for (int j = 0; j < randNumAnimating; j++)
                randPixIndices.push_back(rand() % ssbo_CPUMEM.last_index);*/

       
        while (outlineIndex >= 0 && numPoints>0)
        {
            outlineIndex = ssbo_CPUMEM.outlineIndices[i];

            if (resetAnimators)
            {
                PointCloudVertex pointCloud;

                pointCloud.Color = point3ds[outlineIndex].Color;
                pointCloud.Position = point3ds[outlineIndex].Position;
                pointCloud.PixelLocation = point3ds[outlineIndex].PixelLocation;
                float scale = ((rand() % 390) + 10) / 100.0f;
                pointCloud.Transformations = glm::translate(mat4(1), pointCloud.Position) * glm::rotate(glm::mat4(1), (float)glm::radians((rand() % 360) / 1.0f), glm::vec3(0, 0, 1)) * glm::scale(mat4(1), glm::vec3(scale));
                pointCloud.Animate = 1;
                animatingPixels.push_back(pointCloud);
                transformations.push_back(pointCloud.Transformations);
            }
            else
            {
                float scale = ((rand() % 390) + 10) / 100.0f;
                point3ds[outlineIndex].Transformations = glm::translate(mat4(1), point3ds[outlineIndex].Position) * glm::rotate(glm::mat4(1), (float)glm::radians((rand() % 360) / 1.0f), glm::vec3(0, 0, 1)) * glm::scale(mat4(1), glm::vec3(scale));
                point3ds[outlineIndex].Animate = 0;

            }
                
       
            ssbo_CPUMEM.outlineIndices[i] = -1;
            i++;
        }

    }

    for (int i = 0; i < numPoints; i++)
        transformations.push_back(point3ds[i].Transformations);
    if (!resetAnimators)
        for (int i = 0; i < animatingPixels.size(); i++)
            transformations.push_back(animatingPixels.at(i).Transformations);

    glBindVertexArray(m_vertexArrayObject);
    // Create buffers and bind the geometry
  
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, billboard_position_buffer);
    glVertexAttribPointer(
        0, // attribute. No particular reason for 0, but must match the layout in the shader.
        3, // size
        GL_FLOAT, // type
        GL_FALSE, // normalized?
        0, // stride
        (void*)0 // array buffer offset
    );
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, billboard_texture_buffer);
    glVertexAttribPointer(
        1, // attribute. No particular reason for 0, but must match the layout in the shader.
        2, // size
        GL_FLOAT, // type
        GL_FALSE, // normalized?
        0, // stride
        (void*)0 // array buffer offset
    );


    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);

  /*  if (!drawOnlyPointCloudOutline)
    {*/
        if (!useTestPointClouds)
        {
            if (animatingPixels.size() > 0)
            {
                glBufferData(GL_ARRAY_BUFFER, numPoints * animatingPixels.size() * sizeof(PointCloudVertex), NULL, GL_STREAM_DRAW);
                glBufferSubData(GL_ARRAY_BUFFER, 0, numPoints * sizeof(PointCloudVertex), point3ds);
                glBufferSubData(GL_ARRAY_BUFFER, numPoints * sizeof(PointCloudVertex), animatingPixels.size() * sizeof(PointCloudVertex), animatingPixels.data());
            }
            else
                glBufferData(GL_ARRAY_BUFFER, numPoints * sizeof(PointCloudVertex), point3ds, GL_STREAM_DRAW);

        }
        else
        {
            glBufferData(GL_ARRAY_BUFFER, sizeof(testVertices), testVertices, GL_STREAM_DRAW);
        }
   // }
   // else {
   //     allPixelsInOutline.clear();
   //     allPixelsInOutline.reserve(numPoints + animatingPixels.size()); // preallocate memory
   //     allPixelsInOutline.insert(allPixelsInOutline.end(), point3ds.begin(), point3ds.end());
   //     allPixelsInOutline.insert(allPixelsInOutline.end(), animatingPixels.begin(), animatingPixels.end());
   //     glBufferData(GL_ARRAY_BUFFER, allPixelsInOutline.size() * sizeof(PointCloudVertex), allPixelsInOutline.data(), GL_STREAM_DRAW);
   ///*     for (int i = 0; i < allPixelsInOutline.size(); i++)
   //         transformations.push_back(allPixelsInOutline.at(i).Transformations);*/
   // }


    
    // Set the vertex attribute pointers
    // Vertex Positions
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(PointCloudVertex), (void*)0);
    //animate
    glEnableVertexAttribArray(3);
    glVertexAttribIPointer(3, 1, GL_INT, sizeof(PointCloudVertex), (void*)offsetof(PointCloudVertex, Animate));
    // Vertex Colors
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(PointCloudVertex), (void*)offsetof(PointCloudVertex, Color));
    //// Vertex transformations
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexTrans);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4)*transformations.size(), transformations.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4*sizeof(glm::vec4), (void*)0);
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(1 * sizeof(glm::vec4)));
    glEnableVertexAttribArray(7);
    glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(2 * sizeof(glm::vec4)));
    glEnableVertexAttribArray(8);
    glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(3 * sizeof(glm::vec4)));

    //// Vertex Pixel Location
    //// Notice: For GL_INT type, we need to use glVertexAttribIPointer instead of glVertexAttribPointer
    //glEnableVertexAttribArray(6);
    //glVertexAttribIPointer(6, 2, GL_INT, sizeof(PointCloudVertex), (void*)offsetof(PointCloudVertex, PixelLocation));

    glBindVertexArray(0);

    m_drawArraySize = useTestPointClouds ? 8 : GLsizei(numPoints);
    m_drawArraySize = drawOnlyPointCloudOutline ? (m_drawArraySize + animatingPixels.size()) : m_drawArraySize;
}

void PointCloudRenderer::SetShading(bool enableShading)
{
    m_enableShading = enableShading;
}

void PointCloudRenderer::Render()
{
    std::array<int, 4> data; // x, y, width, height

    glGetIntegerv(GL_VIEWPORT, data.data());
    Render(nullptr, data[2], data[3]);
}

void PointCloudRenderer::Render(SceneComponent* scene, int width, int height)
{
    glEnable(GL_DEPTH_TEST);
    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindFramebuffer(GL_FRAMEBUFFER, fb);

    GLenum buffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};

    glDrawBuffers(2, buffers);


    float pointSize;
    if (m_pointCloudSize)
    {
        pointSize = m_pointCloudSize.value();
    }
    else if (m_width == 0 || m_height == 0)
    {
        pointSize = m_defaultPointCloudSize;
    }
    else
    {
        pointSize = std::min(2.f * width / (float)m_width, 2.f * height / (float)m_height);
    }
    //glPointSize(pointSize); //used for drawing points ----> GL_POINTS ----> glDrawArrays(GL_POINTS, 0, m_drawArraySize);

    //glUseProgram(m_shaderProgram);
    VSFSProgram->bind();

    // Update model/view/projective matrices in shader

    glUniformMatrix4fv(VSFSProgram->getUniform("view"), 1, GL_FALSE, &m_view[0][0]);
    glUniformMatrix4fv(VSFSProgram->getUniform("projection"), 1, GL_FALSE, &m_projection[0][0]);

   /* glm::mat4 model = glm::scale(glm::mat4(1), glm::vec3(1.0f, 1.0f, 1.0f));
    glUniformMatrix4fv(VSFSProgram->getUniform("model"), 1, GL_FALSE, &model[0][0]);*/

    // Update render settings in shader
    glUniform1i(VSFSProgram->getUniform("enableShading"), (GLint)m_enableShading);


    if(scene!=nullptr)
        scene->draw(VSFSProgram);

    // Render point cloud
    glBindVertexArray(m_vertexArrayObject);
    glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
    glVertexAttribDivisor(1, 0); // particles texture coords : always reuse the same 2 coords -> 0
    glVertexAttribDivisor(2, 1); // positions : one per quad (its center) -> 1
    glVertexAttribDivisor(3, 1); // start animation : one per quad -> 1
    glVertexAttribDivisor(4, 1); // color : one per quad -> 1
    glVertexAttribDivisor(5, 1); // particle transformations : one per quad -> 1
    glVertexAttribDivisor(6, 1); // particle transformations : one per quad -> 1
    glVertexAttribDivisor(7, 1); // particle transformations : one per quad -> 1
    glVertexAttribDivisor(8, 1); // particle transformations : one per quad -> 1
    //glVertexAttribDivisor(9, 1); // pixel location : one per quad -> 1

    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, m_drawArraySize);
    glBindVertexArray(0);
    
    VSFSProgram->unbind();

    // Save output to framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, FBOcol1);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, FBOcol2);
    glGenerateMipmap(GL_TEXTURE_2D);

    resetAnimators = false;

    if (scene->getTileProgress() == 0)
    {
        resetAnimators = true;
        animatingPixels.clear();
    }



    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    blendProgram->bind();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, FBOcol1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, FBOcol2);

    glBindVertexArray(VertexArrayIDBox);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    blendProgram->unbind();

}

void PointCloudRenderer::ChangePointCloudSize(float pointCloudSize)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    m_pointCloudSize = pointCloudSize;
}

