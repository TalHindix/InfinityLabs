#define __HANDLETON__

#include "handleton.hpp"

#include "logger.hpp"
#include "scheduler.hpp"

template class ilrd::Handleton<ilrd::Logger>;
template class ilrd::Handleton<ilrd::Scheduler>;