/*
 * ActorContext.h
 *
 *  Created on: Mar 11, 2015
 *      Author: johan
 */

#ifndef ACTORS_ACTORCONTEXT_H_
#define ACTORS_ACTORCONTEXT_H_

#include <functional>

namespace actors {
class IActor;

class ActorContext {
public:
    ActorContext(const int nThreads = 4);
    virtual ~ActorContext();

    static ActorContext * default_instance();

    void post(void* readyActor, std::function<void()> call);
    void signalStop();
    void join();

private:
    class imp;
    imp * imp_;

    ActorContext(const ActorContext& other) = delete;
    ActorContext& operator=(const ActorContext& other) = delete;
};

} /* namespace actors */

#endif /* ACTORS_ACTORCONTEXT_H_ */
