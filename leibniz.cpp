#include <iostream>
#include <math.h>
#include <sstream>
#include <omp.h>
#include <chrono>
using namespace std::chrono;

int main(){
    int max;
    double sum, sum2;
    std::string ent;
    max =1000;

    std::cout << "Insert n iterations" << std::endl;

    std::cin >> ent;

    max = stoi(ent);
    //omp_set_dynamic(0); 
    //omp_set_num_threads(4);
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