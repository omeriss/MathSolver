#include "Meeting.h"

Meeting::Meeting(Screen* boardS, Screen* paricipentsS, Screen* toolsS, Client* cl):voiceRecorder(cl)
{
	this->boardScreen = boardS;
	this->participentsScreen = paricipentsS;
	this->toolsScreen = toolsS;
	this->board = (DrawScreen*)boardScreen->GetElementByName("board");
	this->client = cl;
	board->SetClinet(cl);
	this->type = Host;
	participantsCount = 1;
	new Participant(0, 0, participentsScreen);
}

void Meeting::SetActive(bool state)
{
	isActive = state;
	boardScreen->SetActive(isActive);
	participentsScreen->SetActive(isActive);
	toolsScreen->SetActive(isActive);
	if (state == true)
		voiceRecorder.start(SampleRate);
	else
		voiceRecorder.stop();

}

void Meeting::SetCell(int x, int y, sf::Color color)
{
	board->SetCell(x, y, color);
}

void Meeting::SetLine(int y, array<sf::Color, BASE_DRAW_SCREEN_W>& pixels)
{
	board->SetLine(y, pixels);
}

void Meeting::AddParticipant(uint32_t uid)
{
	participants[uid] = new Participant(uid, participantsCount++, participentsScreen);
	if (type == Host) {
		vector<array<sf::Color, BASE_DRAW_SCREEN_W>>& boardPixels = board->GetBoard();
		int i = 0;
		for (auto& line : boardPixels) {
			std::shared_ptr<Packet> packet = std::make_shared<Packet>();
			packet->GetHeader().packetType = SendLine;
			(*packet) << uid;
			(*packet) << i;
			packet->WriteArr(line.data(), line.size());
			client->SendTcp(std::move(packet));
			i++;
		}
	}
}

void Meeting::RemoveParticipant(uint32_t uid)
{
	auto toRemove = participants.find(uid);
	if (toRemove == participants.end())
		return;
	int pos = toRemove->second->GetPosition();
	toRemove->second->RemoveFromScreen(participentsScreen);
	participants.erase(toRemove);
	for (auto p : participants) {
		p.second->CorrectPosition(pos);
	}
}
