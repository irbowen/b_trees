#include "helper.h"

using namespace std;

mutex cout_mutex;

void safe_cout(string str, int thread_num) {
		return;
    unique_lock<mutex> lock(cout_mutex);
    if(thread_num == 2)
	    cout << "\tThread " << thread_num << ": " << str << endl;
    else
    	cout << "Thread " << thread_num << ": " << str << endl;

}
