#include "Roasty.hpp"
#include "Storage/DiskStorage.hpp"

int main() {
  auto* storage = new DiskStorage{};

  Roasty roasty{storage};

  roasty.startServer();
}
