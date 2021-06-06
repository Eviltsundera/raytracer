#ifndef RT_MYGEOMETRY_H
#define RT_MYGEOMETRY_H

#include <cstdlib>
#include <cmath>
#include <limits>

constexpr double EPS = 0.0001;
constexpr int refComplexity = 4;
constexpr int MAX_DIST = 1000;

template<class T>
class Triple {
    T point[3];
public:
    Triple(T x = T(), T y = T(), T z = T()) {
        point[0] = x;
        point[1] = y;
        point[2] = z;
    }

    T &operator[](size_t i) {
        return point[(i < 0) ? 0 : i];
    }

    const T &operator[](size_t i) const {
        return point[(i < 0) ? 0 : i];
    }

    double length() const {
        return sqrt(point[0] * point[0] + point[1] * point[1] + point[2] * point[2]);
    }

    Triple<T> &normalize(T l = 1) {
        *this = (*this) * (l / length());
        return *this;
    }

    Triple<T> normalized(T l = 1) const {
        return (*this) * (l / length());
    }

    Triple<T> reflect(const Triple<T> &N) const {
        return *this - N * 2.0 * (*this * N);
    }

    Triple<T> refract(const Triple<T> &N, const double refIdx) const {
        // Snell's law
        double cos = -std::max(-1.0, std::min(1.0, *this * N));
        double n1 = 1, n2 = refIdx;
        Triple<T> n = N;
        if (cos < 0) {
            cos *= -1;
            std::swap(n1, n2);
            n = n * -1;
        }
        double d = n1 / n2;
        double k = 1.1 - d * d * (1 - cos * cos);
        if (k < 0) {
            return Triple<T>();
        }
        return *this * d + n * (d * cos - sqrt(k));
    }

    T operator*(const Triple<T> &right) const {
        T result = T();
        for (size_t i = 0; i < 3; ++i) {
            result += point[i] * right[i];
        };
        return result;
    }

    Triple<T> operator+(const Triple<T> &right) const {
        Triple<T> result;
        for (size_t i = 0; i < 3; ++i) {
            result[i] = point[i] + right[i];
        }
        return result;
    }

    Triple<T> operator-(const Triple<T> &right) const {
        Triple<T> result;
        for (size_t i = 0; i < 3; ++i) {
            result[i] = point[i] - right[i];
        }
        return result;
    }

    template<typename C>
    Triple<T> operator*(const C &constant) const {
        Triple<T> result;
        for (size_t i = 0; i < 3; ++i) {
            result[i] = point[i] * constant;
        }
        return result;
    }

    Triple<T> operator-() const {
        return *this * T(-1);
    }
};

using Point = Triple<double>;
using Colour = Triple<double>;
using ReflectionParams = Triple<double>;
using RefractionParams = Triple<double>;
using DiffusiveParams = Triple<double>;
using Pixel = Triple<double>;

class Light {
    Point p;
    double intensity;

public:
    Light(const Point &position, double intensity) : p(position), intensity(intensity) {
    }

    Point getPosition() const {
        return p;
    }

    double getIntensity() const {
        return intensity;
    }
};

struct Material {
    ReflectionParams reflectionParams;
    DiffusiveParams diffusiveParams;
    double specularParam;
    double refractiveParam;
    double refractiveIndex;

    Material(const ReflectionParams &reflect = {1, 0, 0}, const DiffusiveParams &diffuse = {}, double specular = {},
             double refraction = {}, double refIdx = {}) :
        reflectionParams(reflect),
        diffusiveParams(diffuse),
        specularParam(specular),
        refractiveParam(refraction),
        refractiveIndex(refIdx) {
    }
};

class BasicObject {
public:
    virtual Material getMaterial(Point &p) const = 0;

    virtual Point getNormal(Point &p) const = 0;

    virtual bool areIntersected(const Point &beamPoint, const Point &direction, double &t0) const = 0;
};

class Sphere : public BasicObject {
    Point center;
    double radius;
    Material material;
public:
    Sphere(const Point &c, double r, const Material &mat) : center(c), radius(r), material(mat) {}

    bool areIntersected(const Point &beamPoint, const Point &direction, double &t0) const {
        Point vB2C = center - beamPoint;
        double posB2C = vB2C * direction;
        double d2 = vB2C * vB2C - posB2C * posB2C;
        if (d2 > radius * radius) {
            return false;
        }
        double dist = sqrt(radius * radius - d2);
        t0 = posB2C - dist;
        double t1 = posB2C + dist;
        if (t0 < 0) {
            t0 = t1;
        }
        return t0 >= 0;
    }

    Point getNormal(Point &p) const {
        return (p - center).normalized();
    }

    Material getMaterial(Point &p) const {
        return material;
    }
};

class Plane : public BasicObject {
    Point normal;
    Point point;
    Material material1;
    Material material2;
public:
    Plane(const Point &point, const Point &normal, const Material &mat1, const Material &mat2) :
        normal(normal.normalized()), point(point), material1(mat1), material2(mat2) {
    }

    Material getMaterial(Point &p) const {
        return (((int) (0.5 * p[0] + 1000) + (int) (0.5 * p[2])) % 2) ? material1 : material2;
    }

    Point getNormal(Point &p) const {
        return normal;
    }

    bool areIntersected(const Point &beamPoint, const Point &direction, double &t0) const {
        if (std::abs(direction * normal) > EPS / 100) {
            double plane_dist = -((beamPoint - point) * normal) / (direction * normal);
            if (plane_dist > 0) {
                t0 = plane_dist;
                return true;
            } else {
                return false;
            }
        }
        return false;
    }
};

class Triangle : public BasicObject {
    Point p0, p1, p2;
    Material material;

    Point helpNormal(Point v1, Point v2) const {
        return Point(v1[1] * v2[2] - v1[2] * v2[1], v1[2] * v2[0] - v1[0] * v2[2], v1[0] * v2[1] - v1[1] * v2[0]);
    }
public:
    Triangle(const Point &p0 = {}, const Point &p1 = {}, const Point &p2 = {}, const Material &mat = {}) :
        p0(p0), p1(p1), p2(p2), material(mat) {
    }

    Material getMaterial(Point &p) const {
        return material;
    }

    Point getNormal(Point &p) const {
        return helpNormal(p1 - p0, p2 - p0).normalized();
    }

    bool areIntersected(const Point &beamPoint, const Point &direction, double &t0) const {
        Point e1 = p1 - p0, e2 = p2 - p0;
        Point v1 = helpNormal(direction, e2);
        double d = e1 * v1;
        if (std::abs(d) < EPS) {
            return false;
        }
        Point v2 = beamPoint - p0;
        double u = v2 * v1;
        if (u < 0 || u > d) {
            return false;
        }
        Point v3 = helpNormal(v2, e1);
        double v = direction * v3;
        if (v < 0 || u + v > d) {
            return false;
        }
        t0 = e2 * v3 * (1.0 / d);
        return true;
    }
};


#endif //RT_MYGEOMETRY_H