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
std::vector<std::vector<int>> generacja_populacji(int wielkosc_populacji,std::vector<int> const & oceny);
std::vector<int> krzyzowanie(std::vector<int>& rodzic1 , std::vector<int>& rodzic2); //usrednianie
void mutacja(std::vector<int>& potomek,int pm,int pm2);
int funkcja_celu(std::vector<int>& genom , std::vector<int>const & oceny);
std::vector<int> selekcja(std::vector<int>& oceny,std::vector<std::tuple<int , std::vector<int>>>& zbior_dobrych,std::mt19937 gen,std::discrete_distribution<> d);
std::discrete_distribution<> dobraPopulacja(std::vector<std::vector<int>> & populacja,std::vector<int>const & wyniki ,std::vector<std::tuple<int , std::vector<int>>>& zbior_dobrych,std::vector<double>& wyniki_dobre);
using namespace std;
int main() {
    std::random_device rd;    //  https://en.cppreference.com/w/cpp/numeric/random/discrete_distribution
    std::mt19937 gen(rd());
    srand(time(nullptr));
    int wielkosc_populacji = 20;

    vector<int> oceny = czytanie_pliku();
    vector<vector<int>> populacja ;//= generacja_populacji(wielkosc_populacji-1,oceny);
    for(int i=0 ; i <wielkosc_populacji;i++){
        populacja.push_back(oceny);
    }

    //cout<<"\nPo generacji populacji.\n";
    vector<int> wyniki ;
    // glowna petla
    for(int i = 0 ; i <200000; i++){
        vector<vector<int>> nowaPopulacja;
        nowaPopulacja.clear();
        int minimalnyWynik=INT_MAX;
        int indexWyniku = 0;
        int indexWyniku2 = 0;
        for(int i=0 ; i < populacja.size();i++){
           // for(int gen : populacja[i])
            //    cout<<gen<<" ";
            int wynik = funkcja_celu(populacja[i],oceny);
            if(wynik <minimalnyWynik){
                minimalnyWynik = wynik;
                indexWyniku2 = indexWyniku;
                indexWyniku = i;
            }
            wyniki.push_back(wynik);
          //  cout <<"|" <<wynik<<endl;
        }
      //  cout<<"\n==========================\n";
        nowaPopulacja.push_back(populacja[indexWyniku]);   // rezerwowany 1 i 2  najlepszy wynik
        nowaPopulacja.push_back(populacja[indexWyniku2]);

        std::vector<tuple<int , std::vector<int>>> zbior_dobrych;
        zbior_dobrych.clear();
        std::vector<double> wyniki_dobre;
        wyniki_dobre.clear();

        std::discrete_distribution<> d = dobraPopulacja(populacja,wyniki,zbior_dobrych,wyniki_dobre);
        for(int i=0 ; i <wielkosc_populacji -2 ; i++){
            vector<int> rodzic1 = selekcja(oceny,zbior_dobrych,gen,d);
            //vector<int> rodzic2 = selekcja(populacja,wyniki);
           // vector<int> potomek = krzyzowanie(rodzic1,rodzic2);
           vector<int> kopia  = rodzic1;
            mutacja(rodzic1,5 , 20);
            if(funkcja_celu(rodzic1,oceny)==INT_MAX)
                nowaPopulacja.push_back(kopia);
            else
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
   // cout<<"dobraPopulacja\n";
    for(int i = 0 ; i <populacja.size();i++){
        //if(wyniki[i] != INT_MAX){
    //    cout<<wyniki[i]<<" ";
            wyniki_dobre.push_back(wyniki[i]);
            zbior_dobrych.push_back(make_tuple(wyniki[i],populacja[i]));
        //}
    }
   // cout<<"\n zbior dobrych (suma ciasteczek):";
    sort(wyniki_dobre.begin(),wyniki_dobre.end());
    sort(zbior_dobrych.begin(), zbior_dobrych.end()); //sortuje rosnaca
    //for(std::tuple<int,std::vector<int>> tuple : zbior_dobrych){
    //    cout<<get<0>(tuple)<<" ";
   // }cout<<endl;
    double ilosc_wynikow = wyniki_dobre.size();
 //   cout <<"Wyniki dystrybuanty:\n";
    for(int i = 0 ; i <ilosc_wynikow;i++){
        if(wyniki_dobre[i]==INT_MAX)
            wyniki_dobre[i] = 1.0 +10.0*(1 - i/ilosc_wynikow)/populacja.size();
        else
            wyniki_dobre[i] = 1.0 +10.0*(1 - i/ilosc_wynikow); //selekcja turniejowa [a=1 , k=2]
  //      cout<<wyniki_dobre[i]<<" ";
    }
   // cout<<"\n=========================\n";
    std::random_device rd;    //  https://en.cppreference.com/w/cpp/numeric/random/discrete_distribution
    std::mt19937 gen(rd());

    std::discrete_distribution<> d(wyniki_dobre.begin(),wyniki_dobre.end());
    //std::map<int, int> m;
   // for(int n=0; n<10000; ++n) {
   //     ++m[d(gen)];
   // }
   // for(auto p : m) {
   //     std::cout << p.first << " generated " << p.second << " times\n";
   // }
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
        if(potomek[i]!=1){
            if(potomek[i]>=potomek.size()*1.5 && rand() % (100 + 1) < pm2){
                potomek[i] = potomek[i] - 2;
            }
             else if(rand() % (100 + 1) < pm) {
                     potomek[i] = potomek[i] - 1;
             }

        }
    }
}
// selekcja osobnika do krzyzowania i mutacji , selekcja turniejowa [a=1 , k=2],
// wazne zeby przed tym wywolac dobraPopulacja , ktora oczyszcza osobniki oraz sortuje
std::vector<int> selekcja(std::vector<int>& oceny,std::vector<std::tuple<int , std::vector<int>>>& zbior_dobrych , std::mt19937 gen,std::discrete_distribution<> d){
    int liczba1 = d(gen);
    std::vector<int> rodzic1 = get<1>(zbior_dobrych[liczba1]);
   // cout<<"\nWYBRANY RODZIC 1\n";
   // for(int i=0 ; i <rodzic1.size();i++){
   //     cout<<rodzic1[i]<<" ";
   // }
   // cout<<"\n=====================================\n";
    int liczba2 = d(gen);
    std::vector<int> rodzic2 = get<1>(zbior_dobrych[liczba2]);
    if(funkcja_celu(rodzic1,oceny) < funkcja_celu(rodzic2,oceny))
        return rodzic1;
    else
        return rodzic2;
}
// ocenia osobnika , naprawia drobne bledy
int funkcja_celu(std::vector<int> &genom , std::vector<int> const & oceny){
    int suma=0;
    int niespelnianie_warunkow = 0;
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
            niespelnianie_warunkow = niespelnianie_warunkow +1;
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
std::vector<std::vector<int>> generacja_populacji(int wielkosc_populacji,std::vector<int> const & oceny){
    vector<vector<int>> populacja;
    int minimalna_ocena = *min_element (oceny.begin(), oceny.end());
    int maxymalna_ocena = *max_element (oceny.begin(), oceny.end());
    int max_min = *max_element (oceny.begin(), oceny.end()) - *min_element (oceny.begin(), oceny.end());
    int liczba_ocen = oceny.size();
    for(int i=0 ; i<wielkosc_populacji;i++){
            populacja.push_back(generacja_genomu(liczba_ocen,1,maxymalna_ocena,oceny));
            cout<<i<<" ";
    }
    return populacja;
}
//generuje prawidlowy genom osobika poczatkowej populacji az do skutku
std::vector<int> generacja_genomu(int dlugosc,int min , int max,std::vector<int> const & oceny){
    vector<int>v;
    float max_min = max - min;
    float liczba_osob = oceny.size();
        for(int i = 0 ; i < dlugosc;i++){
            if(oceny[i]==1)
                v.push_back(1);
            else
                v.push_back((int) oceny[i]/max_min*liczba_osob);
        }
    return  v;
}