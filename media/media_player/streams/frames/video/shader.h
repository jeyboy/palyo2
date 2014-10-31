#ifndef SHADER_H
#define SHADER_H

//#if defined(YUV_MAT_GLSL)
////http://en.wikipedia.org/wiki/YUV calculation used
////http://www.fourcc.org/fccyvrgb.php
////GLSL: col first
//// use bt601
//    #if defined(CS_BT709)
//    "    const mat4 yuv2rgbMatrix = mat4(
//    "           1,      1,          1,      0,"
//    "           0,      -0.187,     1.8556, 0,"
//    "           1.5701, -0.4664,    0,      0,"
//    "           0,      0,          0,      1)"
//    #else //BT601
//    "    const mat4 yuv2rgbMatrix = mat4("
//    "           1,      1,      1,      0,"
//    "           0,      -0.344, 1.773,  0,"
//    "           1.403,  -0.714, 0,      0,"
//    "           0,      0,      0,      1)"
//    #endif
//    "    * mat4("
//    "           1,      0,      0,      0,"
//    "           0,      1,      0,      0,"
//    "           0,      0,      1,      0,"
//    "           0,      -0.5,   -0.5,   1);"
//#endif



#include <QGLShaderProgram>

struct Shader {

    Shader(const QGLContext * ctx) :
          program(new QGLShaderProgram(ctx))
          , a_Position(-1)
          , a_TexCoords(-1)
          , u_matrix(-1)
          , u_bpp(-1)
    {

    }

    ~Shader() { remove(); }

    void setup(bool planar = false, bool big_endian = false, bool little_endian = false) {
        qDebug() << fragmentShader(planar, big_endian, little_endian);
        program -> addShaderFromSourceCode(QGLShader::Vertex, vertexShader());
        program -> addShaderFromSourceCode(QGLShader::Fragment, fragmentShader(planar, big_endian, little_endian));

        bool res = program -> link();

        a_Position      = program -> attributeLocation("a_Position");
        a_TexCoords     = program -> attributeLocation("a_TexCoords");
        u_matrix        = program -> uniformLocation("u_MVP_matrix");
        u_bpp           = program -> uniformLocation("u_bpp");
        u_colorMatrix   = program -> uniformLocation("u_colorMatrix");

        if (!res) {
            qWarning("Failed to link shader program...%s", program -> log().toUtf8().constData());
        }
    }

    bool remove() {
        if (program) {
            program -> removeAllShaders();
            delete program;
            program = 0;
            return true;
        } else return false;
    }


    const char * vertexShader() {
        return
            "attribute vec4 a_Position;\n"
            "attribute vec2 a_TexCoords;\n"
            "uniform mat4 u_MVP_matrix;\n"
            "varying vec2 v_TexCoords;\n"
            "void main() {\n"
            "  gl_Position = u_MVP_matrix * a_Position;\n"
            "  v_TexCoords = a_TexCoords; \n"
            "}\n";
    }

    const char * fragmentShader(bool planar, bool big_endian, bool little_endian) {
        if (planar) {
            if (big_endian || little_endian) {
                #define LA_16BITS 1
            } else {
                #define LA_16BITS 0
            }

            //#ifdef GL_ES
            //// Set default precision to medium
            //precision mediump int;
            //precision mediump float;
            //#else
            //#define highp
            //#define mediump
            //#define lowp
            //#endif

            return
                    //// Set default precision to medium
                    "precision mediump int;\n"
                    "precision mediump float;\n"
                    "uniform sampler2D u_Texture0;\n "
                    "uniform sampler2D u_Texture1;\n "
                    "uniform sampler2D u_Texture2;\n "
                    "varying lowp vec2 v_TexCoords;\n"
                    "uniform float u_bpp;\n          "
                    "uniform mat4 u_colorMatrix;\n   "


                    // 10, 16bit: http://msdn.microsoft.com/en-us/library/windows/desktop/bb970578%28v=vs.85%29.aspx
                    "void main() {\n"
                    "    highp float y = texture2D(u_Texture0, v_TexCoords).r;"
                    "    highp float u = texture2D(u_Texture1, v_TexCoords).r;"
                    "    highp float v = texture2D(u_Texture2, v_TexCoords).r;"

                    "    y = 1.1643 * (y - 0.0625);"
                    "    u = u - 0.5;"
                    "    v = v - 0.5;"

                    "    highp float r = y + 1.5958 * v;"
                    "    highp float g = y - 0.39173 * u - 0.81290 * v;"
                    "    highp float b = y + 2.017 * u;"

                    "    gl_FragColor = vec4(r, g, b, 1.0);"


//                    #if LA_16BITS
//                        "float range = exp2(u_bpp) - 1.0;\n"
//                        #if defined(LA_16BITS_LE)
//                            "vec2 t = vec2(1.0, 256.0) * 255.0 / range;\n"
//                        #else
//                            "vec2 t = vec2(256.0, 1.0) * 255.0 / range;\n"
//                        #endif
//                    #endif //LA_16BITS // 10p in little endian: yyyyyyyy yy000000 => (L, L, L, A)
//                        "gl_FragColor = clamp(u_colorMatrix\n"
//                        "                     * vec4(\n"
//                    #if LA_16BITS
//                        "                         dot(texture2D(u_Texture0, v_TexCoords).ra, t),\n"
//                        "                         dot(texture2D(u_Texture1, v_TexCoords).ra, t),\n"
//                        "                         dot(texture2D(u_Texture2, v_TexCoords).ra, t),\n"
//                    #else // use r, g, a to work for both yv12 and nv12. idea from xbmc
//                        "                         texture2D(u_Texture0, v_TexCoords).r,\n"
//                        "                         texture2D(u_Texture1, v_TexCoords).g,\n"
//                        "                         texture2D(u_Texture2, v_TexCoords).a,\n"
//                    #endif //LA_16BITS
//                        "                         1)\n"
//                        "                     , 0.0, 1.0);\n"
                    "}\n";
        } else {
            return
                    "uniform sampler2D u_Texture0;"
                    "varying vec2 v_TexCoords;"
                    "uniform mat4 u_colorMatrix;"
                    ""
                    "void main() {"
                    "  gl_FragColor = texture2D(u_Texture0, v_TexCoords);"
//                    "  gl_FragColor = u_colorMatrix * texture2D(u_Texture0, v_TexCoords);"
                    "}";
        }
    }

    QGLShaderProgram * program;

    GLint a_Position;
    GLint a_TexCoords;
    GLint u_matrix;
    GLint u_colorMatrix;
    GLuint u_bpp;
};

#endif // SHADER_H


//struct Shader {

//    ~Shader() { remove(); }

//    bool setup() {
//        program = new QGLShaderProgram();

//        program -> addShaderFromSourceCode(QGLShader::Vertex, kVertexShader);
//        program -> addShaderFromSourceCode(QGLShader::Fragment, kFragmentShader);

////        a_Position = shader_program -> attributeLocation("a_Position");
////        a_TexCoords = shader_program -> attributeLocation("a_TexCoords");
////        u_matrix = shader_program -> uniformLocation("u_MVP_matrix");
////        u_bpp = shader_program -> uniformLocation("u_bpp");

////        u_colorMatrix = shader_program -> uniformLocation("u_colorMatrix");

//        position_object_ = program -> attributeLocation("vPosition");
//        texture_position_object_ = program -> attributeLocation("vTexCoord");

//        yuv_texture_object_[0] = glGetUniformLocation(program_object_, "yTexture");
//        check_gl_error("glGetUniformLocation");
//      yuv_texture_object_[1] = glGetUniformLocation(program_object_, "uTexture");
//        check_gl_error("glGetUniformLocation");
//        yuv_texture_object_[2] = glGetUniformLocation(program_object_, "vTexture");
//        check_gl_error("glGetUniformLocation");

//        return program -> link();
//    }

//    bool remove() {
//        if (program) {
//            program -> removeAllShaders();
//            delete program;
//            program = 0;
//            return true;
//        } else return false;
//    }

////    char * fragmentShader(bool planar = true, bool big_endian = true) {
////        if (planar) {
////            return
////                    "uniform sampler2D u_Texture0; "
////                    "uniform sampler2D u_Texture1; "
////                    "uniform sampler2D u_Texture2; "
////                    "varying lowp vec2 v_TexCoords;"
////                    "uniform float u_bpp;          "
////                    "uniform mat4 u_colorMatrix;   "



//////                    #if defined(LA_16BITS_BE) || defined(LA_16BITS_LE)
//////                    #define LA_16BITS 1
//////                    #else
//////                    #define LA_16BITS 0
//////                    #endif


////                    #if defined(YUV_MAT_GLSL)
////                    //http://en.wikipedia.org/wiki/YUV calculation used
////                    //http://www.fourcc.org/fccyvrgb.php
////                    //GLSL: col first
////                    // use bt601
////                        #if defined(CS_BT709)
////                        "    const mat4 yuv2rgbMatrix = mat4(
////                        "           1,      1,          1,      0,"
////                        "           0,      -0.187,     1.8556, 0,"
////                        "           1.5701, -0.4664,    0,      0,"
////                        "           0,      0,          0,      1)"
////                        #else //BT601
////                        "    const mat4 yuv2rgbMatrix = mat4("
////                        "           1,      1,      1,      0,"
////                        "           0,      -0.344, 1.773,  0,"
////                        "           1.403,  -0.714, 0,      0,"
////                        "           0,      0,      0,      1)"
////                        #endif
////                        "    * mat4("
////                        "           1,      0,      0,      0,"
////                        "           0,      1,      0,      0,"
////                        "           0,      0,      1,      0,"
////                        "           0,      -0.5,   -0.5,   1);"
////                    #endif

////                    // 10, 16bit: http://msdn.microsoft.com/en-us/library/windows/desktop/bb970578%28v=vs.85%29.aspx
////                    "void main() {"
////                        // FFmpeg supports 9, 10, 12, 14, 16 bits
////                    #if LA_16BITS
////                        "float range = exp2(u_bpp) - 1.0;"
////                        #if defined(LA_16BITS_LE)
////                            "vec2 t = vec2(1.0, 256.0) * 255.0 / range;"
////                        #else
////                            "vec2 t = vec2(256.0, 1.0) * 255.0 / range;"
////                        #endif
////                    #endif //LA_16BITS // 10p in little endian: yyyyyyyy yy000000 => (L, L, L, A)
////                        "gl_FragColor = clamp(u_colorMatrix"
////                        "                     * vec4("
////                    #if LA_16BITS
////                        "                         dot(texture2D(u_Texture0, v_TexCoords).ra, t),"
////                        "                         dot(texture2D(u_Texture1, v_TexCoords).ra, t),"
////                        "                         dot(texture2D(u_Texture2, v_TexCoords).ra, t),"
////                    #else // use r, g, a to work for both yv12 and nv12. idea from xbmc
////                        "                         texture2D(u_Texture0, v_TexCoords).r,"
////                        "                         texture2D(u_Texture1, v_TexCoords).g,"
////                        "                         texture2D(u_Texture2, v_TexCoords).a,"
////                    #endif //LA_16BITS
////                        "                         1)"
////                        "                     , 0.0, 1.0);"
////                    "}"
////        } else {
////            return
////                    "uniform sampler2D u_Texture0;"
////                    "varying vec2 v_TexCoords;"
////                    "uniform mat4 u_colorMatrix;"
////                    ""
////                    "void main() {"
////                    "  gl_FragColor = u_colorMatrix*texture2D(u_Texture0, v_TexCoords);"
////                    "}";
////        }
////    }

//    static const char kVertexShader[] =
//        "attribute vec4 vPosition;      \n"
//          "attribute vec2 vTexCoord;        \n"
//          "varying vec2 v_vTexCoord;        \n"
//        "void main() {                        \n"
//            "gl_Position = vPosition;       \n"
//            "v_vTexCoord = vTexCoord;       \n"
//        "}                                          \n";

//    static const char kFragmentShader[] =
//            "precision mediump float;               \n"
//            "varying vec2 v_vTexCoord;          \n"
//            "uniform sampler2D yTexture;        \n"
//            "uniform sampler2D uTexture;        \n"
//            "uniform sampler2D vTexture;        \n"
//            "void main() {                      \n"
//                "float y = texture2D(yTexture, v_vTexCoord).r;\n"
//                "float u = texture2D(uTexture, v_vTexCoord).r - 0.5;\n"
//                "float v = texture2D(vTexture, v_vTexCoord).r - 0.5;\n"
//                "float r = y + 1.13983 * v;\n"
//                "float g = y - 0.39465 * u - 0.58060 * v;\n"
//                "float b = y + 2.03211 * u;\n"
//                "gl_FragColor = vec4(r, g, b, 1.0);\n"
//            "}\n";


//    //    char *FProgram=
//    //      "uniform sampler2DRect Ytex;\n"
//    //      "uniform sampler2DRect Utex,Vtex;\n"
//    //      "void main(void) {\n"
//    //      "  float nx,ny,r,g,b,y,u,v;\n"
//    //      "  vec4 txl,ux,vx;"
//    //      "  nx=gl_TexCoord[0].x;\n"
//    //      "  ny=576.0-gl_TexCoord[0].y;\n"
//    //      "  y=texture2DRect(Ytex,vec2(nx,ny)).r;\n"
//    //      "  u=texture2DRect(Utex,vec2(nx/2.0,ny/2.0)).r;\n"
//    //      "  v=texture2DRect(Vtex,vec2(nx/2.0,ny/2.0)).r;\n"

//    //      "  y=1.1643*(y-0.0625);\n"
//    //      "  u=u-0.5;\n"
//    //      "  v=v-0.5;\n"

//    //      "  r=y+1.5958*v;\n"
//    //      "  g=y-0.39173*u-0.81290*v;\n"
//    //      "  b=y+2.017*u;\n"

//    //      "  gl_FragColor=vec4(r,g,b,1.0);\n"
//    //      "}\n";

//    //    static const char* YUV420P_VS = ""
//    //        "#version 330\n"
//    //        ""
//    //        "uniform mat4 u_pm;"
//    //        "uniform vec4 draw_pos;"
//    //        ""
//    //        "const vec2 verts[4] = vec2[] ("
//    //        " vec2(-0.5, 0.5), "
//    //        " vec2(-0.5, -0.5), "
//    //        " vec2( 0.5, 0.5), "
//    //        " vec2( 0.5, -0.5) "
//    //        ");"
//    //        ""
//    //        "const vec2 texcoords[4] = vec2[] ("
//    //        " vec2(0.0, 1.0), "
//    //        " vec2(0.0, 0.0), "
//    //        " vec2(1.0, 1.0), "
//    //        " vec2(1.0, 0.0) "
//    //        "); "
//    //        ""
//    //        "out vec2 v_coord; "
//    //        ""
//    //        "void main() {"
//    //        " vec2 vert = verts[gl_VertexID];"
//    //        " vec4 p = vec4((0.5 * draw_pos.z) + draw_pos.x + (vert.x * draw_pos.z), "
//    //        " (0.5 * draw_pos.w) + draw_pos.y + (vert.y * draw_pos.w), "
//    //        " 0, 1);"
//    //        " gl_Position = u_pm * p;"
//    //        " v_coord = texcoords[gl_VertexID];"
//    //        "}"
//    //        "";

//    //    static const char* YUV420P_FS = ""
//    //        "#version 330\n"
//    //        "uniform sampler2D y_tex;"
//    //        "uniform sampler2D u_tex;"
//    //        "uniform sampler2D v_tex;"
//    //        "in vec2 v_coord;"
//    //        "layout( location = 0 ) out vec4 fragcolor;"
//    //        ""
//    //        "const vec3 R_cf = vec3(1.164383, 0.000000, 1.596027);"
//    //        "const vec3 G_cf = vec3(1.164383, -0.391762, -0.812968);"
//    //        "const vec3 B_cf = vec3(1.164383, 2.017232, 0.000000);"
//    //        "const vec3 offset = vec3(-0.0625, -0.5, -0.5);"
//    //        ""
//    //        "void main() {"
//    //        " float y = texture(y_tex, v_coord).r;"
//    //        " float u = texture(u_tex, v_coord).r;"
//    //        " float v = texture(v_tex, v_coord).r;"
//    //        " vec3 yuv = vec3(y,u,v);"
//    //        " yuv += offset;"
//    //        " fragcolor = vec4(0.0, 0.0, 0.0, 1.0);"
//    //        " fragcolor.r = dot(yuv, R_cf);"
//    //        " fragcolor.g = dot(yuv, G_cf);"
//    //        " fragcolor.b = dot(yuv, B_cf);"
//    //        "}"
//    //        "";


////    static const GLfloat kVertexInformation[] =
////    {
////             -1.0f,  1.0f,           // TexCoord 0 top left
////             -1.0f, -1.0f,           // TexCoord 1 bottom left
////              1.0f, -1.0f,           // TexCoord 2 bottom right
////              1.0f,  1.0f            // TexCoord 3 top right
////    };
////    static const GLshort kTextureCoordinateInformation[] =
////    {
////              0, 0,         // TexCoord 0 top left
////              0, 1,         // TexCoord 1 bottom left
////              1, 1,         // TexCoord 2 bottom right
////              1, 0          // TexCoord 3 top right
////    };
//////    static const GLuint kStride = 0;//COORDS_PER_VERTEX * 4;
////    static const GLshort kIndicesInformation[] =
////    {
////        0, 1, 2,
////        0, 2, 3
////    };


//    QGLShaderProgram * program;
//};




