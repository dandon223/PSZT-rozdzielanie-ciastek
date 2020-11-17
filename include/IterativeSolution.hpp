#ifndef ITERATIVE_SOLUTION_HPP
#define ITERATIVE_SOLUTION_HPP

#include <bits/stdc++.h>

class IterativeSolution
{
    private:
        int numberOfStudents;
        std::vector<int>marks;
        int cakesAmount[1000000];

    public:
        IterativeSolution();
        void setMarks(std::vector<int> marks);
        void runSolution();

};


#endif