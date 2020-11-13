#pragma once
#include <exception>

#include <string>

struct RoastyServerException : public std::exception {
  RoastyServerException(std::string message, int status)
      : message(std::move(message)), status(status) {}

  std::string message;
  int status;

  const char* what() const noexcept override { return message.c_str(); }
};
