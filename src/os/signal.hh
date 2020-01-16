#ifndef __SIGNAL_HH
#define __SIGNAL_HH

#include <map>
#include <memory>
#include <signal.h>

namespace os {
class SignalHandler {
  public:
    virtual ~SignalHandler() {}
    virtual void handle(int signal, siginfo_t* info, void* userContext) = 0;
};

using SignalHandlerPtr = std::shared_ptr<SignalHandler>;

class Signal {
  public:
    static void action(int signum, SignalHandlerPtr signalHandler);

  private:
    static std::map<int, SignalHandlerPtr> handlerMap;

    static void dispatcher(int signal, siginfo_t* info, void* userContext);
};
} // namespace os

#endif
