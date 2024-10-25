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
    unique_lock<mutex> uni_lck(mtx);    // performs queue modification under the lock
    _condition.wait(uni_lck, [this] { return != _queue.empty(); })   // passes unique lock to "_condition"       [this] { return != _messages.empty(); } ----> lambda function that give "this" access to "_messages"
    
    T msg = move(_queue.front());   // converts 1st "_messages" value into rvalue using Move Semantics
    _queue.pop_front();

    return msg;
}

template <typename T>
void MessageQueue<T>::send(T &&msg)
{
    // FP.4a : The method send should use the mechanisms std::lock_guard<std::mutex> 
    // as well as _condition.notify_one() to add a new message to the queue and afterwards send a notification.
    this_thread::sleep_for(chrono::milliseconds(100));  // mimics work
    lock_guard<mutex> lck_gurd(mtx);    // Locks mutex when "lock_guard" is created & Unlocked when destroyed (goes out of scope)
    cout << "Traffic Light is " << _msg << endl;    // Traffic Light message
    _messages.push_back(move(msg));     // adds vector (deque) to queue
    _condition.notify_one();    // unblocks 1 of the threads waiting for condition_variable (_condition)         adds new message to queue & sends notification afterwards
}


/* Implementation of class "TrafficLight" */


TrafficLight::TrafficLight()    // constructor
{
    _currentPhase = TrafficLightPhase::red;
}

void TrafficLight::waitForGreen()
{
    // FP.5b : add the implementation of the method waitForGreen, in which an infinite while-loop 
    // runs and repeatedly calls the receive function on the message queue. 
    // Once it receives TrafficLightPhase::green, the method returns.
    while (true)
    {
        /* code */
    }
    
}

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
        srand(time(NULL));     // ensures each program run produces different random numbers
        while (true)    // infinite loop
        {
            auto cycle_duration = chrono::seconds((rand() % 3) + 4);    // limits cycle duration to 4-6 seconds       (rand() % 3) ---> produces random number between 0 & 2
            this_thread::sleep_for(chrono::milliseconds(cycle_duration));    // waits 4-6 seconds
            _currentPhase = (_currentPhase == TrafficLightPhase::red) ? TrafficLightPhase::red : TrafficLightPhase::green;  // toggles between "red" & "green"
            _messages.send(move(_currentPhase));    // "move()" converts "_currentPhase" into rvalue
            this_thread::sleep_for(chrono::milliseconds(1));    // waits 1ms
        }
    }
}

