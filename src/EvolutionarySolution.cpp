#include "../include/EvolutionarySolution.hpp"

using namespace std;

EvolutionarySolution::EvolutionarySolution(vector<int> marks, int populationSize = 100, double mutationFactor = 5.0)
{
    this->populationSize = populationSize;
    this->numberOfGenerations = numberOfGenerations;
    this->mutationFactor = mutationFactor;
    this->marks = marks;
}
void EvolutionarySolution::simplifyRepetitions() {

    int lastMark = -1;
    std::vector<int> marksWithoutRepetitions;
    for(int mark: marks){
        if(mark != lastMark){
            numberOfRepetitions.push_back(1);
            marksWithoutRepetitions.push_back(mark);
            lastMark = mark;
        }
        else{
            numberOfRepetitions[numberOfRepetitions.size()-1] = numberOfRepetitions[numberOfRepetitions.size()-1]+1;
        }
    }
    this->marks = marksWithoutRepetitions;

}
void EvolutionarySolution::restoreRepetitions(){
    std::vector<int> newResult;
    for(unsigned int i = 0 ; i <numberOfRepetitions.size();i++){
        for(int j = 0 ; j <numberOfRepetitions[i];j++)
            newResult.push_back(results[i]);
    }
    this->results = newResult;
}

// przygotowuje dystrubuante pod selekcje turniejowa [a=1 , k=2],
discrete_distribution<> EvolutionarySolution::correctPopulation(vector<vector<int>> & population, vector<int> & fitnessResult , vector<tuple<int , vector<int>>>& correctSet){

    for(unsigned int i = 0 ; i <population.size();i++){
            correctSet.push_back(make_tuple(fitnessResult[i], population[i]));
    }
    sort(fitnessResult.begin(), fitnessResult.end());
    sort(correctSet.begin(), correctSet.end());

    double fitnessResultSize = fitnessResult.size();

    for(int i = 0 ; i < fitnessResultSize; i++){
            fitnessResult[i] = 1.0 + 10.0 * (1 - i / fitnessResultSize); //selekcja turniejowa [a=1 , k=2]
    }
    discrete_distribution<> d(fitnessResult.begin(), fitnessResult.end());
    return d;
}
// mutacja_v1 kazdego genu osobno , szansa zmiany pm
void EvolutionarySolution::mutation_v1(vector<int>& genome ,mt19937 & gen, uniform_real_distribution<double>& dist){
    for(unsigned int i = 0 ; i <genome.size();i++){
        double drawn = dist(gen);
        if(genome[i]!=1){
            if(genome[i]>=genome.size()*1.5 && drawn < mutationFactor *2){
                genome[i] = genome[i] - 2;
            }
            else if(drawn < mutationFactor) {
                genome[i] = genome[i] - 1;
             }
        }
    }
}

//mutacja rozkladem mormalnym
void EvolutionarySolution::mutation_v2(std::vector<int>& genome, std::mt19937 & gen)
{
    for(unsigned int i = 0 ; i <genome.size();i++){
        normal_distribution<> n_d{(double)genome[i], mutationFactor};
        int newValue = n_d(gen);
        if(newValue < 0)
        {
            newValue = -newValue;
        }
        else
        if(newValue == 0)
        {
            newValue = 1;
        }
        genome[i] = newValue;
    }
}

//mutacja rozkladem mormalnym tylko w dol
void EvolutionarySolution::mutation_v3(std::vector<int>& genome, std::mt19937 & gene)
{
    for(unsigned int i = 0 ; i <genome.size();i++){
        normal_distribution<> n_d{(double)genome[i], mutationFactor};
        int tmp = n_d(gene);
        int newValue = genome[i] - abs(genome[i] - tmp);
        
        if(newValue <= 0)
        {
            newValue = 1;
        }

        genome[i] = newValue;
    }
}

// selekcja osobnika do mutacji , selekcja turniejowa [a=1 , k=2],
// wazne zeby przed tym wywolac correctPopulation , ktora sortuje oraz tworzy rozklad prawdopodobienstwa wyboru poszczegolnych osobnikow
vector<int> EvolutionarySolution::selection( vector<tuple<int , vector<int> > >& correctSet , mt19937 gene, discrete_distribution<> d){
    int number1 = d(gene);
    vector<int> parent1 = get<1>(correctSet[number1]);

    int number2 = d(gene);
    vector<int> parent2 = get<1>(correctSet[number2]);

    if(fitnessFunction(parent1) < fitnessFunction(parent2))
        return parent1;
    else
        return parent2;
}
// ocenia osobnika , naprawia drobne bledy
int EvolutionarySolution::fitnessFunction(vector<int> & genome ){
    int sum = 0;
    for(unsigned int i = 0 ; i < genome.size(); i++){
        if(genome[i] <= 0 || marks[i] == 1)
            genome[i] = 1;
        if( i == genome.size()-1 ){
            sum = sum + genome[i];
            break;
        }
        else if((marks[i] < marks[i+1] && genome[i] < genome[i+1]) || (marks[i] > marks[i+1] && genome[i] > genome[i+1]) || (marks[i] == marks[i+1] && genome[i] == genome[i+1] ) ){
            sum = sum +genome[i];
        }else{
            return INT_MAX;
        }
    }
    return  sum;
}

//generuje poczatkowa populacje z ocen uczniow
vector<vector<int> > EvolutionarySolution::generatePopulation(){
    vector<vector<int> > population;
    for(int i=0 ; i <populationSize;i++){
        population.push_back(marks);
    }
    return population;
}

void EvolutionarySolution::runSolution(int numberOfMutation, int seed, std::chrono::steady_clock::time_point begin, long long period, long long times )
{
    random_device rd;    //  https://en.cppreference.com/w/cpp/numeric/random/discrete_distribution
    mt19937 gen(rd());
    gen.seed(seed);
    
    uniform_real_distribution<double> uniformDistribution(0, nextafter(100, DBL_MAX)); //losowe liczby zmiennoprzecinkowe od 0 do 100
    simplifyRepetitions();
    vector<vector<int>> population = generatePopulation();

    int generationIndex = 0;

    // glowna petla
    for(long long i = 1 ; i <= times; i++)
    {
        while( std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - begin).count() < i *period )
        {
            vector<int> fitnessResult;
            vector<vector<int>> newPopulation;
            newPopulation.clear();
            fitnessResult.clear();

            int minResult=INT_MAX;
            int indexOfBestResult = 0;
            int indexOfSecondBestResult = 0;

            // ocenianie populacji oraz rezerwowanie dwoch najlepszych osobnikow do nastepnej populacji

            for(unsigned int i=0 ; i < population.size();i++){
                int result = fitnessFunction(population[i]);
                if(result <minResult){
                    minResult = result;
                    indexOfSecondBestResult = indexOfBestResult;
                    indexOfBestResult = i;
                }
                fitnessResult.push_back(result);
            }
            
            // rezerwowany 1 i 2  najlepszy wynik do nastepnej populacji
            newPopulation.push_back(population[indexOfBestResult]);
            newPopulation.push_back(population[indexOfSecondBestResult]);

            vector<tuple<int , vector<int>>> setOfResults;
            setOfResults.clear();

            // dystrybucja dyskretna po ktorej bedziemy wybierac kolejnych osobnikow do selekcji
            discrete_distribution<> discreteDistribution = correctPopulation(population, fitnessResult, setOfResults);

            for(int i=0 ; i <populationSize -2 ; i++){
                vector<int> parent = selection(setOfResults, gen, discreteDistribution);
                vector<int> copy  = parent;
                switch (numberOfMutation)
                {
                case 1:
                    mutation_v1(parent, gen, uniformDistribution);
                    break;
                case 2:
                    mutation_v2(parent, gen);
                    break;
                case 3:
                    mutation_v3(parent, gen);
                    break;
                default:
                    break;
                }
                
                if(fitnessFunction(parent) == INT_MAX)
                    newPopulation.push_back(copy);
                else
                    newPopulation.push_back(parent);
            }
            population = newPopulation;
            generationIndex++;
            
        }
        vector<int>fitnessResult;
        int minResult = INT_MAX;
        int indexOfResult = 0;
        for(unsigned int i = 0 ; i < population.size(); i++){
            int result = fitnessFunction(population[i]);
            if(result <minResult){
                minResult = result;
                indexOfResult = i;
            }
        }
        results = population[indexOfResult];
        milestones.push_back(getMileStone(generationIndex, begin));
    }
    restoreRepetitions();
}

void EvolutionarySolution::writeResult()
{
    cout << "uklad ciastek:\n";
    int sum = 0;
    for(int i : results)
    {
        cout << i << " ";
        sum += i;
    }
    cout << "\nrazem:\n" << sum << "\n";


}

int EvolutionarySolution::getRezult()
{
    int sum = 0;
    for(int i : results)
    {
        sum += i;
    }
    return sum;
}

MileStone EvolutionarySolution::getMileStone(int generation, std::chrono::steady_clock::time_point begin)
{
    int sum = 0;
    for( unsigned int i = 0; i < results.size(); ++i )
    {
        sum += results[i] * numberOfRepetitions[i];
    }

    MileStone ms;
    ms.generations = generation;
    ms.result = sum;
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    ms.time = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();

    return ms;
}

vector<MileStone> EvolutionarySolution::getMilestones()
{
    return milestones;
}
