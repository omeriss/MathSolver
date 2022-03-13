#include "HandlePacketFunctions.h"

void HandleDisconnectFromServer(uint32_t id, Packet* p)
{
	std::cout << "disconnected" << std::endl;
	UiManager::instance->cl.Disconnect();
}

void HandlePingBack(uint32_t id, Packet* p)
{
	std::cout << "got pinged"<<std::endl;
}

#include <SFML/Audio.hpp>

void AudioH(uint32_t id, Packet* p)
{
	static sf::SoundBuffer b;
	static sf::Sound s;
	std::cout << p->GetHeader().size << std::endl;
	b.loadFromSamples((const sf::Int16*)p->GetData(), p->GetHeader().size/sizeof(sf::Int16), 1, 10000);
	s.setBuffer(b);
	s.play();
}
