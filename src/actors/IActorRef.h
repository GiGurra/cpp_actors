/*
 * IActorRef.h
 *
 *  Created on: Mar 10, 2015
 *      Author: johan
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
