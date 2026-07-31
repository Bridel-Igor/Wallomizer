#pragma once

class App;
class Wallpaper;

class Delay
{
public:
	enum class SlideshowStatus : unsigned char
	{
		paused,
		playing,
		stopped
	};

public:
	Delay(App& app);
	~Delay();

	void saveSession(Wallpaper* pCurrent = nullptr);
	void loadSession(Wallpaper*& pCurrent);
	void delay();
	unsigned long getRemainingDelay();
	void abortDelay();
	void replayDelay();
	void setSlideshowStatus(const SlideshowStatus status);

	bool exiting;
	SlideshowStatus slideshowStatus;

private:
	App& m_app;

	bool bAbortDelay = false;
	bool bReplayDelay = false;
	unsigned long uDelayed = 0;
};
