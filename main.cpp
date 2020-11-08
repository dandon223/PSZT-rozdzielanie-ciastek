#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include <random>
#include<tuple>
#include <cfloat>

std::vector<int> czytaniePliku(std::string sciezka);
std::vector<std::vector<int>> generacjaPopulacji(int wielkosc_populacji, std::vector<int> const & oceny);
void mutacja(std::vector<int>& potomek,int pm,int pm2,std::mt19937 & gen,std::uniform_real_distribution<double>& dist);
int funkcjaCelu(std::vector<int>& genom , std::vector<int>const & oceny);
std::vector<int> selekcja(std::vector<int>& oceny,std::vector<std::tuple<int , std::vector<int>>>& zbior_dobrych,std::mt19937 gen,std::discrete_distribution<> d);
std::discrete_distribution<> dobraPopulacja(std::vector<std::vector<int>> & populacja, std::vector<int> & wynikiFunkcjiCelu , std::vector<std::tuple<int , std::vector<int>>>& zbior_dobrych);
using namespace std;
int main() {
    std::random_device rd;    //  https://en.cppreference.com/w/cpp/numeric/random/discrete_distribution
    std::mt19937 gen(rd());
    int wielkosc_populacji = 20;
    int prawdMutacji = 5;
    std::uniform_real_distribution<double> rozkladJednolity(0, std::nextafter(100, DBL_MAX)); //losowe liczby zmiennoprzecinkowe od 0 do 100

    vector<int> oceny = czytaniePliku("C:\\Users\\Magda\\CLionProjects\\PSZT_projekt2\\imput.txt");
    vector<vector<int>> populacja = generacjaPopulacji(wielkosc_populacji, oceny);

    // glowna petla

    for(int i = 0 ; i <20000; i++){

        vector<int> wynikiFunkcjiCelu;
        vector<vector<int>> nowaPopulacja;
        nowaPopulacja.clear();
        wynikiFunkcjiCelu.clear();

        int minimalnyWynik=INT_MAX;
        int indexNajlepszegoWyniku = 0;
        int indexDrugiegoNajlepszegoWyniku = 0;

        // ocenianie populacji oraz rezerwowanie dwoch najlepszych osobnikow do nastepnej populacji

        for(int i=0 ; i < populacja.size();i++){
            int wynik = funkcjaCelu(populacja[i], oceny);
            if(wynik <minimalnyWynik){
                minimalnyWynik = wynik;
                indexDrugiegoNajlepszegoWyniku = indexNajlepszegoWyniku;
                indexNajlepszegoWyniku = i;
            }
            wynikiFunkcjiCelu.push_back(wynik);
        }
        // rezerwowany 1 i 2  najlepszy wynik do nastepnej populacji
        nowaPopulacja.push_back(populacja[indexNajlepszegoWyniku]);
        nowaPopulacja.push_back(populacja[indexDrugiegoNajlepszegoWyniku]);

        std::vector<tuple<int , std::vector<int>>> zbiorRozwiazan;
        zbiorRozwiazan.clear();

        // dystrybucja dyskretna po ktorej bedziemy wybierac kolejnych osobnikow do selekcji
        std::discrete_distribution<> rozkladDyskretny = dobraPopulacja(populacja, wynikiFunkcjiCelu, zbiorRozwiazan);

        for(int i=0 ; i <wielkosc_populacji -2 ; i++){
            vector<int> rodzic = selekcja(oceny, zbiorRozwiazan, gen, rozkladDyskretny);
            vector<int> kopia  = rodzic;
            mutacja(rodzic, prawdMutacji , prawdMutacji * 2, gen, rozkladJednolity);
            if(funkcjaCelu(rodzic, oceny) == INT_MAX)
                nowaPopulacja.push_back(kopia);
            else
                nowaPopulacja.push_back(rodzic);
        }
        populacja = nowaPopulacja;
    }
    std::vector<int>wynikiFunkcjiCelu;
    int minimalnyWynik=INT_MAX;
    int indexWyniku = 0;
    for(int i=0 ; i < populacja.size();i++){
        int wynik = funkcjaCelu(populacja[i], oceny);
        if(wynik <minimalnyWynik){
            minimalnyWynik = wynik;
            indexWyniku = i;
        }
    }
    std::vector<int> genom = populacja[indexWyniku];
    cout<<"\n============================\n";
    cout<<"Ilosc ciastek: "<<minimalnyWynik<<endl;
    for(int x : genom)
        cout<<x<<" ";
    cout<<"\n============================\n";
    return 0;
}
// przygotowuje dystrubuante pod selekcje turniejowa [a=1 , k=2],
std::discrete_distribution<> dobraPopulacja(std::vector<std::vector<int>> & populacja, std::vector<int> & wynikiFunkcjiCelu , std::vector<std::tuple<int , std::vector<int>>>& zbior_dobrych){

    for(int i = 0 ; i <populacja.size();i++){
            zbior_dobrych.push_back(make_tuple(wynikiFunkcjiCelu[i], populacja[i]));
    }
    sort(wynikiFunkcjiCelu.begin(), wynikiFunkcjiCelu.end());
    sort(zbior_dobrych.begin(), zbior_dobrych.end());

    double liczbaWynikow = wynikiFunkcjiCelu.size();

    for(int i = 0 ; i < liczbaWynikow; i++){
            wynikiFunkcjiCelu[i] = 1.0 + 10.0 * (1 - i / liczbaWynikow); //selekcja turniejowa [a=1 , k=2]
    }
    std::discrete_distribution<> d(wynikiFunkcjiCelu.begin(), wynikiFunkcjiCelu.end());
    return d;
}
// mutacja kazdego genu osobno , szansa zmiany pm
void mutacja(std::vector<int>& genom , int pm, int pm2,std::mt19937 & gen,std::uniform_real_distribution<double>& dist){
    for(int i = 0 ; i <genom.size();i++){
        double prawd = dist(gen);
        if(genom[i]!=1){
            if(genom[i]>=genom.size()*1.5 && prawd < pm2){
                genom[i] = genom[i] - 2;
            }
             else if(prawd < pm) {
                genom[i] = genom[i] - 1;
             }
        }
    }
}
// selekcja osobnika do mutacji , selekcja turniejowa [a=1 , k=2],
// wazne zeby przed tym wywolac dobraPopulacja , ktora sortuje oraz tworzy rozklad prawdopodobienstwa wyboru poszczegolnych osobnikow
std::vector<int> selekcja(std::vector<int>& oceny,std::vector<std::tuple<int , std::vector<int>>>& zbior_dobrych , std::mt19937 gen,std::discrete_distribution<> d){
    int liczba1 = d(gen);
    std::vector<int> rodzic1 = get<1>(zbior_dobrych[liczba1]);

    int liczba2 = d(gen);
    std::vector<int> rodzic2 = get<1>(zbior_dobrych[liczba2]);

    if(funkcjaCelu(rodzic1, oceny) < funkcjaCelu(rodzic2, oceny))
        return rodzic1;
    else
        return rodzic2;
}
// ocenia osobnika , naprawia drobne bledy
int funkcjaCelu(std::vector<int> & genom , std::vector<int> const & oceny){
    int suma=0;
    for(int i =0 ; i <genom.size();i++){
        if(genom[i]<=0 || oceny[i]==1)
            genom[i]=1;
        if(i==genom.size()-1){
            suma = suma + genom[i];
            break;
        }
        else if((oceny[i] <oceny[i+1] && genom[i] < genom[i+1]) || (oceny[i] > oceny[i+1] && genom[i] >genom[i+1]) || (oceny[i]==oceny[i+1] && genom[i]==genom[i+1] )  ){
            suma = suma +genom[i];
        }else{
            return INT_MAX;
        }
    }
    return  suma;
}
//zczytuje oceny uczniow z pliku
std::vector<int> czytaniePliku(std::string sciezka){
    vector<int> v;
    ifstream file;
    file.open (sciezka);
    int word;
    while (file >> word){
        v.push_back(word);
    }
    return v;
}
//generuje poczatkowa populacje z ocen uczniow
std::vector<std::vector<int>> generacjaPopulacji(int wielkosc_populacji, std::vector<int> const & oceny){
    vector<vector<int>> populacja;
    for(int i=0 ; i <wielkosc_populacji;i++){
        populacja.push_back(oceny);
    }
    return populacja;
}