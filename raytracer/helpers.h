#pragma once
/*
#include "parser.h"
#include <iostream>

std::ostream& operator<<(std::ostream& fout, const Format& ft) {
    fout << "Format:\n" << ft.v << " " << ft.vn << " " << ft.vt << std::endl;
    return fout;
}


std::ostream& operator<<(std::ostream& fout, const std::vector<Format>& ft) {
    for (auto dd: ft) {
        fout << dd;
    }
    return fout;
}

std::ostream& operator<<(std::ostream& fout, const Point& dd) {
    fout << "Point:\n" << dd.coord_x_ << " " << dd.coord_y_ << " " << dd.coord_z_ << std::endl;
    return fout;
}

std::ostream& operator<<(std::ostream& fout, const ComplexPoint& dd) {
    fout << "ComplexPoint:\n" << dd.pv << " " << dd.pvt << " " << dd.pvn << std::endl;
    return fout;
}

std::ostream& operator<<(std::ostream& fout, const Triangle& dd) {
    fout << "Triangle:\n" << dd.p0 << " " << dd.p1 << " " << dd.p2 << std::endl;
    fout << std::endl;
    return fout;
}

std::ostream& operator<<(std::ostream& fout, const std::vector<Object*>& dd) {
    for (auto it: dd) {
        fout << *(Triangle *)it;
    }
    return fout;
}

std::ostream& operator<<(std::ostream& fout, const RGB& dd) {
    fout << "RGB:\n" << dd.r << " " << dd.g << " " << dd.b << std::endl;
    return fout;
}

std::ostream& operator<<(std::ostream& fout, const Material& dd) {
    fout << "Triangle:\n" << dd.Ks << " " << dd.Kd << " " << dd.Ka <<
        dd.Ke << dd.Ns << dd.Ni << dd.d << dd.Tr << dd.illum << std::endl;
    return fout;
}

std::ostream& operator<<(std::ostream& fout, const std::unordered_map<std::string, Material>& dd) {
    for (auto it: dd) {
        fout << it.first << "\n" << dd;
    }
    return fout;
}

bool operator==(const Format& other1, const Format& other2) {
    return other1.v == other2.v && other1.vn == other2.vn && other1.vt == other2.vt;
}

bool operator==(const Point& other1, const Point& other2) {
    bool res = other1.coord_x_ == other2.coord_x_ &&
               other1.coord_y_ == other2.coord_y_ &&
               other1.coord_z_ == other2.coord_z_;
    return res;
}

bool operator==(const ComplexPoint& other1, const ComplexPoint& other2) {
    return other1.pv == other2.pv &&
           other1.pvt == other2.pvt &&
           other1.pvn == other2.pvn;
}


bool operator==(const Triangle& other1, const Triangle& other2) {
    bool res = other1.p0 == other2.p0 &&
               other1.p1 == other2.p1 &&
               other1.p2 == other2.p2;
    return res;
}

bool operator==(const std::vector<Object*> other1, const std::vector<Object*> other2) {
    for (size_t i = 0; i < other1.size(); ++i) {
        if (!(*(Triangle *)other1[i] == *(Triangle *)other2[i])) {
            return false;
        }
    }
    return true;
}

bool operator==(const Material& other1, const Material& other2) {
    return other1.Ks == other2.Ks &&
        other1.Kd == other2.Kd &&
        other1.Ka == other2.Ka &&
        other1.Ke == other2.Ke &&
        other1.Ns == other2.Ns &&
        other1.Ni == other2.Ni &&
        other1.d == other2.d &&
        other1.Tr == other2.Tr &&
        other1.illum == other2.illum;
}
 */