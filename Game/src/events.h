#ifndef EVENTS_H_
#define EVENTS_H_

struct StartGame {
	std::string text;
	StartGame(std::string text) : text(text) {};
};
struct StartLevel2 {};
struct OptionMenu {};
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
struct LevelEvent {
	int level;
	LevelEvent(int level) : level(level){};
};
#endif // EVENTS_H_