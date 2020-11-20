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
        int wielkoscPopulacji;
	    int liczbaGeneracji;
	    double prawdopodobienstwoMutacji;
        std::vector<int> oceny;
        std::vector<int> wynik;
        std::vector<int> krotnosci;
        std::vector<MileStone> milestones;

        std::vector<std::vector<int> > generacjaPopulacji();
        void tworzeniekrotnosci();
        void powrotOcen();
        void mutacja_v1(std::vector<int>& potomek, std::mt19937 & gen, std::uniform_real_distribution<double>& dist);
        void mutacja_v2(std::vector<int>& potomek, std::mt19937 & gen);
        void mutacja_v3(std::vector<int>& potomek, std::mt19937 & gen);
        int funkcjaCelu(std::vector<int>& genom);
        std::vector<int> selekcja(std::vector<std::tuple<int , std::vector<int> > >& zbior_dobrych, std::mt19937 gen, std::discrete_distribution<> d);
        std::discrete_distribution<> dobraPopulacja(std::vector<std::vector<int>> & populacja, std::vector<int> & wynikiFunkcjiCelu , std::vector<std::tuple<int , std::vector<int> > >& zbior_dobrych);
        MileStone getMileStone(int generation, std::chrono::steady_clock::time_point begin);
    public:
        EvolutionarySolution(int wielkoscPopulacji/*=100*/, double prawdopodobienstwoMutacji/*=5*/);
        void setOceny(std::vector<int>oceny);
        void runSolution(int wersjaMutacji, int seed, std::chrono::steady_clock::time_point begin, long long period, long long times);
        int getRezultat();
        void piszWynik();
        std::vector<MileStone> getMilestones();
};

#endif