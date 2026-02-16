#pragma once
#include <atomic>
#include <cstddef>

class CounterService {
public:
    size_t increment();
    size_t value() const;

private:
    std::atomic<size_t> m_counter{0};
};
