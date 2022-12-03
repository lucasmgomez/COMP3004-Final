#include "oasis.h"


// Constructor
Oasis::Oasis() {
    currSession = new Session;
	currUser = 0;
	power = OFF;
    battery = 100.00;
	connection = NO;
	toRecord = false;
	running = false;
    leftEarConnected = false;
    rightEarConnected = false;
    batteryState = CHARGED;
}

Oasis::~Oasis() {
	if (currSession != NULL) { delete currSession; }
}
// getters + setters

int Oasis::getLeftEarConnected() const {return leftEarConnected;}
int Oasis::getRightEarConnected() const {return rightEarConnected;}
Session* Oasis::getCurrSession() const {return currSession;}
bool Oasis::getRunning() const {return running;}
int Oasis::getPower() const { return power; }
float Oasis::getBattery() const { return battery; }
int Oasis::getConnection() const { return connection; }
User* Oasis::getUser() { return users+currUser; }

void Oasis::setBattery(float b) {
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
        //delete currSession;
        currSession = NULL;
	}
	running = false;
	power = OFF;	
}

void Oasis::useBattery() {
    if (power == ON)
    {
        //Will connection affect battery drain?
        if (running == true && currSession->getIntensity()>0){
            //longest session is 2700 seconds (45 mins)
            //so battery should last at least 2700 seconds while at full intensity...
            setBattery(battery - (100.00/2700.00 - 0.008 + (currSession->getIntensity()/1000.00)));
        }
        else{
            setBattery(battery - (100.00/2700.00 - 0.008));
        }
    }
    //turn off power if battery is completely drained
    handleLowBattery();
}

void Oasis::handleLowBattery(){
    if (getBattery() < LOWTHRESHOLD && getBattery() > CRITICALTHRESHOLD)
    {
        cout << "       battery(low):" << getBattery() << "%" << "(is <25%,>10%)" << endl;
        batteryState = LOW;
    }
    else if (getBattery() < CRITICALTHRESHOLD)
    {
        cout << "       battery(critical):" << getBattery() << "%" << "(is <10%)" << "stopping session until battery is charged." << endl;
        endSession(); // <-?
        batteryState = CRITICAL;
    }
    else if (getBattery() > LOWTHRESHOLD)
    {
        cout << "       battery(charged):" << getBattery() << "%" << "(is >25%)" << endl;
        batteryState = CHARGED;
    }
}

int Oasis::getBatteryState(){
    return batteryState;
}

void Oasis::runSession() {
	if (power == OFF) { return; }
	if (connection < 1) {
		cout << "No connection... please check connectivity and try again." << endl;
		return;
	}
    if (getBattery() < CRITICALTHRESHOLD){
        cout << "Battery levels critical" << getBattery() << "%, please recharge and try again." << endl;
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
        //delete currSession;
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


void Oasis::nextDuration(){
    if (power == OFF) { return; }
    currSession->nextDuration();
}
void Oasis::nextType(){
    if (power == OFF) { return; }
    currSession->nextType();
}
void Oasis::prevType(){
    if (power == OFF) { return; }
    currSession->prevType();
}
void Oasis::nextIntensity(){
    if (power == OFF){ return; } //|| running == false
    currSession->nextIntensity();
}
void Oasis::prevIntensity(){
    if (power == OFF){ return; } //|| running == false
    currSession->prevIntensity();
}


void Oasis::toggleLeftEar(){
    leftEarConnected = !leftEarConnected;
}
void Oasis::toggleRightEar(){
    rightEarConnected = !rightEarConnected;
}

void  Oasis::setLeftEar(bool state)
{
    leftEarConnected = state;
}
void  Oasis::setRightEar(bool state)
{
    rightEarConnected = state;
}

void Oasis::setCustomUserDur(int c){
    currSession->setCustomUserDur(c);
}
int Oasis::getDurationInMin(){
    return currSession->getDurationInMin();
}
void Oasis::setDuration(int d){
    currSession->setDuration(d);
}

int Oasis::getIntensity(){
    return currSession->getIntensity();
}




