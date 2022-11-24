#ifndef SESSION_H
#define SESSION_H

#include "defs.h"
#include <iostream>

using namespace std;

class Session {

	public:
		// cosntructor
		Session();
		
		// getters + setters
        int getDuration();
		void setDuration(int);
		void setType(int);
		void setIntensity(int);
		
		// functions
		void changeIntensity(int);	// increase intensity by the passed value 
		void print() const;

        void nextDuration(); //selects next duration for currSession
        void nextType(); //selects next session type for currSession
        void prevType(); //selects previous session type for currSession
		
	private:
		int duration;
		int type;
		int intensity;
		
		
		
};



#endif
