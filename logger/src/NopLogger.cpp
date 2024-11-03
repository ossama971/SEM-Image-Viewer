#include "NopLogger.h"

// Initialize static members
boost::mutex NopLogger::mtx;
NopLogger* NopLogger::instance = nullptr;

NopLogger* NopLogger::get_instance() {
    if (!instance) {
        boost::unique_lock<boost::mutex> lock(mtx);
        if (!instance) {
            instance = new NopLogger(); // Create instance if it doesn't exist
        }
        // No need to explicitly unlock the lock as it will go out of scope
    }
    return instance;
}

bool NopLogger::enabled(const Metadata &metadata) {
    return false; // Always returns false
}

void NopLogger::log(const Record &record) {
    // No-op
}

void NopLogger::flush() {
    // No-op
}

