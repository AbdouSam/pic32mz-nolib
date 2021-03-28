
#include "pic32_config.h"
#include "app.h"

int main(void)
{
  app_init();

  for (;; )
    {
      app_task();
    }

  return 0;
}