#include "common.h"

void randomize_asteroids (POINTL *aster_data, int dim, int radius)
{
  int i;
  float incr;

  incr = (M_PI * 2 / (dim-1));

  for (i = 0; i < dim; i++)
  {
    aster_data[i].x = cos(i * incr) * (random(radius)+ radius/2);
    aster_data[i].y = sin(i * incr) * (random(radius)+ radius/2);
  }

  aster_data[dim - 1] = aster_data[0];
}