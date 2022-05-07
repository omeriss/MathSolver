#pragma once
#include "Packet.h"
#include "UiManager.h";

void HandleDisconnectFromServer(uint32_t id ,Packet* p);
void HandlePingBack(uint32_t id, Packet* p);
void AudioH(uint32_t id, Packet* p);
void HandleLine(uint32_t id, Packet* p);
void HandleCell(uint32_t id, Packet* p);
void HandleUserConnected(uint32_t id, Packet* p);
void HandleUserDisconnected(uint32_t id, Packet* p);
void HandleChangeParticipentType(uint32_t id, Packet* p);

#define CreatePacketVector { HandleDisconnectFromServer, HandlePingBack, AudioH, HandleLine, HandleCell, HandleUserConnected, HandleUserDisconnected, HandleChangeParticipentType}