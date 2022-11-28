#ifndef USER_H
#define USER_H

#include "session.h"

using namespace std;

class User {

	public:
		// cosntructor
		User();
		~User();
			
		// functions
		void record(Session*);		// adds sessions to recorded list
        int getnumRecord() const;	// return the number of recorded sessions
        Session* replay(int) ;	// returns the session at the specified index in the recorded list
        void print() const;
		
	private:
        Session *recorded[MAXRECORD];	// array of sessions recorded
		int numRecord;			// number of recorded sessions
		
		
		
};



#endif
