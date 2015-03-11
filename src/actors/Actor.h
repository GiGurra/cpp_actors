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
#include <mutex>
#include <vector>
#include <functional>
#include <queue>

#include <actors/ActorContext.h>
#include <actors/IActorRef.h>

namespace actors {

template<class T>
class ActorBase: public virtual IActorRef<T> {
public:

    ActorBase(ActorContext * context, void * actor_ptr) :
                    context_(context),
                    actor_ptr_(actor_ptr) {
    }

    virtual ~ActorBase() {
    }

protected:

    virtual void handle(T& message) = 0;

    void send(const T& data) override final {
        context_->post(actor_ptr_, [this, data] {
            T message = data;
            handle(message);
        });
    }

    void send(T&& data) override final {
        context_->post(actor_ptr_, [this, data] {
            T message = std::move(data);
            handle(message);
        });
    }

private:
    ActorContext * context_;
    void * actor_ptr_;

};

template<class ... T_Msgs>
class Actor: public ActorBase<T_Msgs> ..., public virtual IActorRef<T_Msgs...> {

public:

    Actor(ActorContext * context = ActorContext::default_instance()) :
                    ActorBase<T_Msgs>(context, this) ... {
    }

    virtual ~Actor() {
    }

private:

};

}

#endif /* CPP_CHANNEL_CHANNEL_H_ */
