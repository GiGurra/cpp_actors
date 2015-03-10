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

#ifndef ACTORS_IACTORREF_H_
#define ACTORS_IACTORREF_H_

namespace actors {

template<typename T>
class IActorRefBase {
public:

    IActorRefBase() {
    }

    virtual ~IActorRefBase() {
    }

    virtual void send(T&& message) = 0;

    virtual void send(const T& message) = 0;

protected:

    void sendTrampoline(T&& message) {
        send(std::forward<T&&>(message));
    }

    void sendTrampoline(const T& message) {
        send(message);
    }

};

template<typename ... T_Msgs>
class IActorRef: public virtual IActorRefBase<T_Msgs> ... {
public:

    template<typename T>
    void send(T&& message) {
        this->IActorRefBase<T>::sendTrampoline(std::forward<T&&>(message));
    }

    template<typename T>
    void send(const T& message) {
        this->IActorRefBase<T>::sendTrampoline(message);
    }

};

} /* namespace actors */

#endif /* ACTORS_IACTORREF_H_ */
