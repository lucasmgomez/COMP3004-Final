#include "oasis.h"


// Constructor
Oasis::Oasis() {
	currSession = NULL;
	currUser = 0;
	power = OFF;
	battery = 100;
	connection = NO;
	toRecord = false;
	running = false;
}

Oasis::~Oasis() {
	if (currSession != NULL) { delete currSession; }
}



// getters + setters

int Oasis::getPower() const { return power; }
int Oasis::getBattery() const { return battery; }
int Oasis::getConnection() const { return connection; }

void Oasis::setBattery(int b) { 
	if (b > 100) { b = 100; }
	else if (b < 0) { b = 0; }
	battery = b; 
}

void Oasis::setConnection(int c) {
	if (c >= 0 && c < 3) { connection = c; }
	else { cout << "invalid connectivity level" << endl; }
}

void Oasis::setUser(int u) {
	if (u >= 0 && u < 8) { currUser = u; }
	else { cout << "invalid user" << endl; }
}



// functions

void Oasis::turnOn() {
	cout << "Turning on Oasis pro" << endl;
	power = ON;
	currSession = new Session();
}

void Oasis::turnOff() {
	cout << "Turning off Oasis pro" << endl;
	if (currSession != NULL) {
		delete currSession;
		currSession = NULL;
	}
	running = false;
	power = OFF;	
}

void Oasis::useBattery() {
	if (power == ON) { --battery; }
}

void Oasis::runSession() {
	if (power == OFF) { return; }
	if (connection < 1) {
		cout << "No connection... please check connectivity and try again." << endl;
		return;
	}
	cout << "Running session..." << endl;
	currSession->print();
	running = true;
}

void Oasis::endSession() {
	if (power == OFF) { return; }
	cout << "... session complete." << endl;
	if (toRecord) {
		users[currUser].record(currSession);
		currSession = NULL;
		toRecord = false;
	}
	else {
		delete currSession;
		currSession = NULL;
	}
	turnOff();
}

void Oasis::replay(int session) {
	if (power == OFF) { return; }
	currSession = users[currUser].replay(session);
	runSession();
}

void Oasis::record() {
	if (power == OFF) { return; }
	toRecord = true;
}

void Oasis::selectDuration(int duration) {
	if (power == OFF) { return; }
	currSession->setDuration(duration);
}

void Oasis::selectType(int type) {
	currSession->setType(type);
}

void Oasis::changeIntensity(int incr) {
	if (power == OFF) { return; }
	currSession->changeIntensity(incr);
}

void Oasis::print() const {
	if (power == OFF) {
		cout << "Oasis is off" << endl;
		return;
	}
	
	cout << "Battery level: " << battery << endl;
	
	cout << "Connectivity: " << connection << endl;
	
	cout << "Current user: ";
	users[currUser].print();
	
	if (currSession != NULL) {
		cout << "Current session: ";
		currSession->print();
	}
	else { cout << "Currently no session" << endl; }
}






