#ifndef RENDER_TYPES_H
#define RENDER_TYPES_H

#include <QString>

enum RenderType {
    hardware = 0,
    gl = 1,
    gl_plus = 2
};

static QString renderTypeToStr(RenderType type) {
    switch(type) {
        case gl: return "gl + conversion";
        case gl_plus: return "gl + shaders";
        default: return "hardware";
    };
}

#endif // RENDER_TYPES_H
