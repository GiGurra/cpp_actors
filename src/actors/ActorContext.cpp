/*
 * ActorContext.cpp
 *
 *  Created on: Mar 11, 2015
 *      Author: johan
 */

#include <actors/ActorContext.h>

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstdbool>
#include <iterator>
#include <mutex>
#include <set>
#include <thread>
#include <utility>
#include <vector>

using namespace std;

namespace actors {

class ActorContext::imp {
    typedef function<void()> Call;

public:

    imp(const int nThreads) :
                    n_threads_(nThreads),
                    kill_flag_(false) {
    }

    virtual ~imp() {
    }

    void post(void* readyActor, Call call) {
        unique_lock<mutex> lock(mutex_);
        queued_.push_back(make_pair(readyActor, call));
        condition_.notify_one();
    }

    void start() {
        for (int i = 0; i < n_threads_; i++) {
            threads_.push_back(thread([&] {
                while(!kill_flag_) {
                    thread_tic();
                }
            }));
        }
    }

    void signalStop() {
        kill_flag_ = true;
    }

    void join() {
        for (auto& t : threads_)
            t.join();
        threads_.clear();
    }

private:
    const int n_threads_;

    mutex mutex_;

    vector<pair<void*, Call>> queued_;
    set<void*> acting_;

    condition_variable condition_;
    vector<thread> threads_;
    atomic_bool kill_flag_;

    imp(const imp& other) = delete;
    imp& operator=(const imp& other) = delete;

    void thread_tic() {

        void * actor = nullptr;
        Call call = [] {};

        {
            unique_lock<mutex> lock(mutex_);
            if (queued_.empty())
                condition_.wait_for(lock, chrono::milliseconds(50));

            if (queued_.empty())
                return;

            for (auto it = queued_.begin(); it != queued_.end(); it++) {
                if (!acting_.count(it->first)) {
                    actor = it->first;
                    call = it->second;
                    acting_.insert(actor);
                    queued_.erase(it);
                    break;
                }
            }

        }

        if (actor) {
            call();

            { // This actor may have more tasks
                unique_lock<mutex> lock(mutex_);
                acting_.erase(actor);
                condition_.notify_one();
            }
        }
    }

};

ActorContext::ActorContext(const int nThreads) :
                imp_(new imp(nThreads)) {
    imp_->start();
}

ActorContext::~ActorContext() {
    imp_->signalStop();
    imp_->join();
    delete imp_;
}

void ActorContext::signalStop() {
    imp_->signalStop();
}

void ActorContext::join() {
    imp_->join();
}

void ActorContext::post(void* readyActor, function<void()> call) {
    imp_->post(readyActor, call);
}

ActorContext * ActorContext::default_instance() {
    static ActorContext default_inst;
    return &default_inst;
}

} /* namespace actors */
