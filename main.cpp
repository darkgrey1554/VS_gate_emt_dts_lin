#include <cstdio>
#include "tcp_unitL.h"
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

int main()
{
    /*timeval timelast;
    timeval timenow;
    float time = 0.;
    char c;

    gettimeofday(&timelast, NULL);
    sleep(2);
    usleep(15000);
    gettimeofday(&timenow, NULL);
    
    time = (timenow.tv_sec - timelast.tv_sec) * 1000.0 + (timenow.tv_usec - timelast.tv_usec) / 1000.0;
    std::cout << time << std::endl;
    std::cin >> c;*/


    pthread_mutex_t mut_out = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_t mut_in = PTHREAD_MUTEX_INITIALIZER;
    
    std::list<ConfigSharedMemory> memory;
    std::list<ConfigUnitGate> gaties;

    TypeSignal m = TypeSignal::Analog;

    if (read_config_file("conf.txt", &memory, &gaties) != 0)
    {
        std::cout << "ERROR READ CONFIG FILE" << std::endl;
        return -1;
    }
    

    float* out = new float[100];
    /*float* in = new float[100];
    for (int i = 0; i < 100; i++)
    {
        out[i] = i;
    }*/

    std::cout << "Hello World!\n";

    for (auto iter = gaties.begin(); iter != gaties.end(); iter++)
    {
        if ((&*iter)->type_unit == TypeUnitGate::SERVER)
        {
            (&*iter)->buf_data = (char*)out;
            (&*iter)->mutex_data = mut_out;
        }
        if ((&*iter)->type_unit == TypeUnitGate::CLIENT)
        {
            (&*iter)->buf_data = (char*)out;
            (&*iter)->mutex_data = mut_out;
        }  
    }

    /*
    for (auto iter = gaties.begin(); iter != gaties.end(); iter++)
    {
        if ((&*iter)->type_unit == TypeUnitGate::SERVER) std::cout << (int*)(&*iter)->buf_data << "  ---  " << out << std::endl;
        if ((&*iter)->type_unit == TypeUnitGate::CLIENT) std::cout << (int*)(&*iter)->buf_data << "  ---  " << in << std::endl;

        std::cout << (&*iter)->type_unit << " " << (&*iter)->Port << " " << (&*iter)->IP << std::endl;
    }*/


    std::list<tcp_unit*> units_gate;
    int id = 0;
    for (auto iter = gaties.begin(); iter != gaties.end(); iter++)
    {
        units_gate.push_back(tcp_unit::create_tcp_unit(*iter, id));
        id++;
    }

next:
    sleep(2);

    /*pthread_mutex_lock(&mut_out);
    for (int i = 0; i < 100; i++)
    {
        out[i]++;
    }
    pthread_mutex_unlock(&mut_out);

    pthread_mutex_lock(&mut_in);
    std::cout << in[0] << std::endl;
    std::cout << in[99] << std::endl;
    pthread_mutex_unlock(&mut_in);
    */
    goto next;
    return 0;
}