#include "tcp_unitL.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/time.h>



///  --- вспомогательные функции --- ///
int conver_IPstr_int(const char* str)
{
    int res = 0;
    int var = 0;
    int count = 0;
    int count2 = 0;
    const char* simvol = str;


    while (*simvol != '\000')//while (*simvol != '\0')
    {
        while (*simvol != '\000' && *simvol != '.') //while (*simvol != '\0' && *simvol != '.')
        {
            if (*simvol < 0x30 || *simvol > 0x39) return -1;
            var *= 10;
            var += *simvol - 0x30;
            count++;
            simvol++;
        }
        if (*simvol == '.') simvol++;
        count2++;
        if (count > 3) return -1;
        if (count2 > 4) return -1;
        count = 0;
        res = res << 8;
        res += var;
        var = 0;
    }
    return res;
}

std::ostream& operator<<(std::ostream& out, TypeSignal& m)
{
    if (m == TypeSignal::Analog) out << "Analog";
    if (m == TypeSignal::Discrete) out << "Discrete";
    if (m == TypeSignal::Nothing) out << "Nothing";
    return out;
}

std::ostream& operator<<(std::ostream& out, TypeUnitGate& m)
{
    if (m == TypeUnitGate::CLIENT) out << "CLIENT";
    if (m == TypeUnitGate::SERVER) out << "SERVER";
    if (m == TypeUnitGate::EMPTY) out << "EMPTY";
    return out;
}

int read_config_file(const char* Namefile, std::list<ConfigSharedMemory>* listsharmem, std::list<ConfigUnitGate>* listunitgate)
{
    FILE* config_file = NULL;
    char simvol = 0;
    std::string str_info;
    std::string helpstr;
    int res_read = 0;
    int pos[2] = { 0,0 };
    ConfigSharedMemory UnitMem;
    ConfigUnitGate UnitGate;

    config_file = fopen(Namefile, "r");
    if (config_file == NULL) return -1;

    for (;;)
    {
        simvol = 0;
        str_info.clear();
        while (simvol != '\n' && res_read != EOF)
        {
            res_read = fscanf(config_file, "%c", &simvol);
            if ((simvol > 0x1F || simvol == '\t') && res_read != EOF) str_info += simvol;
        }

        if (str_info == "[List]" || res_read == EOF || str_info == "[GATE]" && listunitgate == NULL)
        {
            if (res_read == EOF)
            {
                std::cout << "MAIT\tERROR_FORMATION_OF_CONFIG_FILE" << std::endl;
                return -1;
            }
            break;
        }

        if (str_info.substr(0, 4) == "@EMT")
        {
            pos[0] = 0;
            pos[1] = 0;
            UnitMem.clear();

            if (str_info.find("Input") != -1)
            {
                UnitMem.type_data = TypeData::InPut;
            }
            else if (str_info.find("Output") != -1)
            {
                UnitMem.type_data = TypeData::OutPut;
            }
            else
            {
                std::cout << "MAIT\tERROR_FORMATION_OF_CONFIG_FILE" << std::endl;
                return -1;
            }

            if (str_info.find("Discrete") != -1)
            {
                UnitMem.type_signal = TypeSignal::Discrete;
            }
            else if (str_info.find("Analog") != -1)
            {
                UnitMem.type_signal = TypeSignal::Analog;
            }
            else
            {
                std::cout << "MAIT\tERROR_FORMATION_OF_CONFIG_FILE" << std::endl;
                return -1;
            }

            pos[0] = str_info.find('\t', 0);
            if (pos[0] == -1)
            {
                std::cout << "MAINT\tERROR_FORMATION_OF_CONFIG_FILE" << std::endl;
                return -1;
            }

            pos[1] = str_info.find('\t', (size_t)pos[0] + 1);
            if (pos[1] == -1)
            {
                std::cout << "MAINT\tERROR_FORMATION_OF_CONFIG_FILE" << std::endl;
                return -1;
            }
            helpstr.clear();
            helpstr = str_info.substr((size_t)pos[0] + 1, (size_t)pos[1] - pos[0] - 1);
            UnitMem.size = atoi(helpstr.c_str());
            helpstr.clear();
            helpstr = str_info.substr((size_t)pos[1] + 1);
            UnitMem.name_memory = helpstr;
            listsharmem->push_back(UnitMem);
        }
        else if (str_info.substr(0, 5) == "@GATE")
        {
            pos[0] = 0;
            pos[1] = 0;
            UnitGate.clear();

            if (str_info.find("Input") != -1)
            {
                UnitGate.type_unit = TypeUnitGate::CLIENT;
            }
            else if (str_info.find("Output") != -1)
            {
                UnitGate.type_unit = TypeUnitGate::SERVER;
            }
            else
            {
                std::cout << "MAIT\tERROR_FORMATION_OF_CONFIG_FILE" << std::endl;
                return -1;
            }

            if (str_info.find("Discrete") != -1)
            {
                UnitGate.type_signal = TypeSignal::Discrete;
            }
            else if (str_info.find("Analog") != -1)
            {
                UnitGate.type_signal = TypeSignal::Analog;
            }
            else
            {
                std::cout << "MAIT\tERROR_FORMATION_OF_CONFIG_FILE" << std::endl;
                return -1;
            }

            pos[0] = str_info.find('\t', 0);
            if (pos[0] == -1)
            {
                std::cout << "MAINT\tERROR_FORMATION_OF_CONFIG_FILE" << std::endl;
                return -1;
            }
            pos[1] = str_info.find('\t', (size_t)pos[0] + 1);
            if (pos[1] == -1)
            {
                std::cout << "MAINT\tERROR_FORMATION_OF_CONFIG_FILE" << std::endl;
                return -1;
            }

            helpstr.clear();
            helpstr = str_info.substr((size_t)pos[0] + 1, (size_t)pos[1] - pos[0] - 1);
            UnitGate.offset = atoi(helpstr.c_str());

            pos[0] = pos[1];
            pos[1] = str_info.find('\t', (size_t)pos[0] + 1);
            if (pos[1] == -1)
            {
                std::cout << "MAINT\tERROR_FORMATION_OF_CONFIG_FILE" << std::endl;
                return -1;
            }
            helpstr.clear();
            helpstr = str_info.substr((size_t)pos[0] + 1, (size_t)pos[1] - pos[0] - 1);
            UnitGate.size_data = atoi(helpstr.c_str());

            pos[0] = pos[1];
            pos[1] = str_info.find('\t', (size_t)pos[0] + 1);
            if (pos[1] == -1)
            {
                std::cout << "MAINT\tERROR_FORMATION_OF_CONFIG_FILE" << std::endl;
                return -1;
            }
            helpstr.clear();
            helpstr = str_info.substr((size_t)pos[0] + 1, (size_t)pos[1] - pos[0] - 1);
            UnitGate.IP = helpstr.c_str();

            pos[0] = pos[1];
            pos[1] = str_info.find('\t', (size_t)pos[0] + 1);
            if (pos[1] == -1)
            {
                std::cout << "MAINT\tERROR_FORMATION_OF_CONFIG_FILE" << std::endl;
                return -1;
            }
            helpstr.clear();
            helpstr = str_info.substr((size_t)pos[0] + 1, (size_t)pos[1] - pos[0] - 1);
            UnitGate.Port = atoi(helpstr.c_str());

            pos[0] = pos[1];
            helpstr.clear();
            helpstr = str_info.substr((size_t)pos[0] + 1);
            UnitGate.frequency = atoi(helpstr.c_str());
            listunitgate->push_back(UnitGate);
        }
        else if (str_info.substr(0, 5) != "[EMT]" && str_info.substr(0, 6) != "[GATE]")
        {
            std::cout << "MAIN\tERROR_FORMATION_OF_CONFIG_FILE" << std::endl;
            return -1;
        }
    }
    return 0;
}


/// --- tcp_unit -- ///
tcp_unit* tcp_unit::create_tcp_unit(ConfigUnitGate config, int id)
{
    if (config.type_unit == TypeUnitGate::SERVER) return new tcp_server(config, id);
    if (config.type_unit == TypeUnitGate::CLIENT) return new tcp_client(config, id);
}


/// --- tcp_server --- ///

tcp_server::tcp_server(ConfigUnitGate config, int id)
{
    set = config;
    ID = id;
    if (set.type_signal == TypeSignal::Analog || set.type_signal == TypeSignal::Discrete)
    {
        s_data = 4;
    }
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
    int res_recv = 0;
    int num_recv = 1;
    char* buf_send = new char[set.size_data * s_data + 4 + 1];
    char* ibuf_send;
    char* imass_data;
    float* f;
    int ip;

    timeval timenow;
    timeval timelast;
    float time;


    listensocket = socket(AF_INET, SOCK_STREAM, 0);
    if (listensocket == -1)
    {
        std::cout << "SERVER ID: " << ID << "/tERROR INITIALIZATION CODE ERROR: " << std::endl;
        return -1;
    }

    ip = conver_IPstr_int(set.IP.c_str());
    if (ip == -1)
    {
        std::cout << "SERVER ID: " << ID << "/tERROR INITIALIZATION ERROR: BED IP" << std::endl;
        return -1;
    }
    addr_server.sin_family = AF_INET;
    addr_server.sin_addr.s_addr = htonl(ip); //inet_addr(IP_Server.c_str());
    addr_server.sin_port = htons(set.Port);

    result = bind(listensocket, (sockaddr*)&addr_server, sizeof(addr_server));
    if (result == -1)
    {
        std::cout << "SERVER ID: " << ID << "\tERROR INITIALIZATION CODE ERROR: " << errno <<std::endl;
        return -1;

    }

    if (listen(listensocket, 3) == -1)
    {
        std::cout << "SERVER ID: " << ID << "\tERROR INITIALIZATION CODE ERROR: " << std::endl;
        close(listensocket);
        //closesocket(listensocket);
        return -1;
    }

    std::cout << "SERVER INITIALIZED ID: " << ID
        << " IP: " << set.IP
        << " PORT: " << set.Port
        << " TYPE_SIGNAL: " << set.type_signal << std::endl;

    for (;;)
    {
        client = accept(listensocket,NULL,NULL);//client = accept(listensocket, (sockaddr*)&addr_client, &lenght_addr_client);
        if (client == -1)
        {
            std::cout << "SERVER ID: " << ID << "\tERROR CONNECTION CLIENT CODE ERROR: " << std::endl;
            close(client);
            continue;
        }

        std::cout << "SERVER ID: " << ID << "\tCONNECTION WITH CLIENT IP: "
            //<< (int)addr_client.sin_addr.S_un.S_un_b.s_b1 << "."
            //<< (int)addr_client.sin_addr.S_un.S_un_b.s_b2 << "."
            //<< (int)addr_client.sin_addr.S_un.S_un_b.s_b3 << "."
            //<< (int)addr_client.sin_addr.S_un.S_un_b.s_b4
            << "  PORT: " << addr_client.sin_port << std::endl;

        gettimeofday(&timelast, NULL);
        gettimeofday(&timenow, NULL);
        for (;;)
        {
            /*count_recv = 0;
            res_recv = 0;
            for (;;)
            {
                res_recv= recv(client, buf_recv + count_recv, num_recv- count_recv, NULL);
                if (res_recv == 0) break;
                count_recv+=res_recv;
                if (count_recv == num_recv) break; 
            }
            if (res_recv == 0)
            {
                std::cout << "SERVER ID: " << ID << "/tERROR RECV CODE ERROR: " << std::endl;
                close(client);
                break;
            }*/

            gettimeofday(&timenow, NULL);
            time = (timenow.tv_sec - timelast.tv_sec) * 1000. + (timenow.tv_usec - timelast.tv_usec) / 1000.;
            if (time > set.frequency) std::cout << "SERVER ID: " << ID << "\tWARNING: LIMIT_TIME_MESSENG_READING_EXCEEDED " << time << " ms" << std::endl;

            for (;;)
            {
                gettimeofday(&timenow, NULL);
                time = (timenow.tv_sec - timelast.tv_sec) * 1000. + (timenow.tv_usec - timelast.tv_usec) / 1000.;
                if (time > set.frequency - TIME_DIV) break;
                usleep(1000);
            }
            gettimeofday(&timelast, NULL);

            //if (buf_recv[0] == 3)
            //{
                ibuf_send = buf_send;
                *ibuf_send = 3;
                for (int i = 0; i < 4; i++)
                {
                    ibuf_send++;
                    *ibuf_send = *(((char*)&set.size_data) + i);
                }

                ibuf_send++;
                imass_data = set.buf_data;

                pthread_mutex_lock(&set.mutex_data);
                for (int i = 0; i < set.size_data * s_data; i++)
                {
                    *ibuf_send = *imass_data;
                    ibuf_send++;
                    imass_data++;
                }
                pthread_mutex_unlock(&set.mutex_data);

                send(client, buf_send, set.size_data * s_data + 5, NULL);
            //}
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


/// --- tcp_client --- ///

tcp_client::tcp_client(ConfigUnitGate config, int id)
{
    set=config;
    ID = id;
    if (set.type_signal == TypeSignal::Analog || set.type_signal == TypeSignal::Discrete)
    {
        s_data = 4;
    }
    thread_unit = std::thread(&tcp_client::thread_tcp_client, this);
}

int tcp_client::thread_tcp_client()
{
    int result = 0;
    int server;
    sockaddr_in addr_server;
    int size_addr = sizeof(addr_server);

    char* buf_recv = new char[set.size_data * s_data + 5];
    int count_recv = 0;
    int num_recv = 0;
    char* ibuf_recv;
    char* imass_data;
    char* buf_send = new char[1];
    int ip;
    int res_recv;

    timeval timenow;
    timeval timelast;
    float time;


    ip = conver_IPstr_int(set.IP.c_str());
    if (ip == -1)
    {
        std::cout << "CLIENT ID: " << ID << "/tERROR INITIALIZATION ERROR: BED IP" << std::endl;
        return -1;
    }

    addr_server.sin_family = AF_INET;
    addr_server.sin_addr.s_addr = htonl(ip);//inet_addr(IP_Server.c_str());
    addr_server.sin_port = htons(set.Port);

    for (;;)
    {
        server = socket(AF_INET, SOCK_STREAM, 0);
        if (server == -1)
        {
            std::cout << "CLIENT ID: " << ID << "/tERROR INITIALIZATION CODE ERROR: " << errno << std::endl;
            return -1;
        }

        if (connect(server, (sockaddr*)&addr_server, sizeof(addr_server)) == -1)
        {
            std::cout << "CLIENT ID: " << ID << "\tERROR CONNECTION WITH SERVER CODE ERROR: " << errno << std::endl;
            close(server);
            sleep(2);
            continue;
        }
        else
        {
            std::cout << "CLIENT ID: " << ID << " CONNECTED WITH SERVER IP: " << set.IP << " PORT: " << set.Port << std::endl;
        }

        gettimeofday(&timelast, NULL);
        gettimeofday(&timenow, NULL);

        for (;;)
        {
            //gettimeofday(&timenow, NULL);
            //time = (timenow.tv_sec - timelast.tv_sec) * 1000. + (timenow.tv_usec - timelast.tv_usec) / 1000.;
            //if (time > set.frequency) std::cout << "CLIENT ID: " << ID << "\tWARNING: LIMIT_TIME_MESSENG_READING_EXCEEDED " << time <<" ms"<<std::endl;

            /*for (;;)
            {
                gettimeofday(&timenow, NULL);
                time = (timenow.tv_sec - timelast.tv_sec) * 1000. + (timenow.tv_usec - timelast.tv_usec) / 1000.;
                if (time > set.frequency - TIME_DIV) break;
                usleep(1000);
            }*/
            //gettimeofday(&timelast, NULL);

            //buf_send[0] = 3;
            //send(server, buf_send, 1, NULL);
            count_recv = 0;
            res_recv = 0;
            for (;;)
            {
                res_recv= recv(server, buf_recv + count_recv, set.size_data * s_data + 5 - count_recv, NULL);
                if (res_recv == 0) break;
                count_recv += res_recv;
                if (count_recv < 5) continue;
                num_recv = *((int*)(buf_recv + 1));
                if (count_recv < num_recv * 4 + 5) { continue; }
                else break;
            }
            if (res_recv == 0)
            {
                close(server);
                break;
            }

            gettimeofday(&timenow, NULL);
            time = (timenow.tv_sec - timelast.tv_sec) * 1000. + (timenow.tv_usec - timelast.tv_usec) / 1000.;
            if (time > set.frequency) std::cout << "CLIENT ID: " << ID << "\tWARNING: LIMIT_TIME_MESSENG_READING_EXCEEDED " << time <<" ms"<<std::endl;
            gettimeofday(&timelast, NULL);

            ibuf_recv = buf_recv + 5;
            imass_data = set.buf_data;
            
            pthread_mutex_lock(&set.mutex_data);
            for (int i = 0; i < num_recv * 4; i++)
            {
                *imass_data = *ibuf_recv;
                imass_data++;
                ibuf_recv++;
            }
            pthread_mutex_unlock(&set.mutex_data);
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