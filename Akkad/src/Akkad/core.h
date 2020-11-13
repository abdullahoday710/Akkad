#pragma once
#include <memory>
#include <iostream>

template<typename T>
using SharedPtr = std::shared_ptr<T>;

template<typename T, typename ... Args>
SharedPtr<T> CreateSharedPtr(Args&& ... args) {
	return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T, typename Arg>
SharedPtr<T> DynamicCastPtr(Arg arg) {

    return std::dynamic_pointer_cast<T>(std::forward<Arg>(arg));
}

#ifdef AK_DEBUG
#   define AK_ASSERT(condition, message) \
    do { \
        if (! (condition)) { \
            std::cerr << "Assertion `" #condition "` failed in " << __FILE__ \
                      << " line " << __LINE__ << ": " << message << std::endl; \
            std::terminate(); \
        } \
    } while (false)
#else
#   define AK_ASSERT(condition, message) do { } while (false)
#endif
