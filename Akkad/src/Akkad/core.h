#pragma once
#include <memory>

template<typename T>
using SharedPtr = std::shared_ptr<T>;

template<typename T, typename ... Args>
SharedPtr<T> CreateSharedPtr(Args&& ... args) {
	return std::make_shared<T>(std::forward<Args>(args)...);
}