#include "CounterService.hpp"

size_t CounterService::increment()
{
  return ++m_counter;
}

size_t CounterService::value() const
{
  return m_counter.load();
}
