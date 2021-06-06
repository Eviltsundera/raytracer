#pragma once

#include <cmath>
const double kEps = 1e-9;

struct Point {
    double coord_x_=0, coord_y_=0, coord_z_=0;

    Point()
        : coord_x_(0), coord_y_(0), coord_z_(0) {
    }

    Point(double coord_x, double coord_y, double coord_z)
        : coord_x_(coord_x), coord_y_(coord_y), coord_z_(coord_z) {
    }

    double Length() const {
        return std::sqrt(coord_x_ * coord_x_ + coord_y_ * coord_y_ + coord_z_ * coord_z_);
    }

    Point Normalize() const  {
        if (Length() < kEps) {
            return *this;
        }
        return *this / Length();
    }

    Point Reflect(const Point &norm) const {
        return norm.Normalize() * 2.0 * std::abs(this->Normalize() * norm.Normalize()) + this->Normalize(); // TODO maybe
    }

    Point Refract(const Point &norm, const double ref_idx) const {

        double cos1 = std::abs(this->Normalize() * norm.Normalize()),
               sin1 = std::sqrt(1 - cos1 * cos1),
               sin2 = sin1 / ref_idx,
               cos2 = std::sqrt(1 - sin2 * sin2);

        Point d = this->Normalize();
        Point h1 = d + norm.Normalize()*cos1;
        Point h2 = h1 * (sin2 * cos1 / sin1 / cos2);
        Point res = -norm.Normalize()*cos1 + h2;
        return res;

        Point norm_sz = norm.Normalize() * cos1;
        Point direction = this->Normalize();
        Point v1 = direction + norm_sz;
         cos2 = std::sqrt(1 - (1 - cos1*cos1) / (ref_idx * ref_idx));
        auto cos_to_tg = [](double cos) {
            return sqrt(1 - cos * cos) / cos;
        };
        Point v2 = v1 * cos_to_tg(cos2) / cos_to_tg(cos1);
        return (-norm + v2).Normalize();
         /*
        double Ni = ref_idx;
        Point hit = *this;

        double cosine_first = hit * norm / hit.Normalize().Length();
        double sine_first_squared = 1 - cosine_first * cosine_first;
        Point parallel_part = (hit / hit.Normalize().Length() + norm * cosine_first) * Ni;
        Point perpendicular_part = -norm * std::sqrt(1 - Ni * Ni * sine_first_squared);
        return parallel_part + perpendicular_part;
         */
    }


    double operator*(const Point &other) const {
        return coord_x_ * other.coord_x_ + coord_y_ * other.coord_y_ + coord_z_ * other.coord_z_;
    }

    Point operator+(const Point &other) const {
        return {coord_x_ + other.coord_x_,
                coord_y_ + other.coord_y_,
                coord_z_ + other.coord_z_};
    }

    Point & operator+=(const Point& other) {
        *this = *this + other;
        return *this;
    }

    Point operator-(const Point &other) const {
        return *this + -other;
    }

    Point operator*(const double &constant) const {
        return {coord_x_ * constant,
                coord_y_ * constant,
                coord_z_ * constant};
    }
    Point operator/(const double &constant) const {
        return *this * (1 / constant);
    }

    Point operator-() const {
        return *this * double(-1);
    }

    bool IsCollinear(const Point& other) const {
        if (*this * other > 0) {
            return true;
        }
        return false;
    }

    bool IsParallel(const Point& other) const {
        if (std::abs(1 - std::abs(CosOfAngle(*this, other))) < kEps) {
            return true;
        }
        return false;
    }

    bool IsOrto(const Point& other) const {
        return std::abs(*this * other) < kEps;
    }

    operator bool() const {
        return !(coord_x_ == 0 && coord_y_ == 0 && coord_z_ == 0);
    }
    double CosOfAngle(Point v1, Point v2) const {
        return v1 * v2 / v1.Length() / v2.Length();
    }
};


Point VectorMul(Point v1, Point v2) {
    return Point(v1.coord_y_ * v2.coord_z_ - v1.coord_z_ * v2.coord_y_,
                 v1.coord_z_ * v2.coord_x_ - v1.coord_x_ * v2.coord_z_,
                 v1.coord_x_ * v2.coord_y_ - v1.coord_y_ * v2.coord_x_);
}

Point MulChannel(Point v1, Point v2) {
    return {v1.coord_x_ * v2.coord_x_,
            v1.coord_y_ * v2.coord_y_,
            v1.coord_z_ * v2.coord_z_};
}

Point DivideChannel(Point v1, Point v2) {
    return {v1.coord_x_ / v2.coord_x_,
            v1.coord_y_ / v2.coord_y_,
            v1.coord_z_ / v2.coord_z_};
}



Point GetNormal(Point p0, Point p1, Point p2) {
    return VectorMul(p1 - p0, p2 - p0); // TODO .Normalize()
}

Point Power(const Point& other, double st) {
    return {std::pow(other.coord_x_, st),
            std::pow(other.coord_y_, st),
            std::pow(other.coord_z_, st)};
}

using Pixel=Point;