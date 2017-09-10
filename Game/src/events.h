#ifndef EVENTS_H_
#define EVENTS_H_

struct StartGame {
	std::string text;
	StartGame(std::string text) : text(text) {};
};
struct StartLevel1 {};
struct StartLevel2 {};
struct StartLevel3 {};
struct OptionMenu {};
struct EndScreenEvent{};
struct BackToMainMenu {};
struct CharSelect {};
struct Death {};
struct PauseMenuEvent {};
struct BackToGame {};
struct ResumeGame {};
struct MuteMusic {};
struct MuteFx {};
struct PauseGameEvent {};
struct UnpauseGameEvent {};
struct PlayText{
	std::string text;
	PlayText(std::string text) : text(text){};
};
struct MasiatrixNextPhase {
	bool three_d;
	int phase;
	MasiatrixNextPhase(int phase, bool three_d) : phase(phase), three_d(three_d) {};
};
struct LevelEvent {
	int level;
	LevelEvent(int level) : level(level){};
};
struct SetThreeD {
	bool three_d;
	SetThreeD(bool three_d) : three_d(three_d){};
};
#endif // EVENTS_H_