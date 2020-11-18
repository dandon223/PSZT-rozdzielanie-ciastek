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

    randomCase(30, 100); //pierwsza paczka - liczba osob w grupie: 30; oceny calkowicie losowe od 1 do 100
    normalCase(30, 100, 10, &gen);   //druga paczka - liczba osob w grupie: 30; oceny losowe od 1 do 100 z rozkladem normalnym (50,10)
    randomCase(3000, 6); //trzecia paczka - liczba osob w grupie: 3000; oceny calkowicie losowe od 1 do 6
    normalCase(3000, 6, 1.5, &gen); //czwarta paczka - liczba osob w grupie: 3000; oceny losowe od 1 do 6 z rozkladem normalnym (3.5, 1.5)
    normalCase(30, 199, 15, &gen); //piata paczka - rozklad jak IQ Wechslera - liczby losowe z rozkladem normalnym o sredniej 100 i odchyleniu standardowym 15; 30 osob
    normalCase(3000, 199, 15, &gen); //szosta paczka - jak piata tylko 3000 osob
    normalCase(300000, 199, 15, &gen); //siodma paczka - jak piata tylko 300000 osob
    
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