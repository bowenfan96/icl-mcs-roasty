#pragma once

#include "httplib.h"
#include <string>
#include <utility>

template <typename RoastyImplementation> class RoastyServer {
public:
  RoastyServer(std::string const& interface, int const port, RoastyImplementation* requestHandler)
      : interface(interface), port(port), requestHandler(requestHandler) {}

  void startServer();
  int getPort() const { return port; }
  std::string getInterface() const { return interface; }

private:
  int const port;
  std::string const interface;
  httplib::Server srv;
  RoastyImplementation* requestHandler;
};
