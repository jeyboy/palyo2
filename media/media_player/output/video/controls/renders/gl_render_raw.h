#ifndef GL_RENDER_H
#define GL_RENDER_H

#include <QGLShaderProgram>
#include "media/media_player/output/video/controls/renders/render_interface.h"

class GLRenderRaw : public RenderInterface {
public:
    GLRenderRaw(QWidget* parent = NULL);
    ~GLRenderRaw();

    inline RenderType getRenderType() const { return gl_plus; }
    void resizeViewport(int w, int h);

    void repaint();

protected:
    void initializeGL();
    void paintGL();

    char *FProgram=
      "uniform sampler2DRect Ytex;\n"
      "uniform sampler2DRect Utex,Vtex;\n"
      "void main(void) {\n"
      "  float nx,ny,r,g,b,y,u,v;\n"
      "  vec4 txl,ux,vx;"
      "  nx=gl_TexCoord[0].x;\n"
      "  ny=576.0-gl_TexCoord[0].y;\n"
      "  y=texture2DRect(Ytex,vec2(nx,ny)).r;\n"
      "  u=texture2DRect(Utex,vec2(nx/2.0,ny/2.0)).r;\n"
      "  v=texture2DRect(Vtex,vec2(nx/2.0,ny/2.0)).r;\n"

      "  y=1.1643*(y-0.0625);\n"
      "  u=u-0.5;\n"
      "  v=v-0.5;\n"

      "  r=y+1.5958*v;\n"
      "  g=y-0.39173*u-0.81290*v;\n"
      "  b=y+2.017*u;\n"

      "  gl_FragColor=vec4(r,g,b,1.0);\n"
      "}\n";

//    static const char* YUV420P_VS = ""
//        "#version 330\n"
//        ""
//        "uniform mat4 u_pm;"
//        "uniform vec4 draw_pos;"
//        ""
//        "const vec2 verts[4] = vec2[] ("
//        " vec2(-0.5, 0.5), "
//        " vec2(-0.5, -0.5), "
//        " vec2( 0.5, 0.5), "
//        " vec2( 0.5, -0.5) "
//        ");"
//        ""
//        "const vec2 texcoords[4] = vec2[] ("
//        " vec2(0.0, 1.0), "
//        " vec2(0.0, 0.0), "
//        " vec2(1.0, 1.0), "
//        " vec2(1.0, 0.0) "
//        "); "
//        ""
//        "out vec2 v_coord; "
//        ""
//        "void main() {"
//        " vec2 vert = verts[gl_VertexID];"
//        " vec4 p = vec4((0.5 * draw_pos.z) + draw_pos.x + (vert.x * draw_pos.z), "
//        " (0.5 * draw_pos.w) + draw_pos.y + (vert.y * draw_pos.w), "
//        " 0, 1);"
//        " gl_Position = u_pm * p;"
//        " v_coord = texcoords[gl_VertexID];"
//        "}"
//        "";

//    static const char* YUV420P_FS = ""
//        "#version 330\n"
//        "uniform sampler2D y_tex;"
//        "uniform sampler2D u_tex;"
//        "uniform sampler2D v_tex;"
//        "in vec2 v_coord;"
//        "layout( location = 0 ) out vec4 fragcolor;"
//        ""
//        "const vec3 R_cf = vec3(1.164383, 0.000000, 1.596027);"
//        "const vec3 G_cf = vec3(1.164383, -0.391762, -0.812968);"
//        "const vec3 B_cf = vec3(1.164383, 2.017232, 0.000000);"
//        "const vec3 offset = vec3(-0.0625, -0.5, -0.5);"
//        ""
//        "void main() {"
//        " float y = texture(y_tex, v_coord).r;"
//        " float u = texture(u_tex, v_coord).r;"
//        " float v = texture(v_tex, v_coord).r;"
//        " vec3 yuv = vec3(y,u,v);"
//        " yuv += offset;"
//        " fragcolor = vec4(0.0, 0.0, 0.0, 1.0);"
//        " fragcolor.r = dot(yuv, R_cf);"
//        " fragcolor.g = dot(yuv, G_cf);"
//        " fragcolor.b = dot(yuv, B_cf);"
//        "}"
//        "";
private:
    QGLShaderProgram * fprogram;
};

#endif // GL_RENDER_H
