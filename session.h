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
		void setDuration(int);
		void setType(int);
		void setIntensity(int);
		
		// functions
		void changeIntensity(int);	// increase intensity by the passed value 
		void print() const;
		
		
	private:
		int duration;
		int type;
		int intensity;
		
		
		
};



#endif
