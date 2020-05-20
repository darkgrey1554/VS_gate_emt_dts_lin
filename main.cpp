#include <cstdio>
#include "tcp_unitL.h"
#include <unistd.h>

int main()
{

    float* out = new float[100];
    float* in = new float[100];
    for (int i = 0; i < 100; i++)
    {
        out[i] = i;
    }

    std::cout << "Hello World!\n";

    tcp_unit* ser1 = tcp_unit::create_tcp_unit("Server", 1, "127.0.0.1", 32000, "analog", 100, (char*)out, 0);
    tcp_unit* cli1 = tcp_unit::create_tcp_unit("Client", 1, "127.0.0.1", 32000, "analog", 100, (char*)in, 0);

next:
    sleep(2);
    for (int i = 0; i < 100; i++)
    {
        out[i]++;
    }

    std::cout << in[0] << std::endl;
    std::cout << in[99] << std::endl;

    goto next;
    return 0;
}