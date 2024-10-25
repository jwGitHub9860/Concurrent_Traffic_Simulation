#include <iostream>
#include <random>
#include <chrono>
#include <cstdlib>  // defines "rand()" & "srand()"
#include <ctime>    // defines "time()"
#include "TrafficLight.h"

using namespace std;

/* Implementation of class "MessageQueue" */

 
template <typename T>
T MessageQueue<T>::receive()
{
    // FP.5a : The method receive should use std::unique_lock<std::mutex> and _condition.wait() 
    // to wait for and receive new messages and pull them from the queue using move semantics. 
    // The received object should then be returned by the receive function. 
    return r_val;
}

template <typename T>
void MessageQueue<T>::send(T &&msg)
{
    // FP.4a : The method send should use the mechanisms std::lock_guard<std::mutex> 
    // as well as _condition.notify_one() to add a new message to the queue and afterwards send a notification.
    lock_guard<mutex> lck_gurd(mtx);    // Locks mutex when "lock_guard" is created & Unlocked when destroyed (goes out of scope)
    _condition.notify_one();    // unblocks 1 of the threads waiting for condition_variable (_condition)         adds new message to queue & sends notification afterwards
}


/* Implementation of class "TrafficLight" */


TrafficLight::TrafficLight()    // constructor
{
    _currentPhase = TrafficLightPhase::red;
}
/*
void TrafficLight::waitForGreen()
{
    // FP.5b : add the implementation of the method waitForGreen, in which an infinite while-loop 
    // runs and repeatedly calls the receive function on the message queue. 
    // Once it receives TrafficLightPhase::green, the method returns.
}*/

TrafficLightPhase TrafficLight::getCurrentPhase()
{
    return _currentPhase;
}

void TrafficLight::simulate()
{
    // FP.2b : Finally, the private method „cycleThroughPhases“ should be started in a thread when the public method „simulate“ is called. To do this, use the thread queue in the base class. 
    thread t(cycleThroughPhases);   // starts "cycleThroughPhases" in thread
    // DO THIS ----> use the thread queue in the base class. 
}

// virtual function which is executed in a thread
void TrafficLight::cycleThroughPhases()
{
    // FP.2a : Implement the function with an infinite loop that measures the time between two loop cycles 
    // and toggles the current phase of the traffic light between red and green and sends an update method 
    // to the message queue using move semantics. The cycle duration should be a random value between 4 and 6 seconds. 
    // Also, the while-loop should use std::this_thread::sleep_for to wait 1ms between two cycles. 
    while (true)    // infinite loop
    {
        srand(time(0));     // ensures each program run produces different random numbers

        int loop_num = 0;   // initializes "loop_num"

        while (true)    // infinite loop
        {
            auto cycle_duration = chrono::seconds((rand() % 3) + 4);    // limits cycle duration to 4-6 seconds       (rand() % 3) ---> produces random number between 0 & 2

            _currentPhase = (_currentPhase == TrafficLightPhase::red) ? TrafficLightPhase::red : TrafficLightPhase::green;  // toggles between "red" & "green"
            cout << "Traffic Light is " << _currentPhase << endl;    // Traffic Light message
            //this_thread::sleep_for(chrono::seconds(rand_value));    // waits between 4 to 6 seconds before switching light
            //_currentPhase = TrafficLightPhase::green;
            cout << "Traffic Light is green." << endl;    // Traffic Light message
            //this_thread::sleep_for(chrono::seconds(rand_value));    // waits between 4 to 6 seconds before switching light


            // SEND UPDATE METHOD TO MESSAGE QUEUE USING MOVE SEMANTICS


            loop_num++;
            if (loop_num % 2 == 0)  // checks if 2 loops have passed
            {
                cout << "Two loop cycle time: " << cycle_duration.count() << " seconds" << endl;

                this_thread::sleep_for(chrono::milliseconds(1));    // waits 1ms AFTER 2 cycles passed
            }
        }
    }
}

