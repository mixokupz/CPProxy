#include <unordered_set>
#include <utility>
#include <vector>

#include <pthread.h>

#include "wrappers.h"

class CacheEntry {
public:
    enum class State {
        LOADING,
        SUCCESS,
        FAILED
    };

    State state = State::LOADING;
    std::string key;
    
    char* data = nullptr;
    size_t capacity = 10*1024*1024;
    size_t size = 0;

    LockWrapper lock;
    CVWrapper data_change_cv;

    AtomicInt listeners_count;

    ~CacheEntry(){
    	delete[] data;
    }
};
