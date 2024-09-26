/* Title: Calculating the jet height and the tip velocity
# v2.0
# Last updated: Sep 26, 2024
# Authors: Vatsal Sanjay, Ayush Dixit, & Aman Bhargava
# vatsalsanjay@gmail.com
# Physics of Fluids
*/

#include "utils.h"
#include "output.h"
#include "fractions.h"
#include "tag.h"

char filename[80];
scalar f[];
vector u;
double Ymax = 1e-2;
// Structure to hold the result of the tagging function
typedef struct {
  int main_phase;
  scalar d;
} TagResult;

// Function to tag connected regions and find the largest one
TagResult tag_largest_region(scalar field, double threshold) {
  scalar d[];
  foreach() {
    d[] = (field[] > threshold);
  }

  int n = tag(d), size[n];
  for (int i = 0; i < n; i++) {
    size[i] = 0;
  }

  foreach_leaf() {
    if (d[] > 0) {
      size[((int) d[]) - 1]++;
    }
  }

  int max_size = 0;
  int main_phase = 0;
  for (int i = 0; i < n; i++) {
    if (size[i] > max_size) {
      max_size = size[i];
      main_phase = i + 1;
    }
  }

  TagResult result = {main_phase, d};
  return result;
}

int main(int argc, char const *argv[]) {
  sprintf(filename, "%s", argv[1]);
  restore(file = filename);

  double threshold = 1e-4;

  // Tag all liquid parts and find the largest connected region
  TagResult liquid_result = tag_largest_region(f, threshold);
  int main_phase_liquid = liquid_result.main_phase;
  scalar d_liquid = liquid_result.d;

  FILE *fp = ferr;

  double Xeta = -HUGE;
  double vz = 0;
  foreach() {
    if (f[] > 1e-6 && f[] < 1. - 1e-6 && d_liquid[] == main_phase_liquid) {
      coord n = interface_normal(point, f);
      double alpha = plane_alpha(f[], n);
      coord segment[2];

      if (facets(n, alpha, segment) == 2) {
        double x0 = x + (segment[0].x + segment[1].x) * Delta / 2.;
        double y0 = y + (segment[0].y + segment[1].y) * Delta / 2.;
        if (y0 < Ymax && x0 > Xeta){
          Xeta = X0;
          vz = interpolate(u.x, x0, y0);
        }
      }
    }
  }
  fprintf(fp, "%g %g\n", Xeta, vz);
  fflush(fp);
  fclose(fp);

}