#ifndef RENDER_TYPES_H
#define RENDER_TYPES_H

#include <QString>

enum RenderType {
    none = -1,
    stub,
    hardware,
    gl,
    gl_plus
};

static QString renderTypeToStr(RenderType type) {
    switch(type) {
        case stub: return "stub";
        case gl: return "gl + conversion";
        case gl_plus: return "gl + shaders";
        default: return "hardware";
    };
}

#endif // RENDER_TYPES_H
