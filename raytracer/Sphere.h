#pragma once

#include <optional>
#include "Base.h"

struct Sphere : public Object {
    Point center_;
    double radius_;
    Material material_;

public:
    Sphere(const Point &c, double r, const Material &mat = {}, const Type=Type::SPHERE)
        : Object(type), center_(c), radius_(r), material_(mat) {
    }

    virtual std::optional<Point> IsIntersected(const Point &origin, const Point &direction) const {
        Point dir_c = center_ - origin;
        bool inside = false;
        if (dir_c.Length() < radius_) {
            inside = true;
        }
        double cos_alpha = dir_c.CosOfAngle(dir_c, direction);
        double dist = dir_c.Length() * sqrt(1 - cos_alpha * cos_alpha);
        if (dist < radius_ + kEps) {
            Point dir1 = direction.Normalize() * dir_c.Length() * cos_alpha;
            Point v_in = dir_c - dir1;
            double len_to_inter = sqrt(radius_ * radius_ - v_in.Length() * v_in.Length());
            Point p_inter{};
            if (inside) {
                p_inter = origin + dir1 + dir1.Normalize() * len_to_inter;
            } else {
                p_inter = origin + dir1 - dir1.Normalize() * len_to_inter;
            }
            if ((p_inter - origin).IsCollinear(direction)) {
                return {p_inter};
            }
        }
        return std::nullopt;
    }

    Material GetMaterial() const {
        return material_;
    }

    Point GetNormal(const Point &p_inter) const {
        return p_inter - center_;
    }
};
