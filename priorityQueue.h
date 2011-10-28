

#ifndef __priorityQueue_h
#define __priorityQueue_h __priorityQueue_h

#include <list>
#include <vector>
#include <map>
#include <iostream>





using namespace std;


class eventHandler;


class calendarQueue
{


	public:
		calendarQueue (unsigned int size, eventHandler& p) ;
		
		void cycleMarker();

		bool operator() ( const unsigned int  s1, const unsigned int s2 ) const;

		void update (unsigned int i);
		void increaseKey (unsigned int i);
		void decreaseKey (unsigned int i);   // higher priority
			

		void debug ();
		



		void push (unsigned int i);
		unsigned int top ();
		unsigned int pop ();
		void nextYear ();
	private:
		static eventHandler *priorities;
		int head;
		unsigned int nBuckets;
		unsigned int currentBucket; 
//		vector <set < unsigned int , calendarQueue> > buckets; 
		vector <list < unsigned int > > buckets; 


		bool isEmpty;
		unsigned int firstElement;
		list <unsigned int>::iterator marker, newMarker;
//		map <unsigned int, unsigned int>  bucketOf;
		vector <unsigned int> bucketOf;

};









#endif
