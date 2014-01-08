#ifndef LEAPMIDI_H
#define LEAPMIDI_H

#include <map>
#include <alsa/asoundlib.h>
#include "Leap.h"

class SoundClient {
private:
	snd_seq_t* seq;
	int port;
	int basenote;
	const static int channel = 0;
	typedef enum { PLAY, RELEASE } NoteAction;

	void openSequencer();
	void initTempo();
	void noteAction(int note, NoteAction action);

public:
	SoundClient();
	void play(int note);
	void release(int note);
	void close();
};

class LeapListener : public Leap::Listener {
private:
	const int basenote;
	SoundClient soundClient;
	std::map<int, bool> playingNotes;
	static const int notes[][3];

	void doPlay(int note);
	void doRelease(int note);

public:
	virtual void onInit(const Leap::Controller&);
	virtual void onConnect(const Leap::Controller&);
	virtual void onDisconnect(const Leap::Controller&);
	virtual void onExit(const Leap::Controller&);
	virtual void onFrame(const Leap::Controller&);
	virtual void onFocusGained(const Leap::Controller&);
	virtual void onFocusLost(const Leap::Controller&);

	LeapListener(int basenote, const SoundClient& soundClient);
};

#endif  /*LEAPMIDI_H*/

