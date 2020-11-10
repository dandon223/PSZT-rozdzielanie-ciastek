#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include <random>
#include<tuple>
#include <cfloat>

using namespace std;

const string FILE_NAME = "imput.txt";

vector<int> czytaniePliku(string sciezka);
vector<vector<int>> generacjaPopulacji(int wielkosc_populacji, vector<int> const & oceny);
void mutacja(vector<int>& potomek, int pm, int pm2, mt19937 & gen, uniform_real_distribution<double>& dist);
int funkcjaCelu(vector<int>& genom , vector<int>const & oceny);
vector<int> selekcja(vector<int>& oceny,vector<tuple<int , vector<int>>>& zbior_dobrych, mt19937 gen, discrete_distribution<> d);
discrete_distribution<> dobraPopulacja(vector<vector<int>> & populacja, vector<int> & wynikiFunkcjiCelu , vector<tuple<int , vector<int>>>& zbior_dobrych);

int main(int argc, char *argv[]) {
	int wielkoscPopulacji = 100;
	int liczbaGeneracji = 10000;
	int prawdopodobienstwoMutacji = 5;
	
	//analiza flag
	for(int i = 1; i < argc; i+=2 )
	{
		if(i+1 < argc)
		{
			switch(argv[i][1])
			{
				case 'p': //populacja
					if(stoi(argv[i+1]) < 5)
					{
						cout << "populacja musi byc nie mniejsza niz 5\n";
						return 0;
					}
					else
					{
						wielkoscPopulacji = stoi(argv[i+1]);
					}
				case 'g': //liczba generacji
					liczbaGeneracji = stoi(argv[i+1]);
				case 'm': //prawdopodobienstwo mutacji
					prawdopodobienstwoMutacji = stoi(argv[i+1]);
			}
		}
		else
		{
			cout << "bledna liczba parametrow wykonania\n";
			return 0;
		}
	}
	
    random_device rd;    //  https://en.cppreference.com/w/cpp/numeric/random/discrete_distribution
    mt19937 gen(rd());
    
    uniform_real_distribution<double> rozkladJednolity(0, nextafter(100, DBL_MAX)); //losowe liczby zmiennoprzecinkowe od 0 do 100

    vector<int> oceny = czytaniePliku(FILE_NAME);
    vector<vector<int>> populacja = generacjaPopulacji(wielkoscPopulacji, oceny);

    // glowna petla

    for(int i = 0 ; i < liczbaGeneracji; i++){
        vector<int> wynikiFunkcjiCelu;
        vector<vector<int>> nowaPopulacja;
        nowaPopulacja.clear();
        wynikiFunkcjiCelu.clear();

        int minimalnyWynik=INT_MAX;
        int indexNajlepszegoWyniku = 0;
        int indexDrugiegoNajlepszegoWyniku = 0;

        // ocenianie populacji oraz rezerwowanie dwoch najlepszych osobnikow do nastepnej populacji

        for(int i=0 ; i < populacja.size();i++){
        	//for(int gen : populacja[i])
        	//    cout<<gen<<" ";
            int wynik = funkcjaCelu(populacja[i], oceny);
            if(wynik <minimalnyWynik){
                minimalnyWynik = wynik;
                indexDrugiegoNajlepszegoWyniku = indexNajlepszegoWyniku;
                indexNajlepszegoWyniku = i;
            }
            wynikiFunkcjiCelu.push_back(wynik);
            //cout <<"|" <<wynik<<"\n";
        }
        // rezerwowany 1 i 2  najlepszy wynik do nastepnej populacji
        nowaPopulacja.push_back(populacja[indexNajlepszegoWyniku]);
        nowaPopulacja.push_back(populacja[indexDrugiegoNajlepszegoWyniku]);

        vector<tuple<int , vector<int>>> zbiorRozwiazan;
        zbiorRozwiazan.clear();

        // dystrybucja dyskretna po ktorej bedziemy wybierac kolejnych osobnikow do selekcji
        discrete_distribution<> rozkladDyskretny = dobraPopulacja(populacja, wynikiFunkcjiCelu, zbiorRozwiazan);

        for(int i=0 ; i <wielkoscPopulacji -2 ; i++){
            vector<int> rodzic = selekcja(oceny, zbiorRozwiazan, gen, rozkladDyskretny);
            vector<int> kopia  = rodzic;
            mutacja(rodzic, prawdopodobienstwoMutacji , prawdopodobienstwoMutacji * 2, gen, rozkladJednolity);
            if(funkcjaCelu(rodzic, oceny) == INT_MAX)
                nowaPopulacja.push_back(kopia);
            else
                nowaPopulacja.push_back(rodzic);
        }
        populacja = nowaPopulacja;
    }
    vector<int>wynikiFunkcjiCelu;
    int minimalnyWynik=INT_MAX;
    int indexWyniku = 0;
    for(int i=0 ; i < populacja.size();i++){
        int wynik = funkcjaCelu(populacja[i], oceny);
        if(wynik <minimalnyWynik){
            minimalnyWynik = wynik;
            indexWyniku = i;
        }
    }
    vector<int> genom = populacja[indexWyniku];
    cout<<"\n============================\n";
    cout<<"Ilosc ciastek: "<<minimalnyWynik<<endl;
    for(int x : genom)
        cout<<x<<" ";
    cout<<"\n============================\n";
    return 0;
}
// przygotowuje dystrubuante pod selekcje turniejowa [a=1 , k=2],
discrete_distribution<> dobraPopulacja(vector<vector<int>> & populacja, vector<int> & wynikiFunkcjiCelu , vector<tuple<int , vector<int>>>& zbior_dobrych){

    for(int i = 0 ; i <populacja.size();i++){
            zbior_dobrych.push_back(make_tuple(wynikiFunkcjiCelu[i], populacja[i]));
    }
    sort(wynikiFunkcjiCelu.begin(), wynikiFunkcjiCelu.end());
    sort(zbior_dobrych.begin(), zbior_dobrych.end());

    double liczbaWynikow = wynikiFunkcjiCelu.size();

    for(int i = 0 ; i < liczbaWynikow; i++){
            wynikiFunkcjiCelu[i] = 1.0 + 10.0 * (1 - i / liczbaWynikow); //selekcja turniejowa [a=1 , k=2]
    }
    discrete_distribution<> d(wynikiFunkcjiCelu.begin(), wynikiFunkcjiCelu.end());
    return d;
}
// mutacja kazdego genu osobno , szansa zmiany pm
void mutacja(vector<int>& genom , int pm, int pm2,mt19937 & gen, uniform_real_distribution<double>& dist){
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
vector<int> selekcja(vector<int>& oceny, vector<tuple<int , vector<int>>>& zbior_dobrych , mt19937 gen,discrete_distribution<> d){
    int liczba1 = d(gen);
    vector<int> rodzic1 = get<1>(zbior_dobrych[liczba1]);

    int liczba2 = d(gen);
    vector<int> rodzic2 = get<1>(zbior_dobrych[liczba2]);

    if(funkcjaCelu(rodzic1, oceny) < funkcjaCelu(rodzic2, oceny))
        return rodzic1;
    else
        return rodzic2;
}
// ocenia osobnika , naprawia drobne bledy
int funkcjaCelu(vector<int> & genom , vector<int> const & oceny){
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
vector<int> czytaniePliku(string sciezka){
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
vector<vector<int>> generacjaPopulacji(int wielkosc_populacji, vector<int> const & oceny){
    vector<vector<int>> populacja;
    for(int i=0 ; i <wielkosc_populacji;i++){
        populacja.push_back(oceny);
    }
    return populacja;
}
