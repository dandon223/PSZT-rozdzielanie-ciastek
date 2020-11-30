# PSZT rozdzielanie ciastek
Nauczycielka w przedszkolu chce rozdać ciastka dzieciom w swojej grupie. Dzieci siedzą w linii obok siebie (i nie zmieniają tych pozycji). Każde dziecko ma przypisaną ocene s i , i ∈ (1, 2, ..., n), zgodnie z wynikiem testu umiejetności. Nauczycielka chce dać każdemu dziecku co najmniej jedno ciastko. Jeśli dzieci siedzą obok siebie, dziecko z wyższą ocena musi dostać więcej ciastek niż to z nizszą oceną. Nauczycielka ma ograniczony budżet, więc chce rozdać jak najmniej ciastek. Zaimplementuj program bazujacy na Algorytmie Ewolucyjnym, który zwróci najmniejszą liczbę ciastek, które musi rozdać nauczycielka. Zastosowanie dodatkowego algorytmu i porównanie będzie dodatkowym atutem przy ocenie projektu.

Zastosowano dwa podejścia rozwiązania tego problemu: iteracyjne oraz ewolucyjne.

# Kompilowalie oraz uruchomienie programu
W projekcie znajduje się plik Makefile który umożliwia skompilowanie projektu przy pomocy polecenia make.
Po skompilowaniu plik wynikowy znajduje się w katalogu bin i ma nazwę main
Program możemy uruchomić wpisując polecenie ./bin/main
Program możemy uruchamiać z różnymi parametrami
    -g - tryb generowania testów, jeśli go uruchomimy, zostaną wygenerowane od nowa testy
    opis testów znajduje się w sekcji poszukiwanie optymalnych parametrów
    
-t - tryb testowania, podczas jego uruchomienia mierzone są wyniki dla wygenerowanych testów oraz różnych parametrów algorytmu ewolucyjnego, domyślne parametry testowania to:
    początkowy współczynnik mutacji = 0.05
    liczba różnych współczynników = 8
    początkowa wielkość populacji = 15
    liczba różnych populacji = 3
    okres próbkowania (pobierania kolejnych wyników) = 0.1s
    liczba okresów próbkowania = 35
    
    W kolejnych testach, sprawdzane będą współczynniki mutacji oraz wielkości populacji 2 razy większe niż poprzednie wartości czyli dla podstawowych parametrów zostaną przetestowane wszystkie wariacje wielkości populacji: 15, 30, 60 oraz współczynników mutacji: 0.05, 0.1, 0.2, 0.4, 0.8, 1.6, 3.2, 6.4
    podstawowe wartości możemy  modyfikować używając flag:
        -v <liczba>  dodanie uruchomienia wersji mutacji nr <liczba> do listy mutacji do przetestowania (jeśli nie dodamy żadnej mutacji, żadna nie zostanie przetestowana)
        -p <liczba> liczba okresów próbkowania
        -m <liczba> podstawowy współczynnik mutacji
    przykładowe uruchomienie dla tego trybu:
        ./bin/main -t -v 1 -v 3 -p 20 -m 0.5

OSTRZEŻENIE - działanie programu w trybie testów może trwać kilka godzin

    -r - tryb uruchamiania z najlepszymi znalezionymi przez nas parametrami wykonania
        współczynnik mutacji = 1.6
        wielkość populacji = 5
        mutacja w wersji nr 1
        
        W tym trybie wejście należy umieścić na standardowym wejściu, wpisując w jednym wierszu liczby będące ocenami kolejnych uczniów

        Wynik zostanie wyświetlony na standardowym wyjściu w postaci:
układ ciastek:
<n liczb reprezentujących ile ciastek zostało przydzielonych kolejnemu dziecku> 
razem:
<suma przydzielonych ciastek>

