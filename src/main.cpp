/*
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Johan Kjölhede
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#include <actors/Actor.h>
#include <actors/IActorRef.h>
#include <iostream>
#include <string>

#include <thread>

using namespace std;
using namespace chrono;
using namespace actors;

class StringIntActor: public Actor<string, int> {
public:

    void handle(string& message) override {
        cout << "Got string: " << message << endl;
    }

    void handle(int& message) override {
        cout << "Got int: " << message << endl;
    }

};

int main() {

    StringIntActor actor;

    IActorRef<string>& stringActor = actor;
    IActorRef<int>& intActor = actor;
    IActorRef<string, int>& ref2 = actor;

    stringActor.send(string("abc"));
    intActor.send(1);

    ref2.send(string("ABC"));
    ref2.send(2);
    ref2.send(string("XYZ"));
    ref2.send(3);


    this_thread::sleep_for(milliseconds(1000));

    ActorContext::default_instance()->signalStop();
    ActorContext::default_instance()->join();

}

