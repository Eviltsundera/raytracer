#pragma once

#include <vector>
#include "Base.h"

struct Triangle : public Object {
    ComplexPoint p0, p1, p2;
    Material material_;

    Triangle(const ComplexPoint &p0={}, const ComplexPoint &p1={}, const ComplexPoint &p2={}, const Material &mat = {}, const Type type=Type::TRIANGLE) :
        Object(type), p0(p0), p1(p1), p2(p2), material_(mat) {
    }

    std::optional<Point> IsIntersected(const Point &origin, const Point &direction) const {
        Point norm = ::GetNormal(p0.pv, p1.pv, p2.pv).Normalize();
        if (norm.IsOrto(direction)) {
            return std::nullopt;
        }
        double tt = (p0.pv - origin) * norm / (norm * direction);
        if (tt < 0) {
            return std::nullopt;
        }
        Point p_inter = origin + direction.Normalize() * tt;
        double diff_sq = std::abs(GetSquare(*this) -
                                  GetSquare(Triangle{p_inter, p0.pv, p1.pv}) -
                                  GetSquare(Triangle{p_inter, p0.pv, p2.pv}) -
                                  GetSquare(Triangle{p_inter, p1.pv, p2.pv}));
        if (diff_sq < kEps) {
            return {p_inter};
        } else {
            return std::nullopt;
        }
    }

    double GetSquare(const Triangle& triangle) const {
        double sq = ::GetNormal(triangle.p0.pv,
                         triangle.p1.pv,
                         triangle.p2.pv).Length();
        return sq;
    }

    Material GetMaterial() const {
        return material_;
    }

    Point GetNormal(const Point &p_inter) const {
        if (!p0.pvn && !p1.pvn && !p2.pvn) {
            return ::GetNormal(this->p0.pv,
                               this->p1.pv,
                               this->p2.pv);
        } else {
            double s0 = GetSquare(Triangle{p_inter, p0.pv, p1.pv}),
                   s1 = GetSquare(Triangle{p_inter, p0.pv, p2.pv}),
                   s2 = GetSquare(Triangle{p_inter, p1.pv, p2.pv}),
                   s_all = GetSquare(Triangle{p0.pv, p1.pv, p2.pv});
            return (p0.pvn * s0 + p1.pvn * s1 + p2.pvn * s2) / s_all;
        }
    }
};



