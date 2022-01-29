#include <iostream>
#include <math.h>
#include <sstream>
#include <omp.h>
#include <chrono>
#include <pthread.h>

#define NUMBER_OF_CORES 8

using namespace std::chrono;

struct argsP
{
    int ini;
    int fin;
    double *sum;
};

void *suma(void *input)
{

    for (int i = ((struct argsP *)input)->ini; i < ((struct argsP *)input)->fin; i++)
    {
        *((struct argsP *)input)->sum += (pow(-1, i) / (2 * i + 1));
    }
    return NULL;
}

int main()
{
    int max;
    double sum, sum2;
    double sum3 = 0, sum4 = 0;
    std::string ent;
    max = 1000;

    std::cout << "Insert n iterations" << std::endl;

    std::cin >> ent;

    max = stoi(ent);
    std::cout << "\n---- Using " << NUMBER_OF_CORES << " cores ----" << std::endl;

    pthread_t tids[NUMBER_OF_CORES];
    struct argsP *argumentosArray[NUMBER_OF_CORES];
    auto start1 = high_resolution_clock::now();

    for (int i = 0; i < NUMBER_OF_CORES; i++)
    {
        argumentosArray[i] = (struct argsP *)malloc(sizeof(struct argsP));
        argumentosArray[i]->ini = (int)i * (double)max / NUMBER_OF_CORES;
        argumentosArray[i]->fin = (int)(i + 1) * (double)max / NUMBER_OF_CORES;
        argumentosArray[i]->sum = new double(0);
        pthread_create(&tids[i], NULL, suma, (void *)argumentosArray[i]);
    }
    double total = 0;
    for (int i = 0; i < NUMBER_OF_CORES; i++)
    {
        pthread_join(tids[i], NULL);
        total += *(argumentosArray[i]->sum);
    }
    total *= 4;
    auto stop1 = high_resolution_clock::now();
    auto duration1 = duration_cast<microseconds>(stop1 - start1);

    std::cout << "\nWith POXIS Threads: " << std::endl;
    std::cout << "Aproximation: " << total << std::endl;
    std::cout << "Duracion:" << duration1.count() << "[uS] \n"
              << std::endl;
    std::cout << "With OMP:" << std::endl;

    // omp_set_num_threads((NUMBER_OF_CORES))
    // Parallel
    auto start = high_resolution_clock::now();
#pragma omp parallel for reduction(+ \
                                   : sum) num_threads(NUMBER_OF_CORES)
    for (int i = 0; i < max; i++)
    {
        sum = sum + (pow(-1, i) / (2 * i + 1));
    }
    sum = sum * 4;
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    std::cout << "Aproximation: " << sum << std::endl;
    std::cout << "Duracion:" << duration.count() << "[uS]" << std::endl;

    std::cout << "\n\nSingle-core:" << std::endl;

    // One-core
    auto start2 = high_resolution_clock::now();
    for (int i = 0; i < max; i++)
    {
        sum2 = sum2 + (pow(-1, i) / (2 * i + 1));
    }
    sum2 = sum2 * 4;
    auto stop2 = high_resolution_clock::now();
    auto duration2 = duration_cast<microseconds>(stop2 - start2);
    std::cout << "Aproximation: " << sum2 << std::endl;
    std::cout << "Duracion:" << duration2.count() << "[uS]" << std::endl;

    return 0;
}