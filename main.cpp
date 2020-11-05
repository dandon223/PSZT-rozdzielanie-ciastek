#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <ctime>
#include <map>
#include <random>
#include<tuple>

std::vector<int> czytanie_pliku(std::string path);
std::vector<int> generacja_genomu(int dlugosc,int min , int max,std::vector<int> const & oceny);
std::vector<std::vector<int>> generacja_populacji(int wielkosc_populacji , int dlugosc_genomu,int min , int max,std::vector<int> const & oceny);
std::vector<int> krzyzowanie(std::vector<int>& rodzic1 , std::vector<int>& rodzic2); //usrednianie
void mutacja(std::vector<int>& potomek,int pm,int pm2);
int funkcja_celu(std::vector<int>& genom , std::vector<int>const & oceny);
std::vector<int> selekcja(std::vector<std::tuple<int , std::vector<int>>>& zbior_dobrych,std::mt19937 gen,std::discrete_distribution<> d);
std::discrete_distribution<> dobraPopulacja(std::vector<std::vector<int>> & populacja,std::vector<int>const & wyniki ,std::vector<std::tuple<int , std::vector<int>>>& zbior_dobrych,std::vector<double>& wyniki_dobre);
using namespace std;
int main() {
    std::random_device rd;    //  https://en.cppreference.com/w/cpp/numeric/random/discrete_distribution
    std::mt19937 gen(rd());
    srand(time(nullptr));
    int wielkosc_populacji = 40;

    vector<int> oceny = czytanie_pliku("");
    vector<vector<int>> populacja = generacja_populacji(wielkosc_populacji,oceny.size(),1,oceny.size()*2,oceny);
    cout<<"\nPo generacji populacji.\n";
    vector<int> wyniki ;
    // glowna petla
    for(int i = 0 ; i <30000; i++){
        vector<vector<int>> nowaPopulacja;
        nowaPopulacja.clear();
        int minimalnyWynik=INT_MAX;
        int indexWyniku = 0;
        int indexWyniku2 = 0;
        for(int i=0 ; i < populacja.size();i++){
           // for(int gen : populacja[i])
          //      cout<<gen<<" ";
            int wynik = funkcja_celu(populacja[i],oceny);
            if(wynik <minimalnyWynik){
                minimalnyWynik = wynik;
                indexWyniku2 = indexWyniku;
                indexWyniku = i;
            }
            wyniki.push_back(wynik);
         //   cout <<"|" <<wynik<<endl;
        }
        nowaPopulacja.push_back(populacja[indexWyniku]);   // rezerwowany 1 i 2  najlepszy wynik
        nowaPopulacja.push_back(populacja[indexWyniku2]);

        std::vector<tuple<int , std::vector<int>>> zbior_dobrych;
        zbior_dobrych.clear();
        std::vector<double> wyniki_dobre;
        wyniki_dobre.clear();

        std::discrete_distribution<> d = dobraPopulacja(populacja,wyniki,zbior_dobrych,wyniki_dobre);
        for(int i=0 ; i <wielkosc_populacji -2 ; i++){
            vector<int> rodzic1 = selekcja(zbior_dobrych,gen,d);
            //vector<int> rodzic2 = selekcja(populacja,wyniki);
           // vector<int> potomek = krzyzowanie(rodzic1,rodzic2);
            mutacja(rodzic1,15 , 30);
            nowaPopulacja.push_back(rodzic1);
        }
        populacja = nowaPopulacja;
    }

    int minimalnyWynik=INT_MAX;
    int indexWyniku = 0;
    for(int i=0 ; i < populacja.size();i++){
        // for(int gen : populacja[i])
        //      cout<<gen<<" ";
        int wynik = funkcja_celu(populacja[i],oceny);
        if(wynik <minimalnyWynik){
            minimalnyWynik = wynik;
            indexWyniku = i;
        }
        wyniki.push_back(wynik);
        //   cout <<"|" <<wynik<<endl;
    }
    std::vector<int> genom = populacja[indexWyniku];
    cout<<"\n============================\n";
    cout<<"Pop. size = "<<populacja.size()<<endl;
    cout<<"Number: "<<minimalnyWynik<<endl;
    for(int x : genom)
        cout<<x<<" ";
    cout<<"\n============================\n";



    return 0;
}
// wybiera z dostepnej populacji tylko te osobniki ktore spelniaja zalozenia zadania,
// nastepnie przygotowuje dystrubuante pod selekcje turniejowa [a=1 , k=2],
std::discrete_distribution<> dobraPopulacja(std::vector<std::vector<int>> & populacja,std::vector<int>const & wyniki ,std::vector<std::tuple<int , std::vector<int>>>& zbior_dobrych,std::vector<double>& wyniki_dobre){
    for(int i = 0 ; i <populacja.size();i++){
        if(wyniki[i] != INT_MAX){
            wyniki_dobre.push_back(wyniki[i]);
            zbior_dobrych.push_back(make_tuple(wyniki[i],populacja[i]));
        }
    }
    sort(zbior_dobrych.begin(), zbior_dobrych.end()); //sortuje rosnaca
    double ilosc_wynikow = wyniki_dobre.size();
    for(int i = 0 ; i <ilosc_wynikow;i++){
        wyniki_dobre[i] = 1.0 +2.0*(1 - i/ilosc_wynikow); //selekcja turniejowa [a=1 , k=2]
    }
    std::random_device rd;    //  https://en.cppreference.com/w/cpp/numeric/random/discrete_distribution
    std::mt19937 gen(rd());
    std::discrete_distribution<> d(wyniki_dobre.begin(),wyniki_dobre.end());
    return d;
}
// krzyzowanie usredniajace
std::vector<int> krzyzowanie(std::vector<int>& rodzic1 , std::vector<int>& rodzic2){
    std::vector<int> potomek;
    for(int i=0 ; i <rodzic1.size();i++)
        potomek.push_back ((rodzic1[i] +rodzic2[i])/2);
    return potomek;
}
// mutacja kazdego genu osobno , szansa zmiany pm
void mutacja(std::vector<int>& potomek , int pm, int pm2){
    for(int i = 0 ; i <potomek.size();i++){
        if(potomek[i]!=1)
            if(potomek[i]>=15){
                if(rand()%(100 + 1) <pm2)
                    potomek[i] = potomek[i]-2;
            }
            else if(potomek[i]>=10){
                if(rand()%(100 + 1) <pm2)
                    potomek[i] = potomek[i]-1;
            }else
                if(rand()%(100 + 1) <pm)
                    potomek[i] = potomek[i]-1;




    }
}
// selekcja osobnika do krzyzowania i mutacji , selekcja turniejowa [a=1 , k=2],
// wazne zeby przed tym wywolac dobraPopulacja , ktora oczyszcza osobniki oraz sortuje
std::vector<int> selekcja(std::vector<std::tuple<int , std::vector<int>>>& zbior_dobrych , std::mt19937 gen,std::discrete_distribution<> d){
    int liczba1 = d(gen);
    std::vector<int> rodzic = get<1>(zbior_dobrych[liczba1]);
    return rodzic;
}
// ocenia osobnika , naprawia drobne bledy
int funkcja_celu(std::vector<int> &genom , std::vector<int> const & oceny){
    int suma=0;
    for(int i =0 ; i <genom.size();i++){
        if(genom[i]<=0 || oceny[i]==1)
            genom[i]=1;
        if(i==genom.size()-1){
            suma = suma + genom[i];
            break;
        }
        if((oceny[i] <oceny[i+1] && genom[i] < genom[i+1]) || (oceny[i] > oceny[i+1] && genom[i] >genom[i+1]) || (oceny[i]==oceny[i+1] && genom[i]==genom[i+1] )  ){
            suma = suma +genom[i];
        }else{
            return INT_MAX;
        }
    }
    return  suma;
}
//zczytuje z pliku
std::vector<int> czytanie_pliku(std::string path){
    vector<int> v;
    ifstream file;
    file.open (path);
    int word;
    while (file >> word){
        v.push_back(word);
    }
    return v;
}
//generuje poczatkowa populacje
std::vector<std::vector<int>> generacja_populacji(int wielkosc_populacji , int dlugosc_genomu,int min , int max,std::vector<int> const & oceny){
    vector<vector<int>> populacja;
    for(int i=0 ; i<wielkosc_populacji;i++){
            populacja.push_back(generacja_genomu(dlugosc_genomu,min,max,oceny));
            cout<<i<<" ";
    }
    return populacja;
}
//generuje prawidlowy genom osobika poczatkowej populacji az do skutku
std::vector<int> generacja_genomu(int dlugosc,int min , int max,std::vector<int> const & oceny){
    vector<int>v;
    while(true){
        for(int i = 0 ; i < dlugosc;i++){
            if(oceny[i]==1)
                v.push_back(1);
            else
                v.push_back(rand()%(max-min + 1) + min);
        }
        if(funkcja_celu(v,oceny)<INT_MAX)
            break;
        else
            v.clear();
    }
    return  v;
}