#pragma once

template <typename T>
class Provider {
public:
	virtual ~Provider() {
	}

    // provide something like optional<T> if T has no clear terminal value?
    // errors can be handled through exceptions or something
	virtual T peek() = 0;
	virtual T consume() = 0;
};
