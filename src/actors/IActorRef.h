/*
 * IActorRef.h
 *
 *  Created on: Mar 10, 2015
 *      Author: johan
 */

#ifndef ACTORS_IACTORREF_H_
#define ACTORS_IACTORREF_H_

namespace actors {

template<typename MessageType>
class IActorRef {
public:

    IActorRef() {
    }

    virtual ~IActorRef() {
    }

    virtual void send(MessageType&& message) = 0;

    virtual void send(const MessageType& message) = 0;

};

} /* namespace actors */

#endif /* ACTORS_IACTORREF_H_ */
