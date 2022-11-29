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
        int getType();
        int getIntensity();
		void setDuration(int);
		void setType(int);
		void setIntensity(int);

        void setCustomUserDur(int);
		
		// functions
		void changeIntensity(int);	// increase intensity by the passed value 
		void print() const;

        void nextDuration(); //selects next duration for currSession
        void nextType(); //selects next session type for currSession
        void prevType(); //selects previous session type for currSession
        void nextIntensity(); //selects next intensity value
        void prevIntensity(); //selects previous intensity value

        int getDurationInMin(); //return duration in minutes, depending on selected "mode"


		
	private:
		int duration;
		int type;
		int intensity;

        int customUserDur; //for user specified time, in minutes
		

		
		
};



#endif
