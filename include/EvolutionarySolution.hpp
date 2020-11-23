#ifndef EVOLUTIONARY_SOLUTION_HPP
#define EVOLUTIONARY_SOLUTION_HPP

#include <vector>
#include <string>
#include <random>
#include <tuple>
#include <fstream>
#include <climits>
#include <algorithm>
#include <cfloat>
#include <iostream>
#include <chrono>

struct MileStone
{
    long long time;
    int generations;
    int result;
};
class EvolutionarySolution
{
    private:
        int populationSize;
	    int numberOfGenerations;
	    double mutationFactor;
        std::vector<int> marks;
        std::vector<int> results;
        std::vector<int> numberOfRepetitions;
        std::vector<MileStone> milestones;

        std::vector<std::vector<int> > generatePopulation();
        void simplifyRepetitions();
        void restoreRepetitions();
        void mutation_v1(std::vector<int>& child, std::mt19937 & gene, std::uniform_real_distribution<double>& dist);
        void mutation_v2(std::vector<int>& child, std::mt19937 & gene);
        void mutation_v3(std::vector<int>& child, std::mt19937 & gene);
        int fitnessFunction(std::vector<int>& genom);
        std::vector<int> selection(std::vector<std::tuple<int , std::vector<int> > >& correctSet, std::mt19937 gene, std::discrete_distribution<> d);
        std::discrete_distribution<> correctPopulation(std::vector<std::vector<int>> & population, std::vector<int> & fitnessResults , std::vector<std::tuple<int , std::vector<int> > >& correctSet);
        MileStone getMileStone(int generation, std::chrono::steady_clock::time_point begin);
    public:
        EvolutionarySolution(std::vector<int>marks, int populationSize/*=100*/, double mutationFactor/*=5*/);
        void runSolution(int numberOfMutation, int seed, std::chrono::steady_clock::time_point begin, long long period, long long times);
        int getRezult();
        void writeResult();
        std::vector<MileStone> getMilestones();
};

#endif