#pragma once
#include "Packet.h"
#include "UiManager.h";

/// <summary>
/// handle user disconnect
/// </summary>
/// <param name="id"> the id of the server, is not used. only to keep packerexecutor the same</param>
/// <param name="p"> the packet</param>
void HandleDisconnectFromServer(uint32_t id ,Packet* p);

/// <summary>
/// handle room code response
/// </summary>
/// <param name="id"> the id of the server, is not used. only to keep packerexecutor the same</param>
/// <param name="p"> the packet</param>
void HandlePingBack(uint32_t id, Packet* p);

/// <summary>
/// handle audio packet
/// </summary>
/// <param name="id"> the id of the server, is not used. only to keep packerexecutor the same</param>
/// <param name="p"> the packet</param>
void AudioH(uint32_t id, Packet* p);

/// <summary>
/// handle line sent from the server
/// </summary>
/// <param name="id"> the id of the server, is not used. only to keep packerexecutor the same</param>
/// <param name="p"> the packet</param>
void HandleLine(uint32_t id, Packet* p);

/// <summary>
/// handle cell change
/// </summary>
/// <param name="id"> the id of the server, is not used. only to keep packerexecutor the same</param>
/// <param name="p"> the packet</param>
void HandleCell(uint32_t id, Packet* p);

/// <summary>
/// handle new user connected
/// </summary>
/// <param name="id"> the id of the server, is not used. only to keep packerexecutor the same</param>
/// <param name="p"> the packet</param>
void HandleUserConnected(uint32_t id, Packet* p);

/// <summary>
/// handle user disconnected
/// </summary>
/// <param name="id"> the id of the server, is not used. only to keep packerexecutor the same</param>
/// <param name="p"> the packet</param>
void HandleUserDisconnected(uint32_t id, Packet* p);

/// <summary>
/// handle participent type changed 
/// </summary>
/// <param name="id"> the id of the server, is not used. only to keep packerexecutor the same</param>
/// <param name="p"> the packet</param>
void HandleChangeParticipentType(uint32_t id, Packet* p);

#define CreatePacketVector { HandleDisconnectFromServer, HandlePingBack, AudioH, HandleLine, HandleCell, HandleUserConnected, HandleUserDisconnected, HandleChangeParticipentType}