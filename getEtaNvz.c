/* Title: Calculating the jet height and the tip velocity
# v2.0
# Last updated: Sep 26, 2024
# Authors: Vatsal Sanjay, Ayush Dixit, & Aman Bhargava
# vatsalsanjay@gmail.com
# Physics of Fluids
*/

#include "navier-stokes/centered.h"
#include "fractions.h"
#include "tag.h"

char filename[80], writefile[80];
scalar f[];
vector u;
double Ymax = 5e-2;
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
  sprintf(writefile, "%s", argv[2]);
  restore(file = filename);

  double threshold = 1e-4;

  // Tag all liquid parts and find the largest connected region
  TagResult liquid_result = tag_largest_region(f, threshold);
  int main_phase_liquid = liquid_result.main_phase;
  scalar d_liquid = liquid_result.d;

  FILE *fp = fopen(writefile, "a");
  if (t == 0) {
    fprintf(fp, "t,r,eta,vz\n");
  }

  double Xeta = -HUGE;
  double vz = 0;
  double rTP = 0;

  foreach_boundary(bottom) {
    if (f[] > 1e-6 && f[] < 1. - 1e-6 && d_liquid[] == main_phase_liquid) {
      // printf("%g %g %g %g\n", x, y, f[], u.x[]);
      coord n = interface_normal(point, f);
      double alpha = plane_alpha(f[], n);
      coord segment[2];

      if (facets(n, alpha, segment) == 2) {
        double x0 = x + (segment[0].x + segment[1].x) * Delta / 2.;
        double y0 = y + (segment[0].y + segment[1].y) * Delta / 2.;
        if (y0 < Ymax && x0 > Xeta){
          Xeta = x0;
          rTP = y0;
          vz = interpolate(u.x, x0, 0.0);
        }
      }

    }
  }
  fprintf(fp, "%5.4f,%5.4e,%5.4e,%5.4e\n", t, rTP, Xeta, vz);
  fflush(fp);
  fclose(fp);

  FILE *fp2 = ferr;
  fprintf(fp2, "%5.4e,%5.4e,%5.4e\n", rTP, Xeta, vz);
  fflush(fp2);
  fclose(fp2);
  dump(file="test");
}