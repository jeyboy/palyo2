#ifndef RENDERS_H
#define RENDERS_H

#include "render_types.h"
#include "render_interface.h"
#include "hardware_render.h"
#include "gl_render.h"

enum RenderType {
    hardware = 0,
    gl = 1,
    gl_plus = 2
};

#endif // RENDERS_H
