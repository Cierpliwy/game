#ifndef UTILS_H
#define UTILS_H
#include <glm/glm.hpp>

template<class T> struct Line
{
    Line(T a, T b) : a(a), b(b) {}
    T a,b;
};

template<class T> struct Rect
{
    Rect(T a,T b,T c,T d)
        : a(a), b(b), c(c), d(d) {}
    T a,b,c,d;
};

#endif //UTILS_H
