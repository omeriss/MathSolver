#include "HandlePacketFunctions.h"

void HandleDisconnectFromServer(uint32_t id, Packet* p)
{
	std::cout << "disconnected" << std::endl;
	UiManager::GetInstance()->cl.Disconnect();
	UiManager::GetInstance()->GetMeeting()->SetActive(false);
	UiManager::GetInstance()->GetScreens()["00main"]->SetActive(true);
}

void HandlePingBack(uint32_t id, Packet* p)
{
	uint32_t myId;
	std::string roomCode;
	(*p) >> myId;
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

void AudioH(uint32_t id, Packet* p)
{
	Byte* soundData = p->GetData();
	uint32_t uid;
	std::memcpy(&uid, soundData + p->GetHeader().size - sizeof(uint32_t), sizeof(uint32_t));
	UiManager::GetInstance()->GetMeeting()->PlayAudioData(uid, p->GetData(), p->GetHeader().size - sizeof(uint32_t));
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
	ParticipentType temp;
	(*p) >> temp;
	UiManager::GetInstance()->GetMeeting()->AddParticipant(newid, temp);
	std::cout << "New user connected: " << newid <<std::endl;
}

void HandleUserDisconnected(uint32_t id, Packet* p)
{
	uint32_t removedId;
	(*p) >> removedId;
	UiManager::GetInstance()->GetMeeting()->RemoveParticipant(removedId);
	std::cout << "user disconnected: " << removedId << std::endl;
}

void HandleChangeParticipentType(uint32_t id, Packet* p)
{
	uint32_t pid;
	(*p) >> pid;
	ParticipentType temp;
	(*p) >> temp;
	UiManager::GetInstance()->GetMeeting()->SetParticipentType(pid, temp);
}
