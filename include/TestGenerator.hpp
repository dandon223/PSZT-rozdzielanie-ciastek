#ifndef TEST_GENERATOR_HPP
#define TEST_GENERATOR_HPP

#include <bits/stdc++.h>

class TestGenerator
{
private:
    static const int TESTS_IN_SET = 10;

    static int testIndex;
    static int seed;


    static const std::string FILE_BASE;
    static const std::string FILE_EXTENSION;

    static void randomCase(int numberOfStudents, int maxMark);
    static void normalCase(int numberOfStudents, int maxMark, double standardDeviation, std::mt19937 *gen);

public:
    static void generateTests();
};


#endif