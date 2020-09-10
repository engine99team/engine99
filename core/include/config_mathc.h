#ifndef ENGINE99_CONFIG_MATHC_H
#define ENGINE99_CONFIG_MATHC_H

#if __APPLE__
#define GL_SILENCE_DEPRECATION
    #include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#define MATHC_USE_INT32
#define MATHC_FLOATING_POINT_TYPE GLfloat
#define MATHC_USE_DOUBLE_FLOATING_POINT

#endif //ENGINE99_CONFIG_MATHC_H
