#ifndef TCP_UNITL_H
#define TCP_UNITL_H


#include <iostream>
#include <thread>


class tcp_unit
{
public:
	std::string type_unit;
	std::thread thread_unit;
	std::string type_data;
	int size_data;
	int frequency;
	int ID;
	int offset;
	char* mass_data;

	void virtual restart_thread() {}
	void virtual close_tcp_unit() {}

	static tcp_unit* create_tcp_unit(std::string type_unit, int id, std::string ip, int port, std::string t_data, int s_data, char* mass_data, int bais);

};

class tcp_server : public tcp_unit
{
	std::string IP_Server;
	int PORT;

	int thread_tcp_server();

public:

	tcp_server(int id, std::string ip, int port, std::string t_data, int s_data, char* mass_data, int bais);
	void restart_thread() override;
	void close_tcp_unit() override;
};

class tcp_client : public tcp_unit
{
	std::string IP_Server;
	int PORT;

	int thread_tcp_client();

public:

	tcp_client(int id, std::string ip, int port, std::string t_data, int s_data, char* mass_data, int bais);
	void restart_thread() override;
	void close_tcp_unit() override;
};


#endif // !TCP_UNITL_H

