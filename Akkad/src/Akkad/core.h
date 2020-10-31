#pragma once
#include <memory>

template<typename T>
using SharedPtr = std::shared_ptr<T>;

template<typename T>
SharedPtr<T> CreateSharedPtr() {
	return std::make_shared<T>();
}