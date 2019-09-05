#pragma once
//------------------------------------------------------------------------------
/**
	Application class used for example application.
	
	(C) 2015-2018 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "core/app.h"
#include "render/window.h"
#include "MeshResource.h"
#include "Matrix4.h"
#include "Vector4D.h"
namespace Example
{
    class ExampleApp : public Core::App
    {
    public:
        /// constructor
        ExampleApp();
        /// destructor
        ~ExampleApp();

        /// open app
        bool Open();
        /// run app
        void Run();
    private:


        const GLchar* vs =
                "#version 430\n"
                "layout(location=0) in vec3 pos;\n"
                "layout(location=1) in vec4 color;\n"
                "layout(location=0) out vec4 Color;\n"
                "uniform mat4 transform;\n"
                "void main()\n"
                "{\n"
                "	gl_Position = transform * vec4(pos, 1.0f);\n"
                "	Color = color;\n"
                "}\n";

        const GLchar* ps =
                "#version 430\n"
                "layout(location=0) in vec4 color;\n"
                "out vec4 Color;\n"
                "void main()\n"
                "{\n"
                "	Color = color;\n"
                "}\n";
        GLuint program;
        GLuint vertexShader;
        GLuint pixelShader;
        GLuint triangle;
        Display::Window* window;


    };
} // namespace Example
