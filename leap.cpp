#include <iostream>
#include <map>
#include "leapmidi.h"

using namespace Leap;
using namespace std;

const int LeapListener::notes[][3] = {
	{ 0, 4, 7 },
	{ 2, 5, 9 },
	{ 4, 7, 11 },
	{ 0, 5, 9 },
	{ 2, 7, 11 },
	{ 0, 4, 9 },
	{ 5, 7, 11 }
};

LeapListener::LeapListener(int bn, const SoundClient& sc) : basenote(bn), soundClient(sc) {}

void LeapListener::onInit(const Controller& controller) { }

void LeapListener::onConnect(const Controller& controller) {
	controller.enableGesture(Gesture::TYPE_CIRCLE);
	controller.enableGesture(Gesture::TYPE_KEY_TAP);
	controller.enableGesture(Gesture::TYPE_SCREEN_TAP);
	controller.enableGesture(Gesture::TYPE_SWIPE);
}

void LeapListener::onDisconnect(const Controller& controller) { }

void LeapListener::onExit(const Controller& controller) { }

void LeapListener::doPlay(int offset) {
	map<int, bool>::iterator it = playingNotes.find(offset);
	if (it == playingNotes.end()) {
		cout << "Playing note " << offset << endl;
		soundClient.play(basenote + offset);
		playingNotes.insert(pair<int, bool>(offset, true));
	}
}

void LeapListener::doRelease(int offset) {
	map<int, bool>::iterator it = playingNotes.find(offset);
	if (it != playingNotes.end()) {
		cout << "Releasing note " << offset << endl;
		soundClient.release(basenote + offset);
		playingNotes.erase(it);
	}
}

void LeapListener::onFrame(const Controller& controller) {
	// Get the most recent frame and report some basic information
	const Frame frame = controller.frame();
	// cout << "Frame id: " << frame.id()
	// 	<< ", timestamp: " << frame.timestamp()
	// 	<< ", hands: " << frame.hands().count()
	// 	<< ", fingers: " << frame.fingers().count()
	// 	<< ", tools: " << frame.tools().count()
	// 	<< ", gestures: " << frame.gestures().count() << endl;

	if (!frame.hands().empty()) {
		// Get the first hand
		int nFingers;

		if (frame.hands().count() == 1) {
			nFingers = frame.hands()[0].fingers().count();
		} else {
			nFingers = frame.hands()[0].fingers().count() + frame.hands()[1].fingers().count();
		}

		if (nFingers > 0 && nFingers < 8) {
			const int* sequence = notes[nFingers-1];
			for (int i=0; i < 12; i++) {
				bool played = false;

				for (int j=0; j < 3; j++) {
					if (sequence[j] == i) {
						doPlay(i);
						played = true;
						break;
					}
				}

				if (!played) {
					doRelease(i);
				}
			}

		// 	// Calculate the hand's average finger tip position
		// 	Vector avgPos;
		// 	for (int i = 0; i < fingers.count(); ++i) {
		// 		avgPos += fingers[i].tipPosition();
		// 	}
		// 	avgPos /= (float)fingers.count();
		// 	cout << "Hand has " << fingers.count()
		// 		<< " fingers, average finger tip position" << avgPos << endl;
		}

		// // Get the hand's sphere radius and palm position
		// cout << "Hand sphere radius: " << hand.sphereRadius()
		// 	<< " mm, palm position: " << hand.palmPosition() << endl;

		// // Get the hand's normal vector and direction
		// const Vector normal = hand.palmNormal();
		// const Vector direction = hand.direction();

		// // Calculate the hand's pitch, roll, and yaw angles
		// cout << "Hand pitch: " << direction.pitch() * RAD_TO_DEG << " degrees, "
		// 	<< "roll: " << normal.roll() * RAD_TO_DEG << " degrees, "
		// 	<< "yaw: " << direction.yaw() * RAD_TO_DEG << " degrees" << endl;
	} else {
		map<int, bool>::iterator it;
		for (it = playingNotes.begin(); it != playingNotes.end(); ++it) {
			int note = it->first;
			doRelease(note);
		}
	}

	// // Get gestures
	// const GestureList gestures = frame.gestures();
	// for (int g = 0; g < gestures.count(); ++g) {
	// 	Gesture gesture = gestures[g];

	// 	switch (gesture.type()) {
	// 		case Gesture::TYPE_CIRCLE:
	// 			{
	// 				CircleGesture circle = gesture;
	// 				string clockwiseness;

	// 				if (circle.pointable().direction().angleTo(circle.normal()) <= PI/4) {
	// 					clockwiseness = "clockwise";
	// 				} else {
	// 					clockwiseness = "counterclockwise";
	// 				}

	// 				// Calculate angle swept since last frame
	// 				float sweptAngle = 0;
	// 				if (circle.state() != Gesture::STATE_START) {
	// 					CircleGesture previousUpdate = CircleGesture(controller.frame(1).gesture(circle.id()));
	// 					sweptAngle = (circle.progress() - previousUpdate.progress()) * 2 * PI;
	// 				}
	// 				cout << "Circle id: " << gesture.id()
	// 					<< ", state: " << gesture.state()
	// 					<< ", progress: " << circle.progress()
	// 					<< ", radius: " << circle.radius()
	// 					<< ", angle " << sweptAngle * RAD_TO_DEG
	// 					<<  ", " << clockwiseness << endl;
	// 				break;
	// 			}
	// 		case Gesture::TYPE_SWIPE:
	// 			{
	// 				SwipeGesture swipe = gesture;
	// 				cout << "Swipe id: " << gesture.id()
	// 					<< ", state: " << gesture.state()
	// 					<< ", direction: " << swipe.direction()
	// 					<< ", speed: " << swipe.speed() << endl;
	// 				break;
	// 			}
	// 		case Gesture::TYPE_KEY_TAP:
	// 			{
	// 				KeyTapGesture tap = gesture;
	// 				cout << "Key Tap id: " << gesture.id()
	// 					<< ", state: " << gesture.state()
	// 					<< ", position: " << tap.position()
	// 					<< ", direction: " << tap.direction()<< endl;
	// 				break;
	// 			}
	// 		case Gesture::TYPE_SCREEN_TAP:
	// 			{
	// 				ScreenTapGesture screentap = gesture;
	// 				cout << "Screen Tap id: " << gesture.id()
	// 					<< ", state: " << gesture.state()
	// 					<< ", position: " << screentap.position()
	// 					<< ", direction: " << screentap.direction()<< endl;
	// 				break;
	// 			}
	// 		default:
	// 			cout << "Unknown gesture type." << endl;
	// 			break;
	// 	}
	// }

	// if (!frame.hands().empty() || !gestures.empty()) {
	// 	cout << endl;
	// }
}

void LeapListener::onFocusGained(const Controller& controller) {
  cout << "Focus Gained" << endl;
}

void LeapListener::onFocusLost(const Controller& controller) {
  cout << "Focus Lost" << endl;
}

