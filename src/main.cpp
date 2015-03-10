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

#include <iostream>
#include <string>

#include <channel/Channel.h>

using namespace channel;
using namespace std;

int main() {

    Channel<string> channel;
    auto& sink = channel.sink();
    auto& source = channel.source();

    sink.give("abcde");
    sink.give("12345");
    sink.give("hehe");
    sink.give("öä+");

    auto msgs1 = source.takeWithin(std::chrono::milliseconds(1));

    sink.give("wglalala");
    sink.give("1aeh2345");
    sink.give("heaeg4he");
    sink.give("öä+ga4");

    auto msgs2 = source.takeNow();
    auto msgs3 = source.takeNow();

    for (const auto& msgs : { msgs1, msgs2, msgs3 })
        for (const auto& msg : msgs)
            cout << msg << endl;

    return 0;
}

