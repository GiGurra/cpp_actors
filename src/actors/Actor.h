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

#ifndef CPP_CHANNEL_CHANNEL_H_
#define CPP_CHANNEL_CHANNEL_H_

#include <channel/Sink.h>
#include <channel/Source.h>

namespace channel {

template<typename MessageType>
class Channel {
public:
    Channel() :
                    source_(mutex_, condition_, messages_),
                    sink_(mutex_, condition_, messages_) {
    }

    Source<MessageType>& source() {
        return source_;
    }

    Sink<MessageType>& sink() {
        return sink_;
    }

private:
    std::mutex mutex_;
    std::condition_variable condition_;
    std::vector<MessageType> messages_;
    Source<MessageType> source_;
    Sink<MessageType> sink_;

    Channel(const Channel& other) = delete;
    Channel& operator=(const Channel& other) = delete;
};

}

#endif /* CPP_CHANNEL_CHANNEL_H_ */
