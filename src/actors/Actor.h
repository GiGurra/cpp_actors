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

#include <chrono>
#include <condition_variable>
#include <mutex>
#include <vector>

#include <actors/IActorRef.h>

namespace actors {

template<class T>
class ActorBase: public virtual IActorRef<T> {
public:

    ActorBase(
            std::mutex& mutex,
            int& message_count,
            std::condition_variable& condition) :
                    mutex_(mutex),
                    message_count_(message_count),
                    condition_(condition) {
    }

    virtual ~ActorBase() {
    }

protected:

    virtual void handle(T& message) = 0;

protected:

    void handleTrampoline(T& message) {
        handle(message);
    }

    void send(const T& data) override final {
        std::unique_lock<std::mutex> lock(mutex_);
        items_.push_back(data);
        message_count_++;
        condition_.notify_all();
    }

    void send(T&& data) override final {
        std::unique_lock<std::mutex> lock(mutex_);
        items_.push_back(std::move(data));
        message_count_++;
        condition_.notify_all();
    }

    std::vector<T> items_;
    std::mutex& mutex_;
    int& message_count_;
    std::condition_variable& condition_;

};

template<class ... T_Msgs>
class Actor: public ActorBase<T_Msgs> ..., public virtual IActorRef<T_Msgs...> {
public:

    Actor() :
                    ActorBase<T_Msgs>(mutex_, message_count_, condition_) ...,
                    message_count_(0) {
    }

    virtual ~Actor() {
    }

    void handleNow() {
        std::unique_lock<std::mutex> lock(mutex_);
        handle(this->ActorBase<T_Msgs>::items_...);
        message_count_ = 0;
    }

    template<typename _Rep, typename _Period>
    void handleWithin(const std::chrono::duration<_Rep, _Period>& timeout) {
        std::unique_lock<std::mutex> lock(mutex_);
        if (message_count_ == 0)
            condition_.wait_for(lock, timeout);
        handle(this->ActorBase<T_Msgs>::items_...);
        message_count_ = 0;
    }

private:
    std::mutex mutex_;
    int message_count_;
    std::condition_variable condition_;

    template<typename T>
    void handle(std::vector<T>& items) {
        for (T& item : items)
            this->ActorBase<T>::handleTrampoline(item);
        items.clear();
    }

    template<typename Head, typename ... Tail>
    void handle(std::vector<Head>& head, std::vector<Tail>& ... tail) {
        handle(head);
        handle(tail...);
    }

};

}

#endif /* CPP_CHANNEL_CHANNEL_H_ */
