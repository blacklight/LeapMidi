#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "leapmidi.h"

using namespace std;

void SoundClient::openSequencer() {
	if (snd_seq_open(&seq, "default", SND_SEQ_OPEN_DUPLEX, 0) < 0) {
		throw "Error opening ALSA sequencer";
	}

	snd_seq_set_client_name(seq, "Leap Motion MIDI");

	if ((port = snd_seq_create_simple_port(
			seq,
			"Port 1",
			SND_SEQ_PORT_CAP_READ | SND_SEQ_PORT_CAP_SUBS_READ,
			SND_SEQ_PORT_TYPE_MIDI_GENERIC | SND_SEQ_PORT_TYPE_APPLICATION)) < 0) {
		throw "Could not open MIDI port";
	}
}

SoundClient::SoundClient() {
	openSequencer();
}

void SoundClient::noteAction(int note, NoteAction action) {
	snd_seq_event_t event;

	snd_seq_ev_clear(&event);
	snd_seq_ev_set_source(&event, port);
	snd_seq_ev_set_subs(&event);
	snd_seq_ev_set_direct(&event);

	if (action == PLAY) {
		snd_seq_ev_set_noteon(&event, channel, note, 127);
	} else {
		snd_seq_ev_set_noteoff(&event, channel, note, 0);
	}

	snd_seq_event_output(seq, &event);
	snd_seq_drain_output(seq);
}

void SoundClient::play(int note) {
	noteAction(note, PLAY);
}

void SoundClient::release(int note) {
	noteAction(note, RELEASE);
}

void SoundClient::close() {
	snd_seq_close(seq);
}

