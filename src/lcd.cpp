#include "core.h"
#include "SPI.h"

#define USER_B(f) f(2,6)
#define USER_L(f) f(2,7)
// #define TEST(f)   f(0,9)  // A4

int main(void)
{
  // delay_us(0);
  // _empty = 0;
  // init();


  USER_B(GPIO);
  USER_B(IN);

  USER_L(GPIO);
  USER_L(OUT);


  // TEST(GPIO);
  // TEST(OUT);

  // SPI.init();
  // test.init();
  // SPI.begin(test);

  while (1) {
    if (USER_B(GET)) {
      // SPI.send(0);
      USER_L(SET);
    }
    else {
      USER_L(CLR);
    }
  }

}
