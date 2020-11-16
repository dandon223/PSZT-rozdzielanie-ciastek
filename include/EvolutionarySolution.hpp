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
        const std::string FILE_NAME = "./in/input.txt";

        int wielkoscPopulacji;
	    int liczbaGeneracji;
	    int prawdopodobienstwoMutacji;
        std::vector<int> oceny;


        std::vector<int> czytaniePliku(std::string sciezka);
        std::vector<std::vector<int> > generacjaPopulacji(int wielkosc_populacji, std::vector<int> const & oceny);
        void mutacja(std::vector<int>& potomek, int pm, int pm2, std::mt19937 & gen, std::uniform_real_distribution<double>& dist);
        int funkcjaCelu(std::vector<int>& genom , std::vector<int>const & oceny);
        std::vector<int> selekcja(std::vector<int>& oceny, std::vector<std::tuple<int , std::vector<int> > >& zbior_dobrych, std::mt19937 gen, std::discrete_distribution<> d);
        std::discrete_distribution<> dobraPopulacja(std::vector<std::vector<int>> & populacja, std::vector<int> & wynikiFunkcjiCelu , std::vector<std::tuple<int , std::vector<int> > >& zbior_dobrych);
    public:
        void runSolution();
        EvolutionarySolution(int wielkoscPopulacji/*=100*/, int liczbaGeneracji/*=10000*/, int prawdopodobienstwoMutacji/*=5*/);

};

#endif