#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "Triangle.h"
#include "Sphere.h"
#include <memory>
#include <iostream>
#include <unordered_map>

struct Format {
    int v = 0, vt = 0, vn = 0;
};



class Parser {
private:
public:
    Parser(std::string filename) {
        std::string mtl_filename;
        {
            std::ifstream fin(filename);
            if (fin.is_open()) {
                std::string line;
                while (getline(fin, line)) {
                    text_obj_.emplace_back(line);
                    if (IsSubstring(line, "mtllib")) {
                        mtl_filename = std::string(line.begin() + 7, line.end());
                    }
                }
            } else {
                throw std::runtime_error("File .obj cant be opened!");
            }
        }
        if (!mtl_filename.empty()) {
            auto it = filename.end();
            while (it != filename.begin() && *prev(it) != '/') {
                --it;
            }
            mtl_filename = std::string(filename.begin(), it) + "/" + mtl_filename;
            std::ifstream fin(mtl_filename);
            if (fin.is_open()) {
                std::string line;
                while (getline(fin, line)) {
                    text_mtl_.emplace_back(line);
                }
            } else {
                throw std::runtime_error("File .mtl cant be opened! Filename = " + mtl_filename);
            }
        } else {
            throw std::runtime_error("No mtllib!");
        }
    }

    Objects RunParse() {
        return ParseText(text_obj_, text_mtl_);
    }

    static Objects ParseText(const std::vector<std::string> &text_obj,
                             const std::vector<std::string> &text_mtl) {
        std::vector<Point> points, points_t, points_n;
        Objects objects;
        size_t ind_mtl = 0;
        auto data_mtl = ParseMTLText(text_mtl);
        Material cur_mlt;
        for (auto &line : text_obj) {
            std::stringstream ss;
            ss << line;

            std::string arg0;
            ss >> arg0;
            if (arg0 == "#") {
                continue;

            } else if (arg0 == "v") {
                double xx, yy, zz;
                ss >> xx >> yy >> zz;
                xx = NormNumber(xx), yy = NormNumber(yy), zz = NormNumber(zz);
                points.emplace_back(Point{xx, yy, zz});

            } else if (arg0 == "vt") {
                double xx, yy, zz;
                ss >> xx >> yy >> zz;
                xx = NormNumber(xx), yy = NormNumber(yy), zz = NormNumber(zz);
                points_t.emplace_back(Point{xx, yy, zz});

            } else if (arg0 == "vn") {
                double xx, yy, zz;
                ss >> xx >> yy >> zz;
                xx = NormNumber(xx), yy = NormNumber(yy), zz = NormNumber(zz);
                points_n.emplace_back(Point{xx, yy, zz});

            } else if (arg0 == "f") {
                std::vector<Format> figures_format =
                    ExtractFormat(std::string(line.begin() + 1, line.end()));
                ComplexPoint top =
                                 GetPointFromFormat(figures_format[0], points, points_t, points_n),
                             cur =
                                 GetPointFromFormat(figures_format[1], points, points_t, points_n);

                for (size_t i = 2; i < figures_format.size(); ++i) {
                    ComplexPoint next =
                        GetPointFromFormat(figures_format[i], points, points_t, points_n);
                    objects.objects.push_back(new Triangle(top, cur, next, cur_mlt));
                    cur = next;
                }

            } else if (arg0 == "S") {
                double xx, yy, zz, rr;
                ss >> xx >> yy >> zz >> rr;
                xx = NormNumber(xx), yy = NormNumber(yy), zz = NormNumber(zz);
                objects.objects.push_back(new Sphere(Point(xx, yy, zz), rr, cur_mlt));

            } else if (arg0 == "P") {
                double xx, yy, zz, rr, gg, bb;
                ss >> xx >> yy >> zz >> rr >> gg >> bb;
                xx = NormNumber(xx), yy = NormNumber(yy), zz = NormNumber(zz);
                objects.lights.emplace_back(Light(Point(xx, yy, zz), Pixel{rr, gg, bb}));

            } else if (arg0 == "usemtl") {
                std::string key;
                ss >> key;
                cur_mlt = data_mtl[key];
            }
        }
        return objects;
    }

    static double NormNumber(double num) {
        return num + 0;
    }

    // CHECKED
    static ComplexPoint GetPointFromFormat(const Format &ft, std::vector<Point> points,
                                           std::vector<Point> points_t,
                                           std::vector<Point> points_n) {
        std::vector<Point> res;
        if (ft.v < 0) {
            res.push_back(points[static_cast<int>(points.size()) + ft.v]);
        } else if (ft.v > 0) {
            res.push_back(points[ft.v - 1]);
        } else {
            res.push_back({});
        }
        if (ft.vt < 0) {
            res.push_back(points_t[static_cast<int>(points_t.size()) + ft.vt]);
        } else if (ft.vt > 0) {
            res.push_back(points_t[ft.vt - 1]);
        } else {
            res.push_back({});
        }
        if (ft.vn < 0) {
            res.push_back(points_n[static_cast<int>(points_n.size()) + ft.vn]);
        } else if (ft.vn > 0) {
            res.push_back(points_n[ft.vn - 1]);
        } else {
            res.push_back({});
        }
        return {res[0], res[1], res[2]};
    }

    // CHECKED
    static std::vector<Format> ExtractFormat(const std::string &str) {
        std::vector<Format> res;
        size_t ind = 0;
        while (ind < str.length()) {
            if (str[ind] == ' ') {
                ++ind;
                continue;
            }
            Format ft;
            size_t ind_prev = ind;
            while (ind < str.length() && str[ind] != '/' && str[ind] != ' ') {
                ++ind;
            }
            ft.v = std::stod(std::string(str.begin() + ind_prev, str.begin() + ind));
            if (ind == str.length() || str[ind] == ' ') {
                res.emplace_back(ft);
                continue;
            }
            ++ind;
            if (str[ind] == '/') {
                ind_prev = ++ind;
                while (ind < str.length() && str[ind] != ' ') {
                    ++ind;
                }
                ft.vn = std::stod(std::string(str.begin() + ind_prev, str.begin() + ind));
                res.emplace_back(ft);
            } else {
                ind_prev = ind;
                while (ind < str.length() && str[ind] != '/' && str[ind] != ' ') {
                    ++ind;
                }
                ft.vt = std::stod(std::string(str.begin() + ind_prev, str.begin() + ind));
                if (ind == str.length() || str[ind] == ' ') {
                    res.emplace_back(ft);
                    continue;
                }
                ind_prev = ++ind;
                while (ind < str.length() && str[ind] != '/' && str[ind] != ' ') {
                    ++ind;
                }
                ft.vn = std::stod(std::string(str.begin() + ind_prev, str.begin() + ind));
                res.emplace_back(ft);
            }
        }
        return res;
    }

    // CHECKED
    static std::unordered_map<std::string, Material> ParseMTLText(
        std::vector<std::string> text_mtl) {
        std::string key;
        Material value;
        std::unordered_map<std::string, Material> res;

        size_t ind = 0;
        for (auto &line : text_mtl) {
            if (IsSubstring(line, "#")) {
                continue;
            } else if (IsSubstring(line, "newmtl")) {
                if (!key.empty()) {
                    res[key] = value;
                }
                std::stringstream ss;
                ss << line;
                ss.ignore(6, ' ') >> key;
                value = {};
            } else {
                std::stringstream ss;
                ss << line;
                std::string arg0;
                ss >> arg0;
                if (arg0 == "Ks") {
                    ss >> value.Ks.coord_x_ >> value.Ks.coord_y_ >> value.Ks.coord_z_;
                } else if (arg0 == "Kd") {
                    ss >> value.Kd.coord_x_ >> value.Kd.coord_y_ >> value.Kd.coord_z_;
                } else if (arg0 == "Ka") {
                    ss >> value.Ka.coord_x_ >> value.Ka.coord_y_ >> value.Ka.coord_z_;
                } else if (arg0 == "Ke") {
                    ss >> value.Ke.coord_x_ >> value.Ke.coord_y_ >> value.Ke.coord_z_;
                } else if (arg0 == "Tr") {
                    ss >> value.Tr;
                } else if (arg0 == "Ns") {
                    ss >> value.Ns;
                } else if (arg0 == "Ni") {
                    ss >> value.Ni;
                } else if (arg0 == "d") {
                    ss >> value.d;
                } else if (arg0 == "illum") {
                    ss >> value.illum;
                }
            }
        }
        if (!key.empty()) {
            res[key] = value;
        }
        return res;
    }

private:
    std::vector<std::string> text_obj_, text_mtl_;

    static bool IsSubstring(const std::string &line, const std::string &subline) {
        if (line.length() >= subline.length()) {
            return subline == std::string(line.begin(), line.begin() + subline.length());
        }
        return false;
    }
};