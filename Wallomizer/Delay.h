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

	void saveSession(Wallpaper* pCurrent = nullptr);
	void loadSession(Wallpaper*& pCurrent);
	void delay();
	unsigned long getRemainingDelay() const;
	void abortDelay();
	void replayDelay();
	void setSlideshowStatus(const SlideshowStatus status);

	bool exiting = false;
	SlideshowStatus slideshowStatus = Delay::SlideshowStatus::playing;

private:
	App& m_app;

	bool bAbortDelay = false;
	bool bReplayDelay = false;
	unsigned long uDelayed = 0;
};
