#pragma once
#include <random>
#include "Base.h"

struct Intersect {
    Point point;
    Object* object;
};

struct LightAnswer {
    Pixel intensity;
    Point direction;
};

std::optional<Intersect> IsIntersect(const Point& origin, const Point& direction,
                                     const Objects& objects) {
    std::optional<Intersect> res{std::nullopt};
    for (auto& object : objects.objects) {
        auto pp = object->IsIntersected(origin, direction);
        if (pp) {
            if (!res || (res.value().point - origin).Length() > (pp.value() - origin).Length()) {
                res = {pp.value(), object};
            }
        }
    }
    return res;
}

Pixel TraceRay(Point origin, Point direction, const Objects& objects, int depth,
               bool inside_sphere = false) {
    // Find point of intersection
    if (depth == 0) {
        return {};
    }

    std::optional<Intersect> data = IsIntersect(origin, direction, objects);
    if (!data) {
        return {};
    }

    auto obj_inter = data.value().object;
    Point p_inter = data.value().point;

    // COmputeIllumination
    if (!obj_inter) {
        throw std::runtime_error("ComputeIllumination Error! Obg_inter=nullptr!");
    }
    Material mlt = obj_inter->GetMaterial();

    Point norm = obj_inter->GetNormal(p_inter).Normalize();
    if (norm.IsCollinear(direction)) {
        norm = -norm;
    }
    std::vector<LightAnswer> lights;
    Pixel intense{};
    // Base
    for (const auto& light : objects.lights) {
        Point light_direction = (light.p - p_inter).Normalize();
        double light_dist = (light.p - p_inter).Length();
        Point light_origin = p_inter + norm * kEps;
        std::optional<Intersect> data1 = IsIntersect(light_origin, light_direction, objects);
        if (data1 && (data1.value().point - p_inter).Length() < light_dist) {
            continue;
        }
        lights.push_back({light.intensity, light_direction});
    }

    // Comp

    if (mlt.illum > 2) {
        if (!inside_sphere) {
            Point reflect_direction = direction.Reflect(norm).Normalize();
            if (!reflect_direction.IsCollinear(norm)) {
                throw "What a fuck boy2!";
            }
            Point reflect_origin = p_inter + norm * kEps;
            Pixel reflection_intense =
                TraceRay(reflect_origin, reflect_direction, objects, depth - 1);
            lights.push_back({reflection_intense, reflect_direction});
        }

        if (inside_sphere) {
            mlt.Tr = 1;
            mlt.Ni = 1 / mlt.Ni;
        } else {
            if (mlt.Tr == -1) {
                mlt.Tr = 1 - mlt.d;
            }
        }
        if (mlt.Tr) {
            Point refract_direction = direction.Refract(norm, mlt.Ni).Normalize();
            Point refract_origin = p_inter - norm * kEps;
            if (obj_inter->type == Type::SPHERE) {
                inside_sphere = !inside_sphere;
            }
            Pixel refraction_intense =
                TraceRay(refract_origin, refract_direction, objects, depth - 1, inside_sphere);
            intense += refraction_intense * mlt.Tr;
        }
    }

    Pixel light_diff_intensity = {}, light_spec_intensity = {};
    for (auto& light : lights) {
        light_diff_intensity += light.intensity * std::max(0., norm * light.direction);
        Point reflect = (-light.direction).Reflect(norm).Normalize();
        light_spec_intensity +=
            light.intensity * pow(std::max(0.0, reflect * (-direction)), mlt.Ns);
    }

    intense += mlt.Ka + mlt.Ke + MulChannel(mlt.Kd, light_diff_intensity) +
              MulChannel(mlt.Ks, light_spec_intensity);

    return intense;
}
