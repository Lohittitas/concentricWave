#include "axi.h"
#include "navier-stokes/centered.h"
#include "two-phase.h"
#include "reduced.h"
#include "tension.h"
#define resolution 1024
#define epsilon 0.061
#define mean 2.141


//NO PENETRATION B.C//
uf.n[left]   = 0.;
uf.n[right]  = 0.;
uf.n[top]    = 0.;
uf.n[bottom] = 0.;

int main(){
	//PARAMETERS//
	origin(-mean,0.);
	G.x = -981.0;
	rho1 = 1.0, rho2 = 0.0010;
	f.sigma =72.0;
	mu1 = 1e-2, mu2 = 1e-4;
	TOLERANCE = 1e-6;

	char comm[80];
  sprintf (comm, "mkdir -p intermediate");
  system(comm);
	
	size(4.282);
	init_grid(resolution);
	run();
}

//INITIAL COLOR FUNCTION FIELD//
event init (t = 0){
	double domain = 4.282;
	double a0 = epsilon*domain;
	double b=1.5625; 
	fraction (f, -((x ) +a0*(1-b*y*y)*exp(-b*y*y)));
	foreach(){
		u.x[] = 0;
		u.y[] = 0;
	}
}

event vof (i++, first);

event display_running(i++){
	printf (" TIME : %g",t);
	putchar ('\n');
	fflush (stdout);
}

event interface (t = 0.0; t <= 0.5; t+=0.001) {
	char namedump[80];
	sprintf (namedump, "intermediate/snapshot-%5.4f", t);
	
	// Vatsal: why do you write pid as well? Also, do you need the pressure? saving these frequenty can be memory intensive and lead to code stalls.
	// scalar pid[];
	// foreach()
	// pid[] = fmod(pid()*(npe() + 37), npe());
	// boundary ({pid});
	// p.nodump = false;
	
	dump(namedump);

}
event adapt (i++) {
  adapt_wavelet ({f,u}, (double[]){1e-3,1e-3,1e-3},8);
}