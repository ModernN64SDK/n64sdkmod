/* to make type:
cc -o makesphere makesphere.c -lm
makesphere -t > sphere.dl
makesphere > sphere.dat
*/

#include <stdio.h>
#include <math.h>

#define PI		3.14159
#define PLUSCLAMP(x)	((x>127)?127:x)
#define MINUSCLAMP(x)	((x<-128)?-128:x)
#define NCL(x)		PLUSCLAMP(MINUSCLAMP((int)x))

float minx= 80.0, maxx=240.0;
float miny= 40.0, maxy=200.0;
float minz=-80.0, maxz= 80.0;

float mins=0.0,maxs=1.0,steps=15.0;
float mint=0.0,maxt=1.0,stept=15.0;
int   func=0, dodata=1;
char  *name="sphere";

void sphere(float s, float t, float *x, float *y, float *z, float *nx, float *ny, float *nz);

void (*functions[])(float s, float t, float *x, float *y, float *z, float *nx, float *ny, float *nz) = {
	sphere,
};

void dosphere()
{
	float s,t,x,y,z,nx,ny,nz;
	float stps, stpt;
	int   block=0;
	
	
	stps=(maxs-mins)/steps;
	stpt=(maxt-mint)/stept;
	for (s=mins; s<maxs; s += stps) {
	for (t=mint; t<maxt; t += stpt) {
	  if (dodata) {
		printf("static Vtx %s_vtx_%d[4] = {\n",name,block++);
		(*functions[func])(s,t,&x,&y,&z,&nx,&ny,&nz);
		printf("{%4d,%4d,%4d, 0, ( 0<<6), ( 0<<6), %4d, %4d, %4d, 0xff },\n",
		       (int) x, (int) y, (int) z, 
		       (int) nx, (int) ny, (int) nz);

		(*functions[func])(s,t+stpt,&x,&y,&z,&nx,&ny,&nz);
		printf("{%4d,%4d,%4d, 0, ( 0<<6), (31<<6), %4d, %4d, %4d, 0xff },\n",
		       (int) x, (int) y, (int) z, 
		       (int) nx, (int) ny, (int) nz);
		
		(*functions[func])(s+stps,t+stpt,&x,&y,&z,&nx,&ny,&nz);
		printf("{%4d,%4d,%4d, 0, (31<<6), (31<<6), %4d, %4d, %4d, 0xff },\n",
		       (int) x, (int) y, (int) z, 
		       (int) nx, (int) ny, (int) nz);
		
		(*functions[func])(s+stps,t,&x,&y,&z,&nx,&ny,&nz);
		printf("{%4d,%4d,%4d, 0, (31<<6), ( 0<<6), %4d, %4d, %4d, 0xff },\n",
		       (int) x, (int) y, (int) z, 
		       (int) nx, (int) ny, (int) nz);

		printf("};\n");
	  } else {
		printf("    gsSPVertex(&%s_vtx_%d, 4, 0),\n",name,block++);

		printf("    gsSPLine3D(0,1,0),\n");
		printf("    gsSPLine3D(1,2,1),\n");
		printf("    gsSPLine3D(2,3,2),\n");
		printf("    gsSPLine3D(3,0,3),\n");

		/* printf("    gsSP1Triangle(0,2,1,0),\n");
		printf("    gsSP1Triangle(0,3,2,0),\n"); */
	  }
	}
	}

}


void sphere(float s, float t, float *x, float *y, float *z, float *nx, float *ny, float *nz)
{
	float r;
	float RGV;
	

	*y = sin((t-0.5)*PI) * (maxy-miny)/2.0 + (maxy+miny)/2 + 0.5;
	r = cos((t-0.5)*PI);
	*x = r * sin(s*2*PI) * (maxx-minx)/2.0 + (maxx+minx)/2 + 0.5;
	*z = r * cos(s*2*PI) * (maxz-minz)/2.0 + (maxz+minz)/2 + 0.5;

	*nx = fsin(3.0 * 2.0 * PI * s) * 127.0 + 127.0;
	*ny = fsin(3.0 * 2.0 * PI * t) * 127.0 + 127.0;

	RGV = (*nx/255.0) * (*nx/255.0) + (*ny/255.0) * (*ny/255.0);
	if (RGV < 0.25)
	  *nz = 255.0 - (*nx/255.0) * (*nx/255.0) - (*ny/255.0) * (*ny/255.0);
	else
	  *nz = 0;	  
}

void useage() 
{
	printf("makesphere: tesselate a surface and print data\n");
	printf("  makesphere <options>      (data sent to standard output)\n");
	printf("OPTIONS:\n");
	printf("h - print this message\n");
	printf("t - print out triangle commands to be inserted in display list\n");
	printf("d - print out data (static variable declarations) (default)\n");
	exit(1);
}

void doargs(int argc, char *argv[])
{
	int i=1;
	while (i<argc) {
	    switch((argv[i][0]=='-')?argv[i][1]:argv[i][0]) {
		case 'd': dodata=1; break;
		case 't': dodata=0; break;
		case 'h':
		default:
			useage();
	    }
	    i++;
	}
}

void main(int argc, char *argv[])
{
	doargs(argc,argv);
	dosphere();
 	exit(0);
}

