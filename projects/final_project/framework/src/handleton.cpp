#define __HANDLETON__

#include "handleton.hpp"
#include "thread_pool.hpp"
#include "logger.hpp"
#include "scheduler.hpp"
#include "factory.hpp"
#include "icommand.hpp"
#include "reactor.hpp"

template class ilrd::Handleton<ilrd::Logger>;
template class ilrd::Handleton<ilrd::Scheduler>;
template class ilrd::Handleton<ilrd::ThreadPool>;
template class ilrd::Handleton<ilrd::Factory<ilrd::ICommand, int> >;