#include "session.h"


// Constructor
Session::Session() {
	duration = 1;
	type = 1;
	intensity = 0;
}



// Getters + Setters

void Session::setDuration(int d) {
	switch (d) {
		case TWENTY:
			duration = d;
			cout << "Duration set to 20 minutes" << endl;
			break;
		case FORTYFIVE:
			duration = d;
			cout << "Duration set to 45 minutes" << endl;
			break;
		case USERDESIGNATED:
			duration = d;
			cout << "Duration set to user designated" << endl;
			break;
		default:
			cout << "Invalid duration" << endl;
	}
}

void Session::setType(int t) {
	if (t > 0 && t <= 8) {
		type = t;
		cout << "Type set to " << type << endl;
	}
	else { cout << "Invalid type" << endl; }
}

void Session::setIntensity(int i) {
	if (i >= 0) {
		intensity = i;
	}
	else { cout << "Invalid intensity" << endl; }
}



// functions

void Session::changeIntensity(int incr) {
	intensity = intensity + incr;
	if (intensity < 0) { intensity = 0; }
}

void Session::print() const {
	cout << "Duration: " << duration << ", Type: " << type << ", Intensity: " << intensity << endl;
}
