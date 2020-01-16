#include "signal.hh"

#include <cerrno>
#include <cstring>
#include <stdexcept>

using namespace os;

std::map<int, SignalHandlerPtr> Signal::handlerMap{};

void Signal::action(int signum, SignalHandlerPtr signalHandler) {
    struct sigaction sa;

    sa.sa_sigaction = Signal::dispatcher;
    sa.sa_flags = SA_RESTART;

    auto hasError = sigemptyset(&sa.sa_mask);
    if (hasError) {
        throw std::runtime_error(std::strerror(errno));
    }

    hasError = sigaction(signum, &sa, NULL);
    if (hasError) {
        throw std::runtime_error(std::strerror(errno));
    }

    handlerMap[signum] = signalHandler;
}

void Signal::dispatcher(int signal, siginfo_t* info, void* userContext) {
    auto action = handlerMap[signal];
    if (action) {
        action->handle(signal, info, userContext);
    }
}
