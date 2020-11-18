#include "../include/TestGenerator.hpp"

using namespace std;

const std::string TestGenerator::FILE_BASE = "./in/input";
const std::string TestGenerator::FILE_EXTENSION = ".txt";

int TestGenerator::testIndex = 0;
int TestGenerator::seed = 0;

void TestGenerator::generateTests()
{
    srand(seed);

    std::random_device rd{};
    std::mt19937 gen{rd()};
    gen.seed(seed);

    randomCase(20, 100); //liczba osob w grupie: 20; oceny calkowicie losowe od 1 do 100
    normalCase(20, 100, 10, &gen);   //iczba osob w grupie: 20; oceny losowe od 1 do 100 z rozkladem normalnym (50,10)
    randomCase(200, 100); //liczba osob w grupie: 200; oceny calkowicie losowe od 1 do 100
    normalCase(200, 100, 10, &gen);   //liczba osob w grupie: 200; oceny losowe od 1 do 100 z rozkladem normalnym (50,10)
    randomCase(2000, 100); //liczba osob w grupie: 2000; oceny calkowicie losowe od 1 do 100
    normalCase(2000, 100, 10, &gen);   //liczba osob w grupie: 2000; oceny losowe od 1 do 100 z rozkladem normalnym (50,10)
    randomCase(20, 6); //liczba osob w grupie: 20; oceny calkowicie losowe od 1 do 6
    normalCase(20, 6, 1.5, &gen); //liczba osob w grupie: 20; oceny losowe od 1 do 6 z rozkladem normalnym (3.5, 1.5)
    randomCase(200, 6); //liczba osob w grupie: 200; oceny calkowicie losowe od 1 do 6
    normalCase(200, 6, 1.5, &gen); //liczba osob w grupie: 200; oceny losowe od 1 do 6 z rozkladem normalnym (3.5, 1.5)
    randomCase(2000, 6); //liczba osob w grupie: 2000; oceny calkowicie losowe od 1 do 6
    normalCase(2000, 6, 1.5, &gen); //liczba osob w grupie: 2000; oceny losowe od 1 do 6 z rozkladem normalnym (3.5, 1.5)
    normalCase(20, 199, 15, &gen); //rozklad jak IQ Wechslera - liczby losowe z rozkladem normalnym o sredniej 100 i odchyleniu standardowym 15; 20 osob
    normalCase(200, 199, 15, &gen); //jak w poprzednim tylko 200 osob
    normalCase(2000, 199, 15, &gen); //jak w poprzednim tylko 2000 osob
    
}

void TestGenerator::randomCase(int numberOfStudents, int maxMark)
{
    for( int i = 0; i < TESTS_IN_SET; ++i )
    {
        fstream file;
        file.open(FILE_BASE + to_string(testIndex) + FILE_EXTENSION, ios::out);

        if( file.good() )
        {
            for( int j = 0; j < numberOfStudents; ++j )
            {
                file << (rand() % maxMark) + 1 << " ";
            }
            
            file.close();
        }
        else
        {
            cout << "nie udalo sie zamisac do pliku " << FILE_BASE + to_string(testIndex) + FILE_EXTENSION << "\n";
        }
        testIndex++;
    }
}

void TestGenerator::normalCase(int numberOfStudents, int maxMark, double standardDeviation, mt19937 *gen)
{
    for( int i = 0; i < TESTS_IN_SET; ++i )
    {
        fstream file;
        file.open(FILE_BASE + to_string(testIndex) + FILE_EXTENSION, ios::out);

        if( file.good() )
        {   
            double avg = (maxMark + 1.0) / 2.0;
            normal_distribution<> d{avg,standardDeviation};

            for( int j = 0; j < numberOfStudents; ++j )
            {
                int tmp;
                do
                {
                    tmp = round(d(*gen));
                } while (tmp < 1 || tmp > maxMark);
                
                file << tmp << " ";
            }
            
            file.close();
        }
        else
        {
            cout << "nie udalo sie zamisac do pliku " << FILE_BASE + to_string(testIndex) + FILE_EXTENSION << "\n";
        }
        testIndex++;
    }
}