#ifndef SHADER_H
#define SHADER_H

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

    void setup(int textures_count, bool planar = false, bool big_endian = false, bool little_endian = false) {
        program -> addShaderFromSourceCode(QGLShader::Vertex, vertexShader());
        program -> addShaderFromSourceCode(QGLShader::Fragment, fragmentShader(planar, big_endian, little_endian));

        bool res = program -> link();

        u_Texture.resize(textures_count);

        QString tex_var;
        for (int i = 0; i < u_Texture.size(); ++i) {
            tex_var = QString("u_Texture%1").arg(i);
            u_Texture[i] = program -> uniformLocation(tex_var);
        }

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
//                    "    highp float y = texture2D(u_Texture0, v_TexCoords).r;"
//                    "    highp float u = texture2D(u_Texture1, v_TexCoords).r;"
//                    "    highp float v = texture2D(u_Texture2, v_TexCoords).r;"

//                    "    y = 1.1643 * (y - 0.0625);"
//                    "    u = u - 0.5;"
//                    "    v = v - 0.5;"

//                    "    highp float r = y + 1.5958 * v;"
//                    "    highp float g = y - 0.39173 * u - 0.81290 * v;"
//                    "    highp float b = y + 2.017 * u;"

//                    "    gl_FragColor = vec4(r, g, b, 1.0);"


                    #if LA_16BITS
                        "float range = exp2(u_bpp) - 1.0;\n"
                        #if defined(LA_16BITS_LE)
                            "vec2 t = vec2(1.0, 256.0) * 255.0 / range;\n"
                        #else
                            "vec2 t = vec2(256.0, 1.0) * 255.0 / range;\n"
                        #endif
                    #endif //LA_16BITS // 10p in little endian: yyyyyyyy yy000000 => (L, L, L, A)
                        "gl_FragColor = clamp(u_colorMatrix\n"
                        "                     * vec4(\n"
                    #if LA_16BITS
                        "                         dot(texture2D(u_Texture0, v_TexCoords).ra, t),\n"
                        "                         dot(texture2D(u_Texture1, v_TexCoords).ra, t),\n"
                        "                         dot(texture2D(u_Texture2, v_TexCoords).ra, t),\n"
                    #else // use r, g, a to work for both yv12 and nv12. idea from xbmc
                        "                         texture2D(u_Texture0, v_TexCoords).r,\n"
                        "                         texture2D(u_Texture1, v_TexCoords).g,\n"
                        "                         texture2D(u_Texture2, v_TexCoords).a,\n"
                    #endif //LA_16BITS
                        "                         1)\n"
                        "                     , 0.0, 1.0);\n"
                    "}\n";
        } else {
            return
                    "uniform sampler2D u_Texture0;"
                    "varying vec2 v_TexCoords;"
                    "uniform mat4 u_colorMatrix;"
                    ""
                    "void main() {"
//                    "  gl_FragColor = texture2D(u_Texture0, v_TexCoords);"
                    "  gl_FragColor = u_colorMatrix * texture2D(u_Texture0, v_TexCoords);"
                    "}";
        }
    }

    QGLShaderProgram * program;

    GLint a_Position;
    GLint a_TexCoords;
    GLint u_matrix;
    GLint u_colorMatrix;
    GLuint u_bpp;

    QVector<GLint> u_Texture;
};

#endif // SHADER_H
