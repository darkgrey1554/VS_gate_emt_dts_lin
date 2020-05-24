#ifndef TCP_UNITL_H
#define TCP_UNITL_H


#include <iostream>
#include <thread>
#include <pthread.h>
#include <list>

#define TIME_DIV 20



enum class TypeData
{
    Not,
    OutPut,
    InPut
};

enum class TypeSignal
{
    Nothing,
    Analog,
    Discrete
};

enum class TypeUnitGate
{
    EMPTY,
    SERVER,
    CLIENT
};

std::ostream& operator<<(std::ostream& out, TypeSignal& m);

std::ostream& operator<<(std::ostream& out, TypeUnitGate& m);



struct ConfigSharedMemory
{
    std::string name_memory;
    int size;
    TypeData type_data;
    TypeSignal type_signal;
    void clear()
    {
        name_memory.clear();
        size = 0;
        type_data = TypeData::Not;
        type_signal = TypeSignal::Nothing;
    }
};

struct ConfigUnitGate
{
    std::string IP;
    int Port = 0;
    int frequency = 0;
    int offset = 0;
    int size_data = 0;
    TypeSignal type_signal = TypeSignal::Nothing;
    TypeUnitGate type_unit = TypeUnitGate::EMPTY;
    char* buf_data;
    pthread_mutex_t mutex_data;

    void clear()
    {
        IP.clear();
        Port = 0;
        frequency = 0;
        offset = 0;
        size_data = 0;
        type_signal = TypeSignal::Nothing;
        type_unit = TypeUnitGate::EMPTY;
        buf_data = NULL;
    }
};

int read_config_file(const char* Namefile, std::list<ConfigSharedMemory>* listsharmem, std::list<ConfigUnitGate>* listunitgate = NULL);

int conver_IPstr_int(const char* str);

class tcp_unit
{
public:
	
    ConfigUnitGate set;

	void virtual restart_thread() {}
	void virtual close_tcp_unit() {}

	static tcp_unit* create_tcp_unit(ConfigUnitGate config, int id);

};

class tcp_server : public tcp_unit
{
    
    std::thread thread_unit;
    int ID;
    int thread_tcp_server();
    int s_data;
public:

	tcp_server(ConfigUnitGate config, int id);
	void restart_thread() override;
	void close_tcp_unit() override;
};

class tcp_client : public tcp_unit
{
    
    std::thread thread_unit;
    int ID;
    int s_data;
	int thread_tcp_client();

public:

	tcp_client(ConfigUnitGate config, int id);
	void restart_thread() override;
	void close_tcp_unit() override;
};


#endif // !TCP_UNITL_H

