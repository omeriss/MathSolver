#include "HandlePacketFunctions.h"

void HandleDisconnectFromServer(uint32_t id, Packet* p)
{
	std::cout << "disconnected" << std::endl;
	UiManager::GetInstance()->cl.Disconnect();
}

void HandlePingBack(uint32_t id, Packet* p)
{
	std::string roomCode;
	(*p) >> roomCode;
	if (roomCode == "") {
		std::cout << "Can't join room" << std::endl;
		return;
	}
	std::cout << "Joined Room: " << roomCode << std::endl;
	UiManager::GetInstance()->GetMeeting()->SetActive(true);
	UiManager::GetInstance()->GetScreens()["01open"]->SetActive(false);
	UiManager::GetInstance()->GetScreens()["01join"]->SetActive(false);
}

#include <SFML/Audio.hpp>

void AudioH(uint32_t id, Packet* p)
{

	static sf::SoundBuffer b;
	static sf::Int16 bu[10000];
	static int from = 0;
	static sf::Sound s;
	//std::cout << p->GetHeader().size << std::endl;
	if (from < 3000) {
		std::memcpy(bu+from, (const sf::Int16*)p->GetData(), p->GetHeader().size);
		from += p->GetHeader().size / sizeof(sf::Int16);
		std::cout << 1;
	}
	else {
		std::memcpy(bu + from, (const sf::Int16*)p->GetData(), p->GetHeader().size);
		from += p->GetHeader().size / sizeof(sf::Int16);
		b.loadFromSamples(bu, from, 1, 10000);
		from = 0;
		s.setBuffer(b);
		s.play();
		std::cout << 2;
	}
}

void HandleLine(uint32_t id, Packet* p)
{
	int y;
	array<sf::Color, BASE_DRAW_SCREEN_W> line;
	(*p) >> y;
	p->ReadArr(line.data(), BASE_DRAW_SCREEN_W);
	UiManager::GetInstance()->GetMeeting()->SetLine(y, line);
}

void HandleCell(uint32_t id, Packet* p)
{
	int x, y;
	sf::Color color;
	(*p) >> x >> y >> color;
	UiManager::GetInstance()->GetMeeting()->SetCell(x, y, color);
}

void HandleUserConnected(uint32_t id, Packet* p)
{
	uint32_t newid;
	(*p) >> newid;
	UiManager::GetInstance()->GetMeeting()->AddParticipant(newid);
	std::cout << "New user connected: " << newid <<std::endl;
}

void HandleUserDisconnected(uint32_t id, Packet* p)
{
	uint32_t removedId;
	(*p) >> removedId;
	UiManager::GetInstance()->GetMeeting()->RemoveParticipant(removedId);
	std::cout << "user disconnected: " << removedId << std::endl;
}
