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

#include <vector>
#include <mutex>
#include <chrono>

namespace actors {

template<class T>
class Buffer {
public:

    Buffer() {
    }

    virtual ~Buffer() {
    }

    void flush();

    virtual void handle(T& message) = 0;

protected:
    void doSend(const T& data) {
        items_.push_back(data);
    }

    void doSend(T&& data) {
        items_.push_back(std::move(data));
    }

    std::vector<T> items_;

};

template<class ... MsgTypes>
class Actor: public Buffer<MsgTypes> ... {
public:

    Actor() :
                    message_count_(0) {
    }

    virtual ~Actor() {
    }

    template<typename T>
    void send(const T& t) {
        std::unique_lock<std::mutex> lock(mutex_);
        this->Buffer<T>::doSend(t);
    }

    template<typename T>
    void send(T&& t) {
        std::unique_lock<std::mutex> lock(mutex_);
        this->Buffer<T>::doSend(t);
    }

    void handleNow() {
        std::unique_lock<std::mutex> lock(mutex_);
        flush(this->Buffer<MsgTypes>::items_...);
        message_count_ = 0;
    }

    template<typename _Rep, typename _Period>
    void handleWithin(const std::chrono::duration<_Rep, _Period>& __d) {
        std::unique_lock<std::mutex> lock(mutex_);
        if (message_count_ == 0) {
            // Do some waiting
        }
        flush(this->Buffer<MsgTypes>::items_...);
        message_count_ = 0;
    }

private:
    std::mutex mutex_;
    int message_count_;

    template<typename T>
    void flush(std::vector<T>& items) {
        for (T& item : items)
            static_cast<Buffer<T>*>(this)->handle(item);
        items.clear();
    }

    template<typename Head, typename ... Tail>
    void flush(std::vector<Head>& head, std::vector<Tail>& ... tail) {
        flush(head);
        flush(tail...);
    }

};

}

#endif /* CPP_CHANNEL_CHANNEL_H_ */
