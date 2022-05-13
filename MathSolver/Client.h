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
	bool Connect(std::string& ip, int16_t port, std::string roomCode = "");
	void SendUdp(std::shared_ptr<Packet> packet);
	void SendTcp(std::shared_ptr<Packet> packet);
	void Disconnect(bool destroy = true);
private:
	asio::io_service service;
	std::thread serviceThread;
	TCP* tcp;
	UDP* udp;
};

