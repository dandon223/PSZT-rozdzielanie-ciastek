#ifndef ITERATIVE_SOLUTION_HPP
#define ITERATIVE_SOLUTION_HPP

#include <bits/stdc++.h>

class IterativeSolution
{
    private:
        int numberOfStudents;
        std::vector<int>marks;
        std::vector<int>cookiesAmount;
        int result;

        std::vector<std::pair<int, int> >reduceProblem();

    public:
        IterativeSolution();
        void setMarks(std::vector<int> marks);
        void runSolution();
        
        int getResult();
        void writeResult();

};


#endif