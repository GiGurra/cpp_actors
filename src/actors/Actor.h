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
#include <functional>

#include <actors/IActorRef.h>

namespace actors {

template<class T>
class ActorBase: public virtual IActorRef<T> {
public:

    ActorBase(
            std::mutex& mutex,
            std::condition_variable& condition,
            std::vector<std::function<void()>>& calls) :
                    mutex_(mutex),
                    condition_(condition),
                    calls_(calls),
                    call_offs_(0) {
    }

    virtual ~ActorBase() {
    }

protected:

    virtual void handle(T& message) = 0;

    void send(const T& data) override final {
        std::unique_lock<std::mutex> lock(mutex_);
        items_.push_back(data);
        calls_.push_back([&] {handleOne();});
        condition_.notify_all();
    }

    void send(T&& data) override final {
        std::unique_lock<std::mutex> lock(mutex_);
        items_.push_back(std::move(data));
        calls_.push_back([&] {handleOne();});
        condition_.notify_all();
    }

    void handleOne() {
        handle(items_[call_offs_++]);
        if (call_offs_ == items_.size()) {
            call_offs_ = 0;
            items_.clear();
        }
    }

private:
    std::vector<T> items_;
    std::mutex& mutex_;
    std::condition_variable& condition_;
    std::vector<std::function<void()>>& calls_;
    int call_offs_;

};

template<class ... T_Msgs>
class Actor: public ActorBase<T_Msgs> ..., public virtual IActorRef<T_Msgs...> {
public:

    Actor() :
                    ActorBase<T_Msgs>(mutex_, condition_, calls_) ... {
    }

    virtual ~Actor() {
    }

    void handleNow() {
        std::unique_lock<std::mutex> lock(mutex_);
        for (auto& call : calls_)
            call();
        calls_.clear();
    }

    template<typename _Rep, typename _Period>
    void handleWithin(const std::chrono::duration<_Rep, _Period>& timeout) {
        std::unique_lock<std::mutex> lock(mutex_);
        if (calls_.empty())
            condition_.wait_for(lock, timeout);
    }

private:
    std::mutex mutex_;
    std::condition_variable condition_;
    std::vector<std::function<void()>> calls_;

};

}

#endif /* CPP_CHANNEL_CHANNEL_H_ */
