#pragma once
#include "Packet.h"
#include "UiManager.h";

void HandleDisconnectFromServer(uint32_t id ,Packet* p);
void HandlePingBack(uint32_t id, Packet* p);
void AudioH(uint32_t id, Packet* p);

#define CreatePacketVector { HandleDisconnectFromServer, HandlePingBack, AudioH}