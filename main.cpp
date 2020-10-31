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
std::vector<int> generacja_genomu(int dlugosc,int min , int max);
std::vector<std::vector<int>> generacja_populacji(int wielkosc_populacji , int dlugosc_genomu,int min , int max);
int funkcja_celu(std::vector<int> genom , std::vector<int> oceny);
std::vector<int> selekcja(std::vector<std::vector<int>> populacja , std::vector<int> wyniki);
using namespace std;
int main() {
    srand(time(nullptr));
    int wielkosc_populacji = 20;

    vector<int> oceny = czytanie_pliku("imput.txt");
    vector<vector<int>> populacja = generacja_populacji(wielkosc_populacji,oceny.size(),1,*max_element(oceny.begin(), oceny.end())+3);

    vector<int> wyniki ;
    for(int i=0 ; i < populacja.size();i++){
        for(int gen : populacja[i])
            cout<<gen<<" ";
        int wynik = funkcja_celu(populacja[i],oceny);
        wyniki.push_back(wynik);
        cout <<"|" <<wynik<<endl;
    }
    selekcja(populacja,wyniki);




    return 0;
}
std::vector<int> selekcja(std::vector<std::vector<int>> populacja , std::vector<int> wyniki){ //selekcja turniejowa a=1 , k = 2
    std::vector<tuple<int , std::vector<int>>> zbior_dobrych;
    std::vector<double> wyniki_dobre;
    for(int i = 0 ; i <populacja.size();i++){
        if(wyniki[i] != INT_MAX){
            wyniki_dobre.push_back(wyniki[i]);
            zbior_dobrych.push_back(make_tuple(wyniki[i],populacja[i]));
        }
    }
    sort(zbior_dobrych.begin(), zbior_dobrych.end());
    cout<<endl<<"=================="<<endl;
    for(tuple<int , std::vector<int>> t : zbior_dobrych){
        cout<<get<0>(t)<<" | ";
        for(int x : get<1>(t))
            cout<<x<<" ";
        cout<<endl;
    }
    double ilosc_wynikow = wyniki_dobre.size();
    for(double i = 0 ; i <ilosc_wynikow;i++){
        wyniki_dobre[i] = 1.0 +2.0*(1 - i/ilosc_wynikow);
    }
    cout<<"===================="<<endl;
    for(double x : wyniki_dobre)
        cout <<x<<" ";
    cout<<endl;
    std::random_device rd;    //  https://en.cppreference.com/w/cpp/numeric/random/discrete_distribution
    std::mt19937 gen(rd());
    std::discrete_distribution<> d(wyniki_dobre.begin(),wyniki_dobre.end()); /* //jak chcesz zobaczyc dystrybucje to odkomentuj
    std::map<int, int> m;
    for(int n=0; n<2; ++n) {
        ++m[d(gen)];
    }
    for(auto p : m) {
        std::cout << p.first << " generated " << p.second << " times\n";
    } */
    cout<<"========================="<<endl;
    int liczba1 = d(gen);
    std::vector<int> rodzic = get<1>(zbior_dobrych[liczba1]);
    cout<<liczba1<<" | ";
    for(int x: rodzic){
        cout<<x<< " ";
    }
    return rodzic;




}
int funkcja_celu(std::vector<int> genom , std::vector<int> oceny){
    int suma=0;
    for(int i =0 ; i <genom.size();i++){
        if(genom[i]==0)
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
std::vector<std::vector<int>> generacja_populacji(int wielkosc_populacji , int dlugosc_genomu,int min , int max){
    vector<vector<int>> populacja;
    for(int i=0 ; i<wielkosc_populacji;i++){
            populacja.push_back(generacja_genomu(dlugosc_genomu,min,max));
    }
    return populacja;
}
std::vector<int> generacja_genomu(int dlugosc,int min , int max){
    vector<int>v;
    for(int i = 0 ; i < dlugosc;i++){
        v.push_back(rand()%(max-min + 1) + min);
    }
    return  v;
}