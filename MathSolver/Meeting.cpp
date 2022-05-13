#include "Meeting.h"

Meeting::Meeting(Screen* boardS, Screen* paricipentsS, Screen* toolsS, Screen* subScreen, Client* cl):voiceRecorder(cl)
{
	this->boardScreen = boardS;
	this->participentsScreen = paricipentsS;
	this->toolsScreen = toolsS;
	this->board = (DrawScreen*)boardScreen->GetElementByName("board");
	this->client = cl;
	board->SetClinet(cl);
	((DrawScreen*)boardS->GetElementByName("board"))->SetSubScreen(subScreen);
	//participants[0] = new Participant(0, 0, paricipentsS);
}

void Meeting::SetActive(bool state)
{
	isActive = state;
	boardScreen->SetActive(isActive);
	participentsScreen->SetActive(isActive);
	toolsScreen->SetActive(isActive);
	if (state == true) {
		if (sf::SoundBufferRecorder::getAvailableDevices().size() >= 1 && sf::Keyboard::isKeyPressed(sf::Keyboard::Tab)) {
			for (auto u : sf::SoundBufferRecorder::getAvailableDevices()) {
				std::cout << u << "\n";
			}
			int micIndex;
			std::cin >> micIndex;
			voiceRecorder.setDevice(sf::SoundBufferRecorder::getAvailableDevices()[micIndex]);
		}
		this->participentType = ParticipentType::WatchOnly;
		board->SetDrawMode(false);
		participantsCount = 0;
		((ToggleButton*)toolsScreen->GetElementByName("MuteButton"))->ChangeState(false);
		voiceRecorder.start(SampleRate);
	}
	else {
		for (auto p : participants) {
			delete p.second;
		}
		participants.clear();
		voiceRecorder.stop();
		board->Clear();
	}

}

void Meeting::SetCell(int x, int y, sf::Color color)
{
	board->SetCell(x, y, color);
}

void Meeting::SetLine(int y, array<sf::Color, BASE_DRAW_SCREEN_W>& pixels)
{
	board->SetLine(y, pixels);
}

void Meeting::AddParticipant(uint32_t uid, ParticipentType pType)
{
	participants[uid] = new Participant(uid, participantsCount++, participentsScreen, this->client);
	participants[uid]->SetParticipentType(pType);
	participants[uid]->SetButtonMode(this->participentType == ParticipentType::Host);
	if (participentType == ParticipentType::Host) {
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
	auto temp = toRemove->second;
	toRemove->second->RemoveFromScreen(participentsScreen);
	participants.erase(toRemove);
	for (auto p : participants) {
		p.second->CorrectPosition(pos);
	}
	delete temp;
	participantsCount--;
}

void Meeting::PlayAudioData(uint32_t uid, Byte* soundData, int dataSize)
{
	auto sentFrom = participants.find(uid);
	if(sentFrom != participants.end())
		sentFrom->second->LoadSoundData(soundData, dataSize);
}

void Meeting::SetColor(sf::Color color)
{
	this->board->SetColor(color);
}

void Meeting::SetParticipentType(uint32_t uid, ParticipentType participentType)
{
	if (uid == selfId) {
		this->participentType = participentType;
		board->SetDrawMode(participentType >= ParticipentType::Editor);
		for (auto pr : participants) {
			pr.second->SetButtonMode(participentType == ParticipentType::Host);
		}
		
	}
	else {
		auto pr = participants.find(uid);
		if (pr == participants.end())
			return;
		pr->second->SetParticipentType(participentType);
	}
}

void Meeting::ChangeRecorderState(bool active)
{
	if (active) {
		voiceRecorder.start(SampleRate);
	}
	else {
		voiceRecorder.stop();
	}
}

void Meeting::SetSelfId(uint32_t selfId)
{
	this->selfId = selfId;
}

void Meeting::SetRadius(float r)
{
	this->board->SetRadius(r);
}