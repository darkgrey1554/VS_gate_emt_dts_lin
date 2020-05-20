#include "tcp_unitL.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>


tcp_unit* tcp_unit::create_tcp_unit(std::string type_unit, int id, std::string ip, int port, std::string t_data, int s_data, char* mass_data, int bais)
{
    if (type_unit == "Server") return new tcp_server(id, ip, port, t_data, s_data, mass_data, bais);
    if (type_unit == "Client") return new tcp_client(id, ip, port, t_data, s_data, mass_data, bais);
}

tcp_server::tcp_server(int id, std::string ip, int port, std::string t_data, int s_data, char* m_data, int bais)
{
    ID = id;
    IP_Server = ip;
    PORT = port;
    size_data = s_data;
    mass_data = m_data;
    type_data = t_data;
    offset = bais * 4; /// пока умнодение на 4 (учет размера int float)
    thread_unit = std::thread(&tcp_server::thread_tcp_server, this);
}

int tcp_server::thread_tcp_server()
{
    int result = 0;
    int listensocket;
    int client;
    sockaddr_in addr_server;
    sockaddr_in addr_client;
    int lenght_addr_client = sizeof(addr_client);
    char* buf_recv = new char[10];
    int count_recv = 0;
    int num_recv = 1;
    char* buf_send = new char[size_data * 4 + 4 + 1];
    char* ibuf_send;
    char* imass_data;
    float* f;

    listensocket = socket(AF_INET, SOCK_STREAM, 0);
    if (listensocket == -1)
    {
        std::cout << "SERVER ID: " << ID << "/tERROR INITIALIZATION CODE ERROR: " << std::endl;
        return -1;
    }

    addr_server.sin_family = AF_INET;
    addr_server.sin_addr.s_addr = htonl(INADDR_LOOPBACK); //inet_addr(IP_Server.c_str());
    addr_server.sin_port = htons(PORT);

    result = bind(listensocket, (sockaddr*)&addr_server, sizeof(addr_server));
    if (result == -1)
    {
        std::cout << "SERVER ID: " << ID << "\tERROR INITIALIZATION CODE ERROR: " << errno <<std::endl;
        return -1;

    }

    if (listen(listensocket, 1) == -1)
    {
        std::cout << "SERVER ID: " << ID << "\tERROR INITIALIZATION CODE ERROR: " << std::endl;
        //close(listensocket);
        //closesocket(listensocket);
        return -1;
    }

    std::cout << "SERVER INITIALIZED ID: " << ID
        << " IP: " << IP_Server
        << " PORT: " << PORT
        << " TYPE_DATA: " << type_data << std::endl;

    for (;;)
    {
        client = accept(listensocket,NULL,NULL);//client = accept(listensocket, (sockaddr*)&addr_client, &lenght_addr_client);
        if (client == -1)
        {
            std::cout << "SERVER ID: " << ID << "\tERROR CONNECTION CLIENT CODE ERROR: " << std::endl;
            continue;
        }

        std::cout << "SERVER ID: " << ID << "\tCONNECTION WITH CLIENT IP: "
            //<< (int)addr_client.sin_addr.S_un.S_un_b.s_b1 << "."
            //<< (int)addr_client.sin_addr.S_un.S_un_b.s_b2 << "."
            //<< (int)addr_client.sin_addr.S_un.S_un_b.s_b3 << "."
            //<< (int)addr_client.sin_addr.S_un.S_un_b.s_b4
            << "  PORT: " << addr_client.sin_port << std::endl;

        for (;;)
        {
            count_recv = 0;
            for (;;)
            {
                count_recv += recv(client, buf_recv + count_recv, 1, NULL);
                if (count_recv == num_recv) break;
                if (count_recv == 0) break;
            }
            if (count_recv == 0)
            {
                std::cout << "SERVER ID: " << ID << "/tERROR RECV CODE ERROR: " << std::endl;
                break;
            }

            if (buf_recv[0] == 3)
            {
                ibuf_send = buf_send;
                *ibuf_send = 3;
                for (int i = 0; i < 4; i++)
                {
                    ibuf_send++;
                    *ibuf_send = *(((char*)&size_data) + i);
                }

                ibuf_send++;
                imass_data = mass_data;
                for (int i = 0; i < size_data * 4; i++)
                {
                    *ibuf_send = *imass_data;
                    ibuf_send++;
                    imass_data++;
                }

                send(client, buf_send, size_data * 4 + 5, NULL);
            }
        }
    }

    return 0;
}


void tcp_server::restart_thread()
{
    std::cout << "restart_thread" << std::endl;
}

void tcp_server::close_tcp_unit()
{
    std::cout << "close_tcp_unit" << std::endl;
}




tcp_client::tcp_client(int id, std::string ip, int port, std::string t_data, int s_data, char* m_data, int bais)
{
    ID = id;
    IP_Server = ip;
    PORT = port;
    size_data = s_data;
    mass_data = m_data;
    type_data = t_data;
    offset = bais * 4; /// пока умнодение на 4 (учет размера int float)
    thread_unit = std::thread(&tcp_client::thread_tcp_client, this);
}

int tcp_client::thread_tcp_client()
{
    int result = 0;
    int server;
    sockaddr_in addr_server;
    int size_addr = sizeof(addr_server);

    char* buf_recv = new char[size_data * 4 + 5];
    int count_recv = 0;
    int num_recv = 0;
    char* ibuf_recv;
    char* imass_data;

    char* buf_send = new char[1];
    float* f;

    server = socket(AF_INET, SOCK_STREAM, 0);
    if (server == -1)
    {
        std::cout << "CLIENT ID: " << ID << "/tERROR INITIALIZATION CODE ERROR: "<< std::endl;
        return -1;
    }

    addr_server.sin_family = AF_INET;
    addr_server.sin_addr.s_addr = htonl(INADDR_LOOPBACK);//inet_addr(IP_Server.c_str());
    addr_server.sin_port = htons(PORT);

    for (;;)
    {

        if (connect(server, (sockaddr*)&addr_server, sizeof(addr_server)) == -1)
        {
            std::cout << "CLIENT ID: " << ID << "/tERROR CONNECTION WITH SERVER CODE ERROR: " << std::endl;
            //closesocket(server);
            //Sleep(2000);
            continue;
        }
        else
        {
            std::cout << "CLIENT ID: " << ID << " CONNECTED WITH SERVER IP: " << IP_Server << " PORT: " << PORT << std::endl;
        }

        for (;;)
        {
            buf_send[0] = 3;
            send(server, buf_send, 1, NULL);
            count_recv = 0;
            for (;;)
            {
                count_recv += recv(server, buf_recv + count_recv, size_data * 4 + 5 - count_recv, NULL);
                if (count_recv < 5) continue;
                num_recv = *((int*)(buf_recv + 1));
                if (count_recv < num_recv * 4 + 5) { continue; }
                else break;
            }
            ibuf_recv = buf_recv + 5;
            imass_data = mass_data;

            for (int i = 0; i < num_recv * 4; i++)
            {
                *imass_data = *ibuf_recv;
                imass_data++;
                ibuf_recv++;
            }

            sleep(2);

        }
    }

    return 0;
}

void tcp_client::restart_thread()
{
    std::cout << "restart_thread" << std::endl;
}

void tcp_client::close_tcp_unit()
{
    std::cout << "close_tcp_unit" << std::endl;
}