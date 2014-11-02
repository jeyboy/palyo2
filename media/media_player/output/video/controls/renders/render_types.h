#ifndef RENDER_TYPES_H
#define RENDER_TYPES_H

#include <QString>

enum RenderType {
    none = -1,
    hardware,
    gl,
    gl_plus
};

static QString renderTypeToStr(RenderType type) {
    switch(type) {
        case gl: return "gl + conversion";
        case gl_plus: return "gl + shaders";
        default: return "hardware";
    };
}

#endif // RENDER_TYPES_H
