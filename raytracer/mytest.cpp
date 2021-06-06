#include "helpers.h"
/*
void TestExtractFormat() {
    std::string test = " 1 5 6 2";
    std::vector<Format> expected = {{1, 0, 0},
                                    {5, 0, 0},
                                    {6, 0, 0},
                                    {2, 0, 0}};
    std::vector<Format> answer = Parser::ExtractFormat(test);
    if (expected != answer) {
        std::cout << "Fuck ExtractFormat" << std::endl;
        std::cout << answer;
    } else {
        std::cout << "That's good!" << std::endl;
    }

    test = "-4//-1 -3//-1 -2//-1 ";
    expected = {{-4, 0, -1},
                {-3, 0, -1},
                {-2, 0, -1}};
    answer = Parser::ExtractFormat(test);
    if (expected != answer) {
        std::cout << "Fuck ExtractFormat 2" << std::endl;
        std::cout << answer;
    } else {
        std::cout << "That's good!" << std::endl;
    }

    test = "78/95/43 80/98/43 83/105/43 84/103/43";
    expected = {{78,  95, 43},
                {80,  98, 43},
                {83,  105, 43},
                {84,  103, 43}};
    answer = Parser::ExtractFormat(test);
    if (expected != answer) {
        std::cout << "Fuck ExtractFormat 3" << std::endl;
        std::cout << answer;
    } else {
        std::cout << "That's good!" << std::endl;
    }

    test = "-4/-1 -3/-1 -2/-1 ";
    expected = {{-4,  -1},
                {-3,  -1},
                {-2,  -1}};
    answer = Parser::ExtractFormat(test);
    if (expected != answer) {
        std::cout << "Fuck ExtractFormat 4" << std::endl;
        std::cout << answer;
    } else {
        std::cout << "That's good!" << std::endl;
    }
}

void TestParserOld() {
    std::string text = "## Object floor\n"
        "v   1.0000 0.0000 -1.0400\n"
        "v  -0.9900 0.0000 -1.0400\n"
        "v  -1.0100 0.0000  0.9900\n"
        "v   1.0000 0.0000  0.9900\n"
        "# 4 vertices\n"
        "\n"
        "vn 0.0000 1.0000 -0.0000\n"
        "# 1 vertex normals\n"
        "\n"
        "g floor\n"
        "usemtl floor\n"
        "\n"
        "s 1\n"
        "f -4//-1 -3//-1 -2//-1 -1//-1";
    std::stringstream ss;
    ss << text;
    text.clear();
    std::vector<std::string> test;
    while (getline(ss, text)) {
        test.push_back(text);
    }
    ComplexPoint p1 = {{1, 0, -1.04},
                       {},
                       {0, 1,0}},
                 p2 = {{-0.9900, 0, -1.04},
                       {},
                       {0, 1,0}},
                 p3 = {{-1.0100, 0, 0.9900},
                       {},
                       {0, 1,0}},
                 p4 = {{1.0000, 0, 0.9900},
                       {},
                       {0, 1,0}};
    std::vector<Object*> expected = {
        new Triangle{p1, p2, p3},
        new Triangle{p1, p3, p4}
    };

    std::vector<Object*> answer = Parser::ParseText(test, {""});
    if (expected != answer) {
        std::cout << "Fuck Parser" << std::endl;
        //std::cout << expected;
        //std::cout << answer;
    } else {
        std::cout << "That's good!" << std::endl;
    }
}

void TestParserMtl() {
    std::string text = "# 3ds Max Wavefront OBJ Exporter v0.97b - (c)2007 guruware\n"
        "# File Created: 25.09.2020 16:04:53\n"
        "\n"
        "newmtl wire_086086086\n"
        "\tNs 32\n"
        "\td 1\n"
        "\tTr 0\n"
        "\tTf 1 1 1\n"
        "\tillum 2\n"
        "\tKa 0.3373 0.3373 0.3373\n"
        "\tKd 0.3373 0.3373 0.3373\n"
        "\tKs 0.3500 0.3500 0.3500";
    std::stringstream ss;
    ss << text;
    text.clear();
    std::vector<std::string> test;
    while (getline(ss, text)) {
        test.push_back(text);
    }
    std::unordered_map<std::string, Material> expected = {
        {"wire_086086086", Material{
                               RGB{0.3500, 0.3500, 0.3500},
                               RGB{0.3373, 0.3373, 0.3373},
                               RGB{0.3373, 0.3373, 0.3373},
                               RGB{},
                               32,
                               0,
                               1,
                               0,
                               2
                           }}
    };

    auto answer = Parser::ParseMTLText(test);
    if (expected != answer) {
        std::cout << "Fuck ParserMTL" << std::endl;
        std :: cout << answer << std::endl;
    } else {
        std::cout << "That's good!" << std::endl;
    }

}


int main() {
    TestExtractFormat();
    TestParserMtl();
}*/