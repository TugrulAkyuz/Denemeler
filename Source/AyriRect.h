/*
  ==============================================================================

    AyriRect.h
    Created: 9 Sep 2021 9:36:06pm
    Author:  Tuğrul Akyüz

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

using namespace juce::gl;
using namespace juce;

class AyriRect : public juce::Component,
    public juce::OpenGLRenderer,public juce::Timer
{
    
    public :
    AyriRect()
    {
        
       // startTimer(33);
       // setSize (40, 60);
       
       // openGLContext.setOpenGLVersionRequired (OpenGLContext::OpenGLVersion::openGL3_2);
        // Indicates that no part of this Component is transparent.
        setOpaque(true);

        // Set this instance as the renderer for the context.
        openGLContext.setRenderer(this);

        // Tell the context to repaint on a loop.
        openGLContext.setContinuousRepainting(true);
        openGLContext.setComponentPaintingEnabled(true);

        // Finally - we attach the context to this Component.
        openGLContext.attachTo(*this);
        startTimer(20);
     
        
    }
    ~AyriRect()
    {
        openGLContext.detach();
    }

    void timerCallback()
    {
        vertexBuffer[1].position[1] += 0.01;
        openGLContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, vbo);
        openGLContext.extensions.glBufferData(
            GL_ARRAY_BUFFER,                        // The type of data we're sending.
            sizeof(Vertex) * vertexBuffer.size(),   // The size (in bytes) of the data.
            vertexBuffer.data(),                    // A pointer to the actual data.
            GL_STREAM_DRAW                          // How we want the buffer to be drawn.
        );

    }

    void  newOpenGLContextCreated() override
    {
        // Generate 1 buffer, using our vbo variable to store its ID.
        openGLContext.extensions.glGenBuffers(1, &vbo);
        
        // Generate 1 more buffer, this time using our IBO variable.
        openGLContext.extensions.glGenBuffers(1, &ibo);
        // Create 4 vertices each with a different colour.
        vertexBuffer = {
            // Vertex 0
            {
                { -0.5f, 0.5f , 0},        // (-0.5, 0.5)
                { 1.f, 0.f, 0.f, 1.f }  // Red
            },
            // Vertex 1
            {
                { 1.0f, 0.5f ,0.0f},         // (0.5, 0.5)
                { 1.f, 0.5f, 0.f, 1.f } // Orange
            },
            // Vertex 2
            {
                { 0.5f, -0.5f ,0},        // (0.5, -0.5)
                { 1.f, 1.f, 0.f, 1.f }  // Yellow
            },
            // Vertex 3
            {
                { -0.5f, -0.5f , 0},       // (-0.5, -0.5)
                { 1.f, 0.f, 1.f, 1.f }  // Purple
            }
        };
        
        // We need 6 indices, 1 for each corner of the two triangles.
        indexBuffer = {
            0, 1, 2,
            0, 2, 3
        };
        
        openGLContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, vbo);
        
        // Send the vertices data.
        openGLContext.extensions.glBufferData(
            GL_ARRAY_BUFFER,                        // The type of data we're sending.
            sizeof(Vertex) * vertexBuffer.size(),   // The size (in bytes) of the data.
            vertexBuffer.data(),                    // A pointer to the actual data.
            GL_STREAM_DRAW                          // How we want the buffer to be drawn.
        );
        
        // Bind the IBO.
        openGLContext.extensions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        
        // Send the indices data.
        openGLContext.extensions.glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            sizeof(unsigned int) * indexBuffer.size(),
            indexBuffer.data(),
            GL_STREAM_DRAW
        );
        
        
        vertexShader =
                R"(
            attribute vec4 a_Position;
            attribute vec4 a_Color;
            varying vec4 frag_Color;
            void main(){
                frag_Color = a_Color;
                gl_Position = a_Position;
            }
        )";

        fragmentShader =
                R"(
            varying vec4 frag_Color;
            void main()
            {
                gl_FragColor = frag_Color;
            }
        )";
        
        // Create an instance of OpenGLShaderProgram
        shaderProgram.reset(new OpenGLShaderProgram(openGLContext));
        
        // Compile and link the shader.
        // Each of these methods will return false if something goes wrong so we'll
        // wrap them in an if statement
        if (shaderProgram->addVertexShader(vertexShader)
            && shaderProgram->addFragmentShader(fragmentShader)
            && shaderProgram->link())
        {
            // No compilation errors - set the shader program to be active
            shaderProgram->use();
        }
        else
        {
            // Oops - something went wrong with our shaders!
            // Check the output window of your IDE to see what the error might be.
            jassertfalse;
        }
        
    }

    void  renderOpenGL() override
    {
        OpenGLHelpers::clear(Colours::black);
        shaderProgram->use();
        openGLContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, vbo);
        openGLContext.extensions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        int v_size = sizeof(Vertex);
        openGLContext.extensions.glVertexAttribPointer(
            0,              // The attribute's index (AKA location).
            3,              // How many values this attribute contains.
            GL_FLOAT,       // The attribute's type (float).
            GL_FALSE,       // Tells OpenGL NOT to normalise the values.
            v_size, // How many bytes to move to find the attribute with
                            // the same index in the next vertex.
            nullptr         // How many bytes to move from the start of this vertex
                            // to find this attribute (the default is 0 so we just
                            // pass nullptr here).
        );
        openGLContext.extensions.glEnableVertexAttribArray(0);
        
        // Enable to colour attribute.
        openGLContext.extensions.glVertexAttribPointer(
            1,                              // This attribute has an index of 1
            4,                              // This time we have four values for the
                                            // attribute (r, g, b, a)
            GL_FLOAT,
            GL_FALSE,
            sizeof(Vertex),
            (GLvoid*)(sizeof(float) * 3)    // This attribute comes after the
                                            // position attribute in the Vertex
                                            // struct, so we need to skip over the
                                            // size of the position array to find
                                            // the start of this attribute.
        );
        openGLContext.extensions.glEnableVertexAttribArray(1);
        int size = (int)indexBuffer.size();
        glDrawElements(
            GL_TRIANGLES,       // Tell OpenGL to render triangles.
            size, // How many indices we have.
            GL_UNSIGNED_INT,    // What type our indices are.
            nullptr             // We already gave OpenGL our indices so we don't
                                // need to pass that again here, so pass nullptr.
        );
        openGLContext.extensions.glDisableVertexAttribArray(0);
        openGLContext.extensions.glDisableVertexAttribArray(1);
    }

    void  openGLContextClosing() override
    {
    }
//    void timerCallback() override
//    {
//
//        t = (t+incr);
//        if(t > getWidth()) t = 0;
//
//        repaint();
//    }
//    void update() override
//    {
//                t = (t+incr);
//                if(t > getWidth()) t = 0;
//
//    }
    void paint(juce::Graphics &g) override
    {
//        g.fillAll (juce::Colour { 0xFF19191b });
//        g.setColour(juce::Colours::white.withAlpha(0.5f));
//        g.drawRect(getLocalBounds(),2);
//        g.drawVerticalLine(t, 0, getHeight());
        
//        juce::Path p ;
//        p.clear();
//        g.setColour(juce::Colours::red);
//        p.addArc(0,0,100,100,0,3.14,true);
//        g.strokePath(p,juce::PathStrokeType(3));

        
    }
    void resized() override
    {
        incr = (getWidth()/4)*33.0/5000;
    }
    
    float incr;
    float t = 0;
    juce::OpenGLContext openGLContext;
    struct Vertex
     {
         float position[3];
         float colour[4];
     };
    std::vector<Vertex> vertexBuffer;
    
    
    
    std::vector<unsigned int> indexBuffer;
    GLuint vbo; // Vertex buffer object.
    GLuint ibo; // Index buffer object.
    String vertexShader;
    String fragmentShader;
    
    std::unique_ptr<OpenGLShaderProgram> shaderProgram;
    
    
    
};
