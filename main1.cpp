#include <bits/stdc++.h>
#include <sys/time.h>
#include <random>

#define para std::pair<long long, long long>

std::string nazwa;//nazwa pliku

const int MAXN=500000;

int INF=INT_MAX;
int V = 0, G = 0;

int tab[500][500];
long long d[MAXN+5]; // tablica odlegleglosci od wierzcholka poczatkowego do kazdego kolejnego wierzcholka

std::vector<std::vector<para>> graf(500);
std::priority_queue <para> kolejka;

//losowanie grafu
void shuffle(std::string nazwa_pliku)
{
    std::ofstream zapis(nazwa_pliku);
    zapis<<V<<" "<<G<<std::endl;
    std::mt19937 rng{std::random_device{}()};
    std::uniform_int_distribution<> dist(0, V - 1);

    std::vector<std::pair<int, int>> edges;
    for(int i = 0; i < G; i++){
        int a = dist(rng);
        int b = dist(rng);
        if(a!=b)
        edges.emplace_back(a,b);
    }
    std::shuffle(edges.begin(), edges.end(), rng);
    for(const auto& [a,b] : edges){
        int waga = dist(rng) +1;
        zapis<<a<<" "<<b<<" "<<waga<<std::endl;
    }
    zapis.close();
}

// tworzenie krawedzii grafa i przyporzadkowania wagi
void addEdge_L(int u, int v, int wt){
    graf[u].push_back(std::make_pair(v, wt));
}
void addEdge_M(int x, int y, int z){
    tab[x][y]=z;
}

// wczytywanie danych do struktury listowej
void load_L(){
    std::ifstream read(nazwa);
    int edge=0, V2=0, waga=0,V1=0;
    read>>V; read>>G;
    edge=((V*G/100)*(V-1))/2;
    graf.clear();
    graf.resize(V);
    for(int i=0; i<edge; i++){
        read>>V1;
        read>>V2;
        read>>waga;
        addEdge_L(V1, V2, waga);
    }
    read.close();

}

//wczytywanie danych do struktury macierzowej
void load_M(){
    std::ifstream read(nazwa);
    int edge=0, V2=0, waga=0, V1=0;
    read>>V;
    read>>G;
    edge=((V*G/100)*(V-1))/2;
    for(int i=0; i<V;i++){
        for(int j =0; j<V;j++){
            tab[i][j]=0;
        }
    }
    for(int i=0; i<edge; i++){
        read>>V1;
        read>>V2;
        read>>waga;

        addEdge_M(V1, V2, waga);
    }
    read.close();
}



//wypisywanie grafu w reprezentacji listowej
void printGraph_L(){
    int v=0, waga=0;
    for (int u = 0; u < V; u++){
        std::cout << u << "--> \n";
        for (auto it = graf[u].begin(); it!=graf[u].end(); it++){
            v = it->first;
            waga = it->second;
            std::cout << "\t" << v << " = "<< waga << "\n";
        }
    }
    std::cout<<"\n";
}

//wypisywanie grafu w reprezentacji macierzowej
void printGraph_M(){
    for(int i=0; i<V; i++){
        for(int j=0; j<V; j++){
            if(j==0){
                std::cout<<"|";
            }
            if(tab[i][j]==INF){
                std::cout<<" INF ";
            }else{
                std::cout<<" "<<tab[i][j]<<" ";
            }
            if(j==V-1){
                std::cout<<"|"<<std::endl;
            }
        }
    }
}
//wykonywanie algorytmu dijkstry dla reprezentacji listowej
void Dijkstra_L(int s)//wierzcholek poczatkowy
{
    bool odw[MAXN+5];//sprawdzanie odwiedzin
    for(int i=0;i<MAXN; i++){
        odw[i]=false;
        d[i]=INF;
    }
    //odleglosc od pierwszego wierzcholka
    d[s]=0;
    kolejka.push({0, s});
    while(!kolejka.empty())
        {
        int x=kolejka.top().second;
        kolejka.pop();
        if(!odw[x])
            {
            odw[x]=true;
            for(int i=0; i<graf[x].size(); i++)
                {
                int v = graf[x][i].first;
                int waga = graf[x][i].second;
                if(d[v]>d[x]+waga)
                    {
                    d[v]=d[x]+waga;
                    }
                kolejka.push({-d[v], v});
                }
            }
        }

    }

//wykonywanie algorytmu dijkstry dla reprezentacji macierzowej
void Dijkstra_M(int s){
    int d[V];
	bool Tset[V];
	for(int i = 0; i<V; i++){
		d[i] = INT_MAX;
		Tset[i] = false;
	}
	d[s] = 0;
	for(int i = 0; i<V; i++){
        int m = INT_MAX;
        int index;
		for(int j=0;j<V;j++){
            if(Tset[j]==false && d[j]<=m){
                m=d[j];
                index=j;
            }
        }
		Tset[index]=true;
		for(int k = 0; k<V; k++){
			if(!Tset[k] && tab[index][k] && d[index]!=INT_MAX && d[index]+tab[index][k]<d[k]){
				d[k]=d[index]+tab[index][k];
			}
		}
	}
}

int main(){
    int wybor;
    while (true){
        std::cout<<"------------------------------------"<<std::endl;
        std::cout<<"Menu wyboru:"<<std::endl;
        std::cout<<"------------------------------------"<<std::endl;
        std::cout<<"  1 - Wygeneruj graf"<<std::endl;
        std::cout<<"  2 - Wczytaj graf i wyswietl(listowo/macierzowo)"<<std::endl;
        std::cout<<"  3 - Usredniony czas wykonywania programu dla 100 powtorzen"<<std::endl;
        std::cout<<"------------------------------------"<<std::endl;
        std::cin>>wybor;

        switch(wybor)
        {
        case 1:
                std::cout << "Podaj ilosc elementow grafu: " << std::endl;
                std::cin >> V;
                std::cout << "Podaj gestosc grafu (0-100%): " << std::endl;
                std::cin >> G;
                std::cout << "Wpisz nazwe pliku: " << std::endl;
                std::cin >> nazwa;
                shuffle(nazwa);
                std::cout << "Graf zostal wygenerowany" << std::endl;
                break;
        case 2:
                std::cout << "Podaj nazwe pliku: " << std::endl;
                std::cin >> nazwa;
                load_L(); load_M();
                std::cout << "Pomyslnie wczytano dane z pliku\n"<< std::endl;
                printGraph_L(); printGraph_M();
                break;
        case 3:
                int s;
                std::cout << "Podaj nazwe pliku: " << std::endl;
                std::cin >> nazwa;
                std::cout << "Podaj ilosc wierzcholkow: " << std::endl;
                std::cin >> V;
                std::cout << "Podaj gestosc grafu (0-100%): " << std::endl;
                std::cin >> G;
                std::cout << "Podaj wierzcholek poczatkowy: " << std::endl;
                std::cin >> s;
                struct timeval start, stop;
                long seconds;
                long microseconds;
                double M, L;
                for (int i=0; i<100; i++)
                    {
                        shuffle(nazwa);
                        gettimeofday(&start, 0);
                        Dijkstra_L(s);
                        gettimeofday(&stop, 0);
                        seconds = stop.tv_sec - start.tv_sec;
                        microseconds = stop.tv_usec - start.tv_usec;
                double czasL = seconds + microseconds*1e-6;
                L=L+czasL;
                        gettimeofday(&start, 0);
                        Dijkstra_M(s);
                        gettimeofday(&stop, 0);
                        seconds = stop.tv_sec - start.tv_sec;
                        microseconds = stop.tv_usec - start.tv_usec;
                double czasM = seconds + microseconds*1e-6;
                M=M+czasM;

                }
                std::cout << "Czas dla reprezentacji listowej: "<<L/100<<std::endl;
                std::cout << "Czas dla reprezentacji macierzowej: "<<M/100<<std::endl;

            break;
                }
        }

        return 0;
}
