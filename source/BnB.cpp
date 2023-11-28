#include <iostream>
#include <climits>
#include <fstream>
using namespace std;

const int tabSize = 100;

int final_path[tabSize]; // ostateczne rozwiązanie - odbyta droga
bool visited[tabSize]; // tablica wizyt
int final_res = INT_MAX; // ostatecznie rozwiązanie - waga drogi
int final_res_compare = final_res;
int minIncoming(int adj[tabSize][tabSize], int currentVertex, int size)
{
	int min = INT_MAX;
	for(int i = 0; i < size; i++)
	{
		if(adj[i][currentVertex] < min && adj[i][currentVertex] > 0)
			min = adj[i][currentVertex];
	}
	return min;
}

// Funkcja szukająca pierwszego minimum w danym wierzchołku
int minOutgoing(int adj[tabSize][tabSize], int currentVertex, int size)
{
	int min = INT_MAX;
	for (int i = 0; i < size; i++)
		if (adj[currentVertex][i] < min && adj[currentVertex][i] > 0) // Jeżeli droga istnieje i jest mniejsza od minimum -> ustawiamy minimum na daną drogę
			min = adj[currentVertex][i];
	return min;
}

void TSPCycle(int adj[tabSize][tabSize], int curr_bound, int curr_weight, int level, int curr_path[], int size) // nowe argumety: curr_weight - obecna waga, iteration - liczba przebytych cyklów
{    
	if (level == size) // Jeżeli przeszliśmy przez wszystkie wierzchołki w danej iteracji (liczba cykli jest równa ilości wierzchołków) aktualizujemy ostateczne rozwiązanie i powracamy 
	{
		if (adj[curr_path[level-1]][curr_path[0]] != 0) // upewniamy się że istnieje droga od końcowego do początkowego wierzchołka (czyli jest to cykl hamiltona)
		{
			int curr_res = curr_weight + adj[curr_path[level-1]][curr_path[0]]; // dodajemy wagę drogi między ostatnim a początkowym wierzchołkiem

			if (curr_res < final_res) // jeżeli otrzymana waga jest mniejsza od wagi ostatecznego rozwiązania - nadpisujemy ostatecze rozwiązanie
			{
				for (int i=0; i < size; i++)
		            final_path[i] = curr_path[i];
	            final_path[size] = curr_path[0];
				final_res = curr_res;
			}
		}
		return;
	}

	for (int i=0; i < size; i++)
	{
		if (adj[curr_path[level-1]][i] > 0 && visited[i] == false) // sprawdzamy każdą istniejącą drogę do nieodwiedzonego wierzchołka wychodzącą od obecnego wierzchołka
		{
			int temp = curr_bound;
			curr_weight += adj[curr_path[level-1]][i];
			
			curr_bound -= (minIncoming(adj, i, size) + minOutgoing(adj, i, size))/2;
			if (curr_bound + curr_weight < final_res)
			{
				curr_path[level] = i;
				visited[i] = true;

				TSPCycle(adj, curr_bound, curr_weight, level+1, curr_path, size);
			}
			// W przypadku gdy nie znaleziono mniejszego ograniczenia lub wracamy resetujemy wszystkie potencjalnie zmienione informacje 
			curr_weight -= adj[curr_path[level-1]][i]; // reset wagi
			curr_bound = temp;                         // reset ograniczenia
			for (int i = 0; i < size; i++) // reset tablicy wizyt do stanu obecnej ścieżki
                visited[i] = false;
            for(int i = 0; i < level; i++)
			    visited[curr_path[i]] = true;
		}
	}
}
void TSP(int graf[tabSize][tabSize], int size)
{
	int curr_path[tabSize]; // obecna ścieżka
	int curr_bound = 0; // obecne ograniczenie

    for(int i = 0; i < size+1; i++) 
        curr_path[i] = -1;
	for(int i = 0; i < size+1; i++)
        visited[i] = false;

	// Kalkulacja pierwszego ograniczenia 
    // (według danej formuły: (pierwsze minimum wierzchołka + drugie minimum wierzchołka)/2
	for (int i=0; i < size; i++)
        curr_bound += (minIncoming(graf, i, size) + minOutgoing(graf, i, size))/2;

	visited[0] = true; // zaczynamy w wierzhołku 0
	curr_path[0] = 0;

	TSPCycle(graf, curr_bound, 0, 1, curr_path, size);
    return;
}
clock_t test(int graf[tabSize][tabSize], int size) 
{
    clock_t time;
    time = clock();
    final_res = INT_MAX;
    TSP(graf, size);
    return clock() - time;   
}

int main()
{
	int graf[tabSize][tabSize]; // graf 
    int numberOfTests; // ilośc wykonywanych testów
    string filename;
    ifstream in_plik;
    in_plik.open("config.cfg");
    if(!in_plik)
    {
        cout << "nie ma takiego pliku";
        return 0;
    }
    in_plik >> numberOfTests;
    in_plik >> filename;
    in_plik.close();
    in_plik.open(filename);
    if(!in_plik)
    {
        cout << "nie ma takiego pliku";
        return 0;
    }
    int size; // wielkość grafu
    in_plik >> size;
    for(int i = 0; i < size; i++ )
    {
        for(int j = 0; j < size; j++)
        {
            in_plik >> graf[i][j];
        }
    }
    in_plik.close();

	clock_t time, timesum = 0;
    filename = "wyniki/czasy.txt";
    ofstream of_plik;
   
    of_plik.open(filename, ios::in | std::ofstream::trunc);
    of_plik << numberOfTests << "\n";
    for(int i = 0; i < numberOfTests; i++)
    {
        time = test(graf, size);
        timesum += time;
        of_plik << ((float)time)/CLOCKS_PER_SEC << "\n";
    }
    of_plik << ((float)timesum)/(numberOfTests*CLOCKS_PER_SEC);
    of_plik.close();
	
	cout << "Odpowiedz: " << final_res;
	cout << "\nOdbyta droga: ";
	for (int i=0; i <= size; i++)
		cout << final_path[i] << " ";
	cout << "\n";
	return 0;
}
