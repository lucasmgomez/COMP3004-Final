#ifndef OASIS_H
#define OASIS_H

#include "user.h"

using namespace std;

class Oasis {

	public:
		// cosntructor
		Oasis();
		~Oasis();
		
		// getters + setters
		int getPower() const;		// return power state of the oasis pro
		int getBattery() const;		// return the current battery level
		int getConnection() const;	// return the result of the cinnection test
		bool getRunning() const;	// return running state of the oasis pro
		void setBattery(int);		// manually set the battery level
		void setConnection(int);	// manually set the connectivity
		void setUser(int);		// change the current user
		
		// functions
		void turnOn();			// turns on the oasis pro
		void turnOff();			// turns off the oasis pro
		void useBattery();		// decreases battery level
		void runSession();		// start currSession
		void endSession();		// end currSession and turn off oasis pro
		void replay(int);		// replay the selected session from the currUser's recorded sessions
		void record();			// set record to true so that the currSession will be recorded
		void selectDuration(int);	// set duration for currSession
		void selectType(int);		// set session type for currSession
		void changeIntensity(int);	// change intensity by the passed value for currSession
		void print() const;
		
		
	private:
		Session* currSession;		// pointer to the currSession to be ran
		User users[MAXUSERS];		// array of all users
		int currUser;			// index of the current user
		int power;			// state of the oasis pro (on or off)
		int battery;			// battery level
		int connection;			// connectivity (no, okay, or excellent)
		bool toRecord;			// indicates if the current session is to be recorded
		bool running;			// indicates if a session is currently running
		
		
		
};



#endif
