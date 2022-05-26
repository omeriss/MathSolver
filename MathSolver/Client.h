#pragma once
#include <asio.hpp>
#include <asio/ts/internet.hpp>
#include <asio/ts/buffer.hpp>
#include "TCP.h"
#include "UDP.h"

class Client
{
public:
	/// <summary>
	/// constructor
	/// </summary>
	Client();

	/// <summary>
	/// connect to server
	/// </summary>
	/// <param name="ip"> the ip of the server</param>
	/// <param name="port">the port of the server</param>
	/// <param name="roomCode">the room code</param>
	/// <param name="userName">the username of the client</param>
	/// <returns>false if failed</returns>
	bool Connect(std::string& ip, int16_t port, std::string userName, std::string roomCode = "");

	/// <summary>
	/// send udp packet
	/// </summary>
	/// <param name="packet"></param>
	void SendUdp(std::shared_ptr<Packet> packet);

	/// <summary>
	/// send tcp packet
	/// </summary>
	/// <param name="packet"></param>
	void SendTcp(std::shared_ptr<Packet> packet);
	
	/// <summary>
	/// disconnect
	/// </summary>
	/// <param name="destroy"></param>
	void Disconnect(bool destroy = true);
private:
	
	/// <summary>
	/// the io service to run async functions
	/// </summary>
	asio::io_service service;

	/// <summary>
	/// the async thread
	/// </summary>
	std::thread serviceThread;

	/// <summary>
	/// tcp class
	/// </summary>
	TCP* tcp;

	/// <summary>
	/// udp class
	/// </summary>
	UDP* udp;
};

