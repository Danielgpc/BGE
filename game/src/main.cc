#include <bge.h>

int main() {
  BGE instance;
  if (instance.init() != 0)
    return 1;
  instance.run();
  instance.shutdown();
  return 0;
}
