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
		
		// functions
		void changeIntensity(int);	// increase intensity by the passed value 
		void print() const;

        void nextDuration(); //selects next duration for currSession
        void nextType(); //selects next session type for currSession
        void prevType(); //selects previous session type for currSession
        void nextIntensity(); //selects next intensity value
        void prevIntensity(); //selects previous intensity value
		
	private:
		int duration;
		int type;
		int intensity;
		
		
		
};



#endif
