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

class EvolutionarySolution
{
    private:
        int wielkoscPopulacji;
	    int liczbaGeneracji;
	    double prawdopodobienstwoMutacji;
        std::vector<int> oceny;
        std::vector<int> wynik;
        std::vector<int> krotnosci;

        std::vector<std::vector<int> > generacjaPopulacji();
        void tworzeniekrotnosci();
        void powrotOcen();
        void mutacja(std::vector<int>& potomek, std::mt19937 & gen, std::uniform_real_distribution<double>& dist);
        int funkcjaCelu(std::vector<int>& genom);
        std::vector<int> selekcja(std::vector<std::tuple<int , std::vector<int> > >& zbior_dobrych, std::mt19937 gen, std::discrete_distribution<> d);
        std::discrete_distribution<> dobraPopulacja(std::vector<std::vector<int>> & populacja, std::vector<int> & wynikiFunkcjiCelu , std::vector<std::tuple<int , std::vector<int> > >& zbior_dobrych);
    public:
        EvolutionarySolution(int wielkoscPopulacji/*=100*/, int liczbaGeneracji/*=10000*/, double prawdopodobienstwoMutacji/*=5*/);
        void setOceny(std::vector<int>oceny);
        void runSolution(int wersjaMutacji);
        int getRezultat();
        void piszWynik();
};

#endif