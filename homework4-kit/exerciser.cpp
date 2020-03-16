#include "exerciser.h"

void exercise(connection *C)
{
  query1(C,1,29,34,1,5,11,1,5,11,1,1,5,1,1.2,1.7,1,0.1,0.4);
  query1(C,1,0,50,1,0,50,1,0,50,1,0,50,1,0.2,10.0,1,0.2,10.0);
  query2(C,"Red");
  query3(C,"NCSU");
  query4(C,"NC","DarkBlue");
  query5(C,10);
}
