#pragma once
#include "Point.h"
#include <optional>
#include <vector>

struct Material {
    Pixel Ks{}, Kd{}, Ka{}, Ke{};
    double Ns = 0, Ni = 0, d = 1, Tr = -1, illum = 0;
};

enum Type{
    SPHERE,
    TRIANGLE
};

struct Object {
    Type type;
    Object(Type tp) : type(tp) {}
    virtual std::optional<Point> IsIntersected(const Point&, const Point&) const = 0;
    virtual Material GetMaterial() const = 0;
    virtual Point GetNormal(const Point&) const = 0;
};

struct Light{
    Point p;
    Pixel intensity;

    Light(const Point &position, Pixel intensity) : p(position), intensity(intensity) {
    }
};

struct ComplexPoint{
    Point pv{}, pvt{}, pvn{};
    ComplexPoint(const Point &p0={}, const Point& p1={}, const Point& p2={}) : pv(p0), pvt(p1), pvn(p2) {}
};

struct Objects {
    std::vector<Light> lights;
    std::vector<Object *> objects;
};