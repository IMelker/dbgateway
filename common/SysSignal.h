//
// Created by imelker on 19.01.2021.
//

#ifndef DBGATEWAY_COMMON_SYSSIGNAL_H_
#define DBGATEWAY_COMMON_SYSSIGNAL_H_

#include <atomic>

class SysSignal {
  public:
    static void setupSignalHandling();
    static void signalHandler(int signal_number);
    static bool serviceTerminated();
    static void setServiceTerminated(bool state);

  private:
    static std::atomic<bool> terminated;
};

#endif //DBGATEWAY_COMMON_SYSSIGNAL_H_
