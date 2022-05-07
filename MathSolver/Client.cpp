#include "Client.h"

Client::Client()
{
	service.stop();
}

bool Client::Connect(std::string& ip, int16_t port, std::string roomCode)
{
	try {
		std::cout << service.stopped() << std::endl;
		if (!service.stopped())
			return false;
		service.~io_context();
		new(&service) asio::io_service;

		tcp = new TCP(service);
		asio::ip::basic_resolver<asio::ip::tcp> rT(service);
		auto endpoint = rT.resolve(ip, std::to_string(port));

		asio::ip::basic_resolver<asio::ip::udp> rU(service);
		auto endpointUdp = rU.resolve(ip, std::to_string(port));

		asio::async_connect(tcp->GetSocket() , endpoint,
			[roomCode ,endpointUdp, this](std::error_code ec, asio::ip::tcp::endpoint endpoint)
			{
				if (!ec)
				{
					std::cout << tcp->GetSocket().local_endpoint().port() << std::endl;
					UDP::socket = new asio::ip::udp::socket(service, asio::ip::udp::endpoint(asio::ip::udp::v4(), tcp->GetSocket().local_endpoint().port()));
					udp = new UDP(service, endpointUdp->endpoint());

					std::shared_ptr<Packet> pingPacket = std::make_shared<Packet>();
					(*pingPacket) << roomCode;
					pingPacket->GetHeader().packetType = RoomRequest;
					tcp->Send(std::move(pingPacket));

					tcp->ReadPacket();
					udp->ReadPacket();
				}
				else {
					std::cout << "cant connect" << std::endl;
					tcp->Disconnect();
					delete tcp;
				}
			});
		std::cout << "connecting to "<<ip<<":"<<port << std::endl;
		this->serviceThread = std::thread([this]() { service.run(); });
		serviceThread.detach();
	}
	catch (std::exception& exp){
		std::cout << "Eror:" << exp.what() << std::endl;
		service.stop();
		if (tcp)
			delete tcp;
		tcp = nullptr;
		return false;
	}
}

void Client::SendUdp(std::shared_ptr<Packet> packet)
{
	if (udp && tcp && tcp->GetSocket().is_open())
		udp->Send(std::move(packet));
}

void Client::SendTcp(std::shared_ptr<Packet> packet)
{
	if (tcp && tcp->GetSocket().is_open())
		tcp->Send(std::move(packet));
}


void Client::Disconnect()
{
	tcp->Disconnect();
	service.stop();
	delete tcp;
	delete udp;
	udp = nullptr;
	tcp = nullptr;
}
