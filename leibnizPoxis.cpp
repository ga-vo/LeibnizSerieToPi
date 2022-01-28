#include <iostream>
#include <math.h>
#include <sstream>
#include <omp.h>
#include <chrono>
#include <pthread.h>

using namespace std::chrono;

struct argsP {
    int ini;
    int fin;
    double sum;
};

double *suma(void *input){
    for(int i = ((struct argsP*)input)->ini; i<((struct argsP*)input)->fin;i++){
        ((struct argsP*)input)->sum += (pow(-1,i)/(2*i+1));
    }
    
    double *sum = ((struct argsP*)input)->sum;
    
    return *sum;
}

int main(){
    int max;
    double sum, sum2, *sum3;
    std::string ent;
    max =1000;
    struct argsP *argumentos = (struct argsP *)malloc(sizeof(struct argsP));
    
    std::cout << "Insert n iterations" << std::endl;

    std::cin >> ent;

    max = stoi(ent);
    
    
    
    argumentos->ini = 0;
    argumentos->fin = (int)max/2;
    argumentos->sum = *sum3;
    pthread_t tid;
    pthread_create(&tid, NULL, suma, (void *)argumentos);
    pthread_join(tid, NULL);
    
    std::cout << "TEST POXIS: " << sum3 << "\n" << std::endl;
    
    std::cout << "With OMP:" << std::endl; 
    std::cout << "Max of threats: " << omp_get_max_threads() << std::endl;
    
    
    // Parallel
    auto start = high_resolution_clock::now();
    #pragma omp parallel for reduction(+: sum)
    for (int i=0;i<max;i++){
        sum = sum + (pow(-1,i)/(2*i+1));
    }
    sum = sum *4;
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    std::cout << "Aproximation: " << sum << std::endl;
    std::cout << "Duracion:" << duration.count() << std::endl;

    std::cout << "\n\nWithout OMP:" << std::endl; 
    
    // One-core
    auto start2 = high_resolution_clock::now();
    for (int i=0;i<max;i++){
        sum2 = sum2 + (pow(-1,i)/(2*i+1));
    }
    sum2 = sum2 *4;
    auto stop2 = high_resolution_clock::now();
    auto duration2 = duration_cast<microseconds>(stop2 - start2);
    std::cout << "Aproximation: " << sum2 << std::endl;
    std::cout << "Duracion:" << duration2.count() << std::endl;

    return 0;
}