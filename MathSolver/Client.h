#pragma once
#include <asio.hpp>
#include <asio/ts/internet.hpp>
#include <asio/ts/buffer.hpp>
#include "TCP.h"
#include "UDP.h"

class Client
{
public:
	Client();
	bool Connect(std::string& ip, int16_t port);
	void SendUdp(Packet& packet);
	void SendTcp(Packet& packet);
	void Disconnect();
private:
	asio::io_service service;
	std::thread serviceThread;
	TCP* tcp;
	UDP* udp;
	asio::ip::udp::socket* UdpScocket;
};

