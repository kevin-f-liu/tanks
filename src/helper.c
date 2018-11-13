#include "helper.h"

void busyWait(uint32_t ticks) {
  for (int i = 0; i < ticks; i++)
    ;
}