#include <iostream.h>
#include <stdio.h>
#include <math.h>

main()
{
  double (*func)(void);
  func = (double (*)())fabs;

  cerr << func(10) << endl;
}
