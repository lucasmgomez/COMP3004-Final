#include "user.h"


// Constructor
User::User() {
	numRecord = 0;

}

User::~User() {
	for (int i = 0; i < numRecord; i++) {
		delete recorded[i];
	}
}



// functions

int User::getnumRecord() const { return numRecord; }


void User::record(Session* s) {
	if (numRecord == MAXRECORD) { 
		cout << "Can't record session" << endl;
		return;
	}
	cout << "Recording current session" << endl;
    recorded[numRecord] = new Session(*s);
	numRecord++;
}

Session* User::replay(int s)  {
    if (s >= 0 && s < numRecord) { return recorded[s]; }
	else { 
		cout << "Invalid recording" << endl; 
		return NULL;
	}
}

void User::print() const {
	cout << "number of recordings: " << numRecord << endl;
}
