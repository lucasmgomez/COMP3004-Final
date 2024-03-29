#include "session.h"


// Constructor
Session::Session() {
	duration = 1;
	type = 1;
    intensity = 1;
}

Session::Session(Session* ses) {
    duration = ses->getDuration();
    type = ses->getType();
    intensity = ses->getIntensity();
}




// Getters + Setters

int Session::getDuration(){return duration;}

int Session::getType(){return type;}

int Session::getIntensity(){return intensity;}

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
    if (i >= 1) {
		intensity = i;
	}
	else { cout << "Invalid intensity" << endl; }
}



// functions

void Session::changeIntensity(int incr) {
	intensity = intensity + incr;
    if (intensity < 1) { intensity = 1; }
    else if (intensity > 8) { intensity = 8;} //is the max intensity 8?
    cout << "Intensity:" << intensity << "/8" << endl;
}

void Session::print() const {
	cout << "Duration: " << duration << ", Type: " << type << ", Intensity: " << intensity << endl;
}

void Session::nextDuration(){
    if (duration+1 > FORTYFIVE)
    {
        setDuration((duration+1) % FORTYFIVE);
    }
    else
    {
        setDuration(duration+1);
    }
}
void Session::nextType(){

    if (type+1 > THETA)
    {
        setType((type+1) %THETA);
    }
    else
    {
        setType(type+1);
    }
}
void Session::prevType(){
    if (type-1 < 1)
    {
        setType(THETA);
    }
    else
    {
        setType(type-1);
    }
}

void Session::nextIntensity(){
    changeIntensity(1);
}
void Session::prevIntensity(){
    changeIntensity(-1);
}

int Session::getDurationInMin(){
    if (duration == TWENTY){
        return 20;
    }
    else if (duration == FORTYFIVE){
        return 45;
    }
    else if (duration == USERDESIGNATED){
        return customUserDur;
    }
    return -1;
}

void Session::setCustomUserDur(int c){
    if( c < 0){
        cout << "duration cannot be negative" << endl;
        customUserDur = 0;
    }
    else if(c > 45){
        cout << "duration cannot be greater than 45 minutes" << endl;
        customUserDur = 45;
    }
    else{
        customUserDur = c;
    }
}

