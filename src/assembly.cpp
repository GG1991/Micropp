#include <cmath>
#include <iostream>
#include "micro.h"

#define nod_index(i,j,k) ((k)*nx*ny + (j)*nx + (i))

using namespace std;

void Problem::setDisp (double *eps)
{

  if (dim == 2) {

    /* dim 2 
       | eps(0)    eps(2)/2 |
       | eps(2)/2  eps(1)   |
     */

    // y = 0
    for (int i=0; i<nx; i++) {
      double xcoor = i*dx;
      double ycoor = 0.0;
      double dux =     eps[0]*xcoor + 0.5*eps[2]*ycoor;
      double duy = 0.5*eps[2]*xcoor +     eps[1]*ycoor;
      u[i*dim  ] = dux;
      u[i*dim+1] = duy;
    }
    // y = ly
    for (int i=0; i<nx; i++) {
      double xcoor = i*dx;
      double ycoor = ly;
      double dux =     eps[0]*xcoor + 0.5*eps[2]*ycoor;
      double duy = 0.5*eps[2]*xcoor +     eps[1]*ycoor;
      u[(i+(ny-1)*nx)*dim  ] = dux;
      u[(i+(ny-1)*nx)*dim+1] = duy;
    }
    // x = 0
    for (int i=0; i<ny-2; i++) {
      double xcoor = 0.0;
      double ycoor = (i+1)*dy;
      double dux =     eps[0]*xcoor + 0.5*eps[2]*ycoor;
      double duy = 0.5*eps[2]*xcoor +     eps[1]*ycoor;
      u[(i+1)*nx*dim  ] = dux;
      u[(i+1)*nx*dim+1] = duy;
    }
    // x = lx
    for (int i=0; i<ny-2; i++) {
      double xcoor = lx;
      double ycoor = (i+1)*dy;
      double dux =       eps[0]*xcoor + 0.5 * eps[2]*ycoor;
      double duy = 0.5 * eps[2]*xcoor +       eps[1]*ycoor;
      u[((i+2)*nx-1)*dim  ] = dux;
      u[((i+2)*nx-1)*dim+1] = duy;
    }

  } else if (dim == 3) {

    /* dim 3
       | eps(0)    eps(3)/2  eps(4)/2 |
       | eps(3)/2  eps(1)    eps(5)/2 |
       | eps(4)/2  eps(5)/2  eps(2)   |
     */

    // z = 0
    for (int i=0; i<nx; i++) {
      for (int j=0; j<ny; j++) {
	int n = nod_index(i,j,0);
	double xcoor = i*dx;
	double ycoor = j*dy;
	double zcoor = 0.0;
	double dux =     eps[0]*xcoor + 0.5*eps[3]*ycoor + 0.5*eps[4]*zcoor;
	double duy = 0.5*eps[3]*xcoor +     eps[1]*ycoor + 0.5*eps[5]*zcoor;
	double duz = 0.5*eps[4]*xcoor + 0.5*eps[5]*ycoor +     eps[2]*zcoor;
	u[n*dim  ] = dux;
	u[n*dim+1] = duy;
	u[n*dim+2] = duz;
      }
    }
    // z = lx
    for (int i=0; i<nx; i++) {
      for (int j=0; j<ny; j++) {
	int n = nod_index(i,j,nz-1);
	double xcoor = i*dx;
	double ycoor = j*dy;
	double zcoor = lz;
	double dux =     eps[0]*xcoor + 0.5*eps[3]*ycoor + 0.5*eps[4]*zcoor;
	double duy = 0.5*eps[3]*xcoor +     eps[1]*ycoor + 0.5*eps[5]*zcoor;
	double duz = 0.5*eps[4]*xcoor + 0.5*eps[5]*ycoor +     eps[2]*zcoor;
	u[n*dim  ] = dux;
	u[n*dim+1] = duy;
	u[n*dim+2] = duz;
      }
    }

    // y = 0
    for (int i=0; i<nx; i++) {
      for (int k=1; k<nz-1; k++) {
	int n = nod_index(i,0,k);
	double xcoor = i*dx;
	double ycoor = 0.0;
	double zcoor = k*dz;
	double dux =     eps[0]*xcoor + 0.5*eps[3]*ycoor + 0.5*eps[4]*zcoor;
	double duy = 0.5*eps[3]*xcoor +     eps[1]*ycoor + 0.5*eps[5]*zcoor;
	double duz = 0.5*eps[4]*xcoor + 0.5*eps[5]*ycoor +     eps[2]*zcoor;
	u[n*dim  ] = dux;
	u[n*dim+1] = duy;
	u[n*dim+2] = duz;
      }
    }

    // y = ly
    for (int i=0; i<nx; i++) {
      for (int k=1; k<nz-1; k++) {
	int n = nod_index(i,ny-1,k);
	double xcoor = i*dx;
	double ycoor = ly;
	double zcoor = k*dz;
	double dux =     eps[0]*xcoor + 0.5*eps[3]*ycoor + 0.5*eps[4]*zcoor;
	double duy = 0.5*eps[3]*xcoor +     eps[1]*ycoor + 0.5*eps[5]*zcoor;
	double duz = 0.5*eps[4]*xcoor + 0.5*eps[5]*ycoor +     eps[2]*zcoor;
	u[n*dim  ] = dux;
	u[n*dim+1] = duy;
	u[n*dim+2] = duz;
      }
    }

    // x=0
    for (int j=1; j<ny-1; j++) {
      for (int k=1; k<nz-1; k++) {
	int n = nod_index(0,j,k);
	double xcoor = 0.0;
	double ycoor = j*dy;
	double zcoor = k*dz;
	double dux =     eps[0]*xcoor + 0.5*eps[3]*ycoor + 0.5*eps[4]*zcoor;
	double duy = 0.5*eps[3]*xcoor +     eps[1]*ycoor + 0.5*eps[5]*zcoor;
	double duz = 0.5*eps[4]*xcoor + 0.5*eps[5]*ycoor +     eps[2]*zcoor;
	u[n*dim  ] = dux;
	u[n*dim+1] = duy;
	u[n*dim+2] = duz;
      }
    }

    // x=lx
    for (int j=1; j<ny-1; j++) {
      for (int k=1; k<nz-1; k++) {
	int n = nod_index(nx-1,j,k);
	double xcoor = lx;
	double ycoor = j*dy;
	double zcoor = k*dz;
	double dux =     eps[0]*xcoor + 0.5*eps[3]*ycoor + 0.5*eps[4]*zcoor;
	double duy = 0.5*eps[3]*xcoor +     eps[1]*ycoor + 0.5*eps[5]*zcoor;
	double duz = 0.5*eps[4]*xcoor + 0.5*eps[5]*ycoor +     eps[2]*zcoor;
	u[n*dim  ] = dux;
	u[n*dim+1] = duy;
	u[n*dim+2] = duz;
      }
    }

  }
}

double Problem::Assembly_b (double **int_vars, bool write_int_vars)
{
  int index[3*8];
  int n0, n1, n2, n3, n4, n5, n6, n7;
  int e;

  for (int i=0 ; i<nn*dim; i++)
    b[i] = 0.0;

  if (dim == 2) {

    double be[2*4];

    for (int ex=0; ex<nx-1; ex++) {
      for (int ey=0; ey<ny-1; ey++) {

	n0 = ey     * nx + ex     ;
	n1 = ey     * nx + ex + 1 ;
	n2 = (ey+1) * nx + ex + 1 ;
	n3 = (ey+1) * nx + ex     ;

	index[0] = n0*dim; index[1] = n0*dim + 1;
	index[2] = n1*dim; index[3] = n1*dim + 1;
	index[4] = n2*dim; index[5] = n2*dim + 1;
	index[6] = n3*dim; index[7] = n3*dim + 1;

	getElemental_b (ex, ey, int_vars, write_int_vars, be);

	for (int i=0; i<npe*dim; i++)
	  b[index[i]] += be[i]; // assembly

      } 
    }

    // boundary conditions
    // y = 0
    for (int i=0; i<nx; i++)
      for (int d=0; d<dim; d++)
	b[i*dim + d] = 0.0;

    // y = ly
    for (int i=0; i<nx; i++)
      for (int d=0; d<dim; d++)
	b[(i+(ny-1)*nx)*dim + d] = 0.0;

    // x = 0
    for (int i=0; i<ny-2; i++)
      for (int d=0; d<dim; d++)
	b[(i+1)*nx*dim + d] = 0.0;

    // x = lx
    for (int i=0; i<ny-2; i++)
      for (int d=0; d<dim; d++)
	b[((i+2)*nx-1)*dim + d] = 0.0;

  } else if (dim == 3) {

    double be[3*8];
    for (int ex=0; ex<nx-1; ex++) {
      for (int ey=0; ey<ny-1; ey++) {
	for (int ez=0; ez<nz-1; ez++) {

	  n0 = ez*(nx*ny) + ey*nx     + ex;
	  n1 = ez*(nx*ny) + ey*nx     + ex + 1;
	  n2 = ez*(nx*ny) + (ey+1)*nx + ex + 1;
	  n3 = ez*(nx*ny) + (ey+1)*nx + ex;
	  n4 = n0 + nx*ny;
	  n5 = n1 + nx*ny;
	  n6 = n2 + nx*ny;
	  n7 = n3 + nx*ny;

	  for (int d=0; d<dim; d++) {
	    index[0*dim + d] = n0*dim + d;
	    index[1*dim + d] = n1*dim + d;
	    index[2*dim + d] = n2*dim + d;
	    index[3*dim + d] = n3*dim + d;
	    index[4*dim + d] = n4*dim + d;
	    index[5*dim + d] = n5*dim + d;
	    index[6*dim + d] = n6*dim + d;
	    index[7*dim + d] = n7*dim + d;
	  }

	  getElemental_b (ex, ey, ez, int_vars, write_int_vars, be);

	  for (int i=0; i<npe*dim; i++)
	    b[index[i]] += be[i];

	}
      }
    }

    // boundary conditions
    // z=0
    for (int i=0; i<nx; i++) {
      for (int j=0; j<ny; j++) {
	int n = nod_index(i,j,0);
	for (int d=0; d<dim; d++)
	  b[n*dim+d] = 0.0;
      }
    }
    // z = lx
    for (int i=0; i<nx; i++) {
      for (int j=0; j<ny; j++) {
	int n = nod_index(i,j,nz-1);
	for (int d=0; d<dim; d++)
	  b[n*dim+d] = 0.0;
      }
    }
    // y = 0
    for (int i=0; i<nx; i++) {
      for (int k=1; k<nz-1; k++) {
	int n = nod_index(i,0,k);
	for (int d=0; d<dim; d++)
	  b[n*dim+d] = 0.0;
      }
    }
    // y = ly
    for (int i=0; i<nx; i++) {
      for (int k=1; k<nz-1; k++) {
	int n = nod_index(i,ny-1,k);
	for (int d=0; d<dim; d++)
	  b[n*dim+d] = 0.0;
      }
    }
    // x=0
    for (int j=1; j<ny-1; j++) {
      for (int k=1; k<nz-1; k++) {
	int n = nod_index(0,j,k);
	for (int d=0; d<dim; d++)
	  b[n*dim+d] = 0.0;
      }
    }
    // x=lx
    for (int j=1; j<ny-1; j++) {
      for (int k=1; k<nz-1; k++) {
	int n = nod_index(nx-1,j,k);
	for (int d=0; d<dim; d++)
	  b[n*dim+d] = 0.0;
      }
    }

  }

  for (int i=0 ; i<nn*dim; i++)
    b[i] = -b[i];

  double norm = 0.0;
  for (int i=0; i<nn*dim; i++)
    norm += b[i]*b[i];
  norm = sqrt(norm);

  return norm;
}

void Problem::Assembly_A (void)
{
  int index[8];

  ell_set_zero_mat(&A);

  if (dim == 2) {

    double Ae[2*4*2*4];
    for (int ex=0; ex<nx-1; ex++) {
      for (int ey=0; ey<ny-1; ey++) {
	getElemental_A (ex, ey, Ae);
	ell_add_struct (A, ex, ey, Ae, dim, nx, ny);
      }
    }
    ell_set_bc_2D (A, dim, nx, ny);

  } else if (dim == 3) {

    double Ae[3*8*3*8];
    for (int ex=0; ex<nx-1; ex++) {
      for (int ey=0; ey<ny-1; ey++) {
	for (int ez=0; ez<nz-1; ez++) {
	  getElemental_A (ex, ey, ez, Ae);
	  ell_add_struct (A, ex, ey, ez, Ae, dim, nx, ny, nz);
	}
      }
    }
    ell_set_bc_3D (A, dim, nx, ny, nz);
  }
  //  ell_print (&A);
}

void Problem::getElemental_A (int ex, int ey, double (&Ae)[2*4*2*4])
{
  
  double nu, E;
  double ctan[3][3];
  bool plasticity;

  int e = glo_elem3D(ex,ey,0);

  material_t material;
  getMaterial(e, material);

  E  = material.E;
  nu = material.nu;
  plasticity = material.plasticity;

  ctan[0][0]=(1-nu); ctan[0][1]=nu    ; ctan[0][2]=0;
  ctan[1][0]=nu    ; ctan[1][1]=(1-nu); ctan[1][2]=0;
  ctan[2][0]=0     ; ctan[2][1]=0     ; ctan[2][2]=(1-2*nu)/2;

  for (int i=0; i<nvoi; i++)
    for (int j=0; j<nvoi; j++)
      ctan[i][j] *= E/((1+nu)*(1-2*nu));

  double xg[4][2] = {
    {-0.577350269189626, -0.577350269189626},
    {+0.577350269189626, -0.577350269189626},
    {+0.577350269189626, +0.577350269189626},
    {-0.577350269189626, +0.577350269189626}};

  double dsh[4][2], b_mat[3][8], cxb[3][8];

  for (int i=0; i<npe*dim*npe*dim; i++) 
    Ae[i] = 0.0;

  for (int gp=0; gp<4; gp++) {

    dsh[0][0] = -(1-xg[gp][1])/4*2/dx ; dsh[0][1] = -(1-xg[gp][0])/4*2/dy;
    dsh[1][0] = +(1-xg[gp][1])/4*2/dx ; dsh[1][1] = -(1+xg[gp][0])/4*2/dy;
    dsh[2][0] = +(1+xg[gp][1])/4*2/dx ; dsh[2][1] = +(1+xg[gp][0])/4*2/dy;
    dsh[3][0] = -(1+xg[gp][1])/4*2/dx ; dsh[3][1] = +(1-xg[gp][0])/4*2/dy;

    for (int i=0; i<4; i++) {
      b_mat[0][i*dim] = dsh[i][0]; b_mat[0][i*dim + 1] = 0        ;
      b_mat[1][i*dim] = 0        ; b_mat[1][i*dim + 1] = dsh[i][1];
      b_mat[2][i*dim] = dsh[i][1]; b_mat[2][i*dim + 1] = dsh[i][0];
    }

    for (int i=0; i<nvoi; i++) {
      for (int j=0; j<npe*dim; j++) {
	cxb[i][j] = 0.0;
	for (int k=0; k<nvoi; k++)
	  cxb[i][j] += ctan[i][k] * b_mat[k][j];
      }
    }

    double wg = 0.25*dx*dy;
    for (int i=0; i<npe*dim; i++)
      for (int j=0; j<npe*dim; j++)
	for (int m=0; m<nvoi; m++)
	  Ae[i*npe*dim + j] += b_mat[m][i] * cxb[m][j] * wg;

  } // gp loop
}

void Problem::getElemental_A (int ex, int ey, int ez, double (&Ae)[3*8*3*8])
{
  double nu, E;
  double ctan[6][6];
  bool plasticity;

  int e = glo_elem3D(ex,ey,ez);

  material_t material;
  getMaterial(e, material);

  E  = material.E;
  nu = material.nu;
  plasticity = material.plasticity;

  ctan[0][0]=(1-nu); ctan[0][1]=nu    ; ctan[0][2]=nu    ; ctan[0][3]=0         ; ctan[0][4]=0         ; ctan[0][5]=0         ;
  ctan[1][0]=nu    ; ctan[1][1]=(1-nu); ctan[1][2]=nu    ; ctan[1][3]=0         ; ctan[1][4]=0         ; ctan[1][5]=0         ;
  ctan[2][0]=nu    ; ctan[2][1]=nu    ; ctan[2][2]=(1-nu); ctan[2][3]=0         ; ctan[2][4]=0         ; ctan[2][5]=0         ;
  ctan[3][0]=0     ; ctan[3][1]=0     ; ctan[3][2]=0     ; ctan[3][3]=(1-2*nu)/2; ctan[3][4]=0         ; ctan[3][5]=0         ;
  ctan[4][0]=0     ; ctan[4][1]=0     ; ctan[4][2]=0     ; ctan[4][3]=0         ; ctan[4][4]=(1-2*nu)/2; ctan[4][5]=0         ;
  ctan[5][0]=0     ; ctan[5][1]=0     ; ctan[5][2]=0     ; ctan[5][3]=0         ; ctan[5][4]=0         ; ctan[5][5]=(1-2*nu)/2;

  for (int i=0; i<nvoi; i++)
    for (int j=0; j<nvoi; j++)
      ctan[i][j] *= E/((1+nu)*(1-2*nu));

  double bmat[6][3*8], cxb[6][3*8];

  for (int i=0; i<npe*dim*npe*dim; i++) 
    Ae[i] = 0.0;

  for (int gp=0; gp<8; gp++) {

    calc_bmat_3D (gp, bmat);

    for (int i=0; i<nvoi; i++) {
      for (int j=0; j<npe*dim; j++) {
	cxb[i][j] = 0.0;
	for (int k=0; k<nvoi; k++)
	  cxb[i][j] += ctan[i][k] * bmat[k][j];
      }
    }

    double wg = (1/8.0)*dx*dy*dz;
    for (int i=0; i<npe*dim; i++)
      for (int j=0; j<npe*dim; j++)
	for (int m=0; m<nvoi; m++)
	  Ae[i*npe*dim + j] += bmat[m][i] * cxb[m][j] * wg;

  } // gp loop
}

void Problem::calc_bmat_3D (int gp, double bmat[6][3*8]) {

  double xg[8][3] = {
    {-0.577350269189626, -0.577350269189626, -0.577350269189626},
    {+0.577350269189626, -0.577350269189626, -0.577350269189626},
    {+0.577350269189626, +0.577350269189626, -0.577350269189626},
    {-0.577350269189626, +0.577350269189626, -0.577350269189626},
    {-0.577350269189626, -0.577350269189626, +0.577350269189626},
    {+0.577350269189626, -0.577350269189626, +0.577350269189626},
    {+0.577350269189626, +0.577350269189626, +0.577350269189626},
    {-0.577350269189626, +0.577350269189626, +0.577350269189626}};

  double dsh[8][3];

  dsh[0][0]= -(1-xg[gp][1])*(1-xg[gp][2])/8*2/dx;  dsh[0][1]= -(1-xg[gp][0])*(1-xg[gp][2])/8*2/dy;  dsh[0][2]= -(1-xg[gp][0])*(1-xg[gp][1])/8*2/dz;
  dsh[1][0]= +(1-xg[gp][1])*(1-xg[gp][2])/8*2/dx;  dsh[1][1]= -(1+xg[gp][0])*(1-xg[gp][2])/8*2/dy;  dsh[1][2]= -(1+xg[gp][0])*(1-xg[gp][1])/8*2/dz;
  dsh[2][0]= +(1+xg[gp][1])*(1-xg[gp][2])/8*2/dx;  dsh[2][1]= +(1+xg[gp][0])*(1-xg[gp][2])/8*2/dy;  dsh[2][2]= -(1+xg[gp][0])*(1+xg[gp][1])/8*2/dz;
  dsh[3][0]= -(1+xg[gp][1])*(1-xg[gp][2])/8*2/dx;  dsh[3][1]= +(1-xg[gp][0])*(1-xg[gp][2])/8*2/dy;  dsh[3][2]= -(1-xg[gp][0])*(1+xg[gp][1])/8*2/dz;
  dsh[4][0]= -(1-xg[gp][1])*(1+xg[gp][2])/8*2/dx;  dsh[4][1]= -(1-xg[gp][0])*(1+xg[gp][2])/8*2/dy;  dsh[4][2]= +(1-xg[gp][0])*(1-xg[gp][1])/8*2/dz;
  dsh[5][0]= +(1-xg[gp][1])*(1+xg[gp][2])/8*2/dx;  dsh[5][1]= -(1+xg[gp][0])*(1+xg[gp][2])/8*2/dy;  dsh[5][2]= +(1+xg[gp][0])*(1-xg[gp][1])/8*2/dz;
  dsh[6][0]= +(1+xg[gp][1])*(1+xg[gp][2])/8*2/dx;  dsh[6][1]= +(1+xg[gp][0])*(1+xg[gp][2])/8*2/dy;  dsh[6][2]= +(1+xg[gp][0])*(1+xg[gp][1])/8*2/dz;
  dsh[7][0]= -(1+xg[gp][1])*(1+xg[gp][2])/8*2/dx;  dsh[7][1]= +(1-xg[gp][0])*(1+xg[gp][2])/8*2/dy;  dsh[7][2]= +(1-xg[gp][0])*(1+xg[gp][1])/8*2/dz;

  for (int i=0; i<8; i++) {
    bmat[0][i*dim] = dsh[i][0]; bmat[0][i*dim+1] = 0        ; bmat[0][i*dim+2] = 0        ;
    bmat[1][i*dim] = 0        ; bmat[1][i*dim+1] = dsh[i][1]; bmat[1][i*dim+2] = 0        ;
    bmat[2][i*dim] = 0        ; bmat[2][i*dim+1] = 0        ; bmat[2][i*dim+2] = dsh[i][2];
    bmat[3][i*dim] = dsh[i][1]; bmat[3][i*dim+1] = dsh[i][0]; bmat[3][i*dim+2] = 0        ;
    bmat[4][i*dim] = 0        ; bmat[4][i*dim+1] = dsh[i][2]; bmat[4][i*dim+2] = dsh[i][1];
    bmat[5][i*dim] = dsh[i][2]; bmat[5][i*dim+1] = 0        ; bmat[5][i*dim+2] = dsh[i][0];
  }

}

void Problem::getElemental_b (int ex, int ey, double **int_vars, bool write_int_vars, double (&be)[2*4])
{
  double dsh[4][2], bmat[3][2*4], cxb[3][8], stress_gp[6];
  double xg[4][2] = {
    {-0.577350269189626, -0.577350269189626},
    {+0.577350269189626, -0.577350269189626},
    {+0.577350269189626, +0.577350269189626},
    {-0.577350269189626, +0.577350269189626}};

  for (int i=0; i<2*4; i++) 
    be[i] = 0.0;

  for (int gp=0; gp<4; gp++) {

    dsh[0][0] = -(1-xg[gp][1])/4*2/dx ; dsh[0][1] = -(1-xg[gp][0])/4*2/dy;
    dsh[1][0] = +(1-xg[gp][1])/4*2/dx ; dsh[1][1] = -(1+xg[gp][0])/4*2/dy;
    dsh[2][0] = +(1+xg[gp][1])/4*2/dx ; dsh[2][1] = +(1+xg[gp][0])/4*2/dy;
    dsh[3][0] = -(1+xg[gp][1])/4*2/dx ; dsh[3][1] = +(1-xg[gp][0])/4*2/dy;

    for (int i=0; i<4; i++) {
      bmat[0][i*dim] = dsh[i][0]; bmat[0][i*dim+1] = 0        ;
      bmat[1][i*dim] = 0        ; bmat[1][i*dim+1] = dsh[i][1];
      bmat[2][i*dim] = dsh[i][1]; bmat[2][i*dim+1] = dsh[i][0];
    }

    getStress (ex, ey, gp, int_vars, write_int_vars, stress_gp);

    double wg = 0.25*dx*dy;
    for (int i=0; i<npe*dim; i++) {
      for (int j=0; j<nvoi; j++) {
	be[i] += bmat[j][i] * stress_gp[j] * wg;
      }
    }

  } // gp loop
}

void Problem::getElemental_b (int ex, int ey, int ez, double **int_vars, bool write_int_vars, double (&be)[3*8])
{
  double bmat[6][3*8], cxb[6][3*8], stress_gp[6];

  for (int i=0; i<3*8; i++) 
    be[i] = 0.0;

  for (int gp=0; gp<8; gp++) {

    calc_bmat_3D (gp, bmat);
    getStress (ex, ey, ez, gp, int_vars, write_int_vars, stress_gp);

    double wg = (1/8.0)*dx*dy*dz;
    for (int i=0; i<npe*dim; i++)
      for (int j=0; j<nvoi; j++)
	be[i] += bmat[j][i] * stress_gp[j] * wg;

  } // gp loop
}

void Problem::calcAverageStress (double *int_vars, double stress_ave[6])
{
  for (int v=0; v<nvoi; v++)
    stress_ave[v] = 0.0;

  if (dim == 2) {

    for (int ex=0; ex<nx-1; ex++) {
      for (int ey=0; ey<ny-1; ey++) {

	double stress_aux[3];
	for (int i=0; i<nvoi; i++)
	  stress_aux[i] = 0.0;

	for (int gp=0; gp<4; gp++) {

	  double stress_gp[6];
	  double wg = 0.25*dx*dy;

	  bool write_int_vars = false;
	  getStress (ex, ey, gp, &int_vars, write_int_vars, stress_gp);
	  for (int v=0; v<nvoi; v++)
	    stress_aux[v] += stress_gp[v] * wg;

	}
	for (int v=0; v<nvoi; v++)
	  stress_ave[v] += stress_aux[v];
      }
    }

  } else if (dim == 3) {

    for (int ex=0; ex<nx-1; ex++) {
      for (int ey=0; ey<ny-1; ey++) {
	for (int ez=0; ez<nz-1; ez++) {

	  double stress_aux[6];
	  for (int i=0; i<nvoi; i++)
	    stress_aux[i] = 0.0;

	  for (int gp=0; gp<8; gp++) {

	    double stress_gp[6];
	    double wg = (1/8.0)*dx*dy*dz;

	    bool write_int_vars = false;
	    getStress (ex, ey, ez, gp, &int_vars, write_int_vars, stress_gp);
	    for (int v=0; v<nvoi; v++)
	      stress_aux[v] += stress_gp[v] * wg;

	  }
	  for (int v=0; v<nvoi; v++)
	    stress_ave[v] += stress_aux[v];
	}
      }
    }
  }

  for (int v=0; v<nvoi; v++)
    stress_ave[v] /= (lx*ly);
}

void Problem::calcAverageStrain (double strain_ave[6])
{
  for (int v=0; v<nvoi; v++)
    strain_ave[v] = 0.0;

  if (dim == 2) {

    for (int ex=0; ex<nx-1; ex++) {
      for (int ey=0; ey<ny-1; ey++) {

	double strain_aux[3];
	for (int i=0; i<nvoi; i++)
	  strain_aux[i] = 0.0;

	for (int gp=0; gp<4; gp++) {
	  double strain_gp[6];
	  double wg = 0.25*dx*dy;
	  getStrain (ex, ey, gp, strain_gp);
	  for (int v=0; v<nvoi; v++)
	    strain_aux[v] += strain_gp[v] * wg;
	}

	for (int v=0; v<nvoi; v++)
	  strain_ave[v] += strain_aux[v];
      }
    }


  } else if (dim == 3) {

    for (int ex=0; ex<nx-1; ex++) {
      for (int ey=0; ey<ny-1; ey++) {
	for (int ez=0; ez<nz-1; ez++) {

	  double strain_aux[6];
	  for (int i=0; i<nvoi; i++)
	    strain_aux[i] = 0.0;

	  for (int gp=0; gp<8; gp++) {
	    double strain_gp[6];
	    double wg = (1/8.0)*dx*dy*dz;
	    getStrain (ex, ey, ez, gp, strain_gp);
	    for (int v=0; v<nvoi; v++)
	      strain_aux[v] += strain_gp[v] * wg;
	  }

	  for (int v=0; v<nvoi; v++)
	    strain_ave[v] += strain_aux[v];
	}
      }
    }
  }

  for (int v=0; v<nvoi; v++)
    strain_ave[v] /= (lx*ly);
}

void Problem::calcDistributions (double *int_vars)
{
  if (dim == 2) {

    for (int ex=0; ex<nx-1; ex++) {
      for (int ey=0; ey<ny-1; ey++) {

	double strain_aux[3];
	double stress_aux[3];
	for (int i=0; i<nvoi; i++) {
	  strain_aux[i] = 0.0;
	  stress_aux[i] = 0.0;
	}

	for (int gp=0; gp<4; gp++) {

	  double stress_gp[3], strain_gp[3];
	  double wg = 0.25*dx*dy;

	  bool write_int_vars = false;
	  getStress (ex, ey, gp, &int_vars, write_int_vars, stress_gp);
	  getStrain (ex, ey, gp, strain_gp);
	  for (int v=0; v<nvoi; v++) {
	    strain_aux[v] += strain_gp[v] * wg;
	    stress_aux[v] += stress_gp[v] * wg;
	  }

	}
	double vol = dx*dy;
	int e = glo_elem3D(ex,ey,0);
	for (int v=0; v<nvoi; v++) {
	  strain[e*nvoi + v] = strain_aux[v] / vol;
	  stress[e*nvoi + v] = stress_aux[v] / vol;
	}
      }
    }

  } else if (dim == 3) {

    for (int ex=0; ex<nx-1; ex++) {
      for (int ey=0; ey<ny-1; ey++) {
	for (int ez=0; ez<nz-1; ez++) {

	  double strain_aux[6];
	  double stress_aux[6];
	  for (int i=0; i<nvoi; i++) {
	    strain_aux[i] = 0.0;
	    stress_aux[i] = 0.0;
	  }

	  for (int gp=0; gp<8; gp++) {

	    double stress_gp[6], strain_gp[6];
	    double wg = (1/8.0)*dx*dy*dz;

	    bool write_int_vars = false;
	    getStress (ex, ey, ez, gp, &int_vars, write_int_vars, stress_gp);
	    getStrain (ex, ey, ez, gp, strain_gp);
	    for (int v=0; v<nvoi; v++) {
	      strain_aux[v] += strain_gp[v] * wg;
	      stress_aux[v] += stress_gp[v] * wg;
	    }

	  }

	  double vol = dx*dy*dz;
          int e = glo_elem3D(ex,ey,ez);
	  for (int v=0; v<nvoi; v++) {
	    strain[e*nvoi + v] = strain_aux[v] / vol;
	    stress[e*nvoi + v] = stress_aux[v] / vol;
	  }
	}
      }
    }

  }
}

void Problem::getStress (int ex, int ey, int gp, double **int_vars, bool write_int_vars, double *stress_gp)
{
  double strain_gp[3];
  getStrain (ex, ey, gp, strain_gp);

  double nu, E;
  double ctan[3][3];
  bool plasticity;

  int e = glo_elem3D(ex,ey,0);

  material_t material;
  getMaterial(e, material);

  E  = material.E;
  nu = material.nu;
  plasticity = material.plasticity;

  ctan[0][0]=(1-nu); ctan[0][1]=nu    ; ctan[0][2]=0;
  ctan[1][0]=nu    ; ctan[1][1]=(1-nu); ctan[1][2]=0;
  ctan[2][0]=0     ; ctan[2][1]=0     ; ctan[2][2]=(1-2*nu)/2;
  for (int i=0; i<nvoi; i++)
    for (int j=0; j<nvoi; j++)
      ctan[i][j] *= E/((1+nu)*(1-2*nu));

  for (int i=0; i<3; i++) {
    stress_gp[i] = 0.0;
    for (int j=0; j<3; j++) {
      stress_gp[i] += ctan[i][j] * strain_gp[j];
    }
  }

}

void Problem::getStress (int ex, int ey, int ez, int gp, double **int_vars, bool write_int_vars, double *stress_gp)
{
  double nu, E;
  double ctan[6][6];
  bool plasticity;

  int e = glo_elem3D(ex,ey,ez);

  material_t material;
  getMaterial(e, material);

  E  = material.E;
  nu = material.nu;
  plasticity = material.plasticity;

  double strain_gp[6];
  getStrain (ex, ey, ez, gp, strain_gp);

  if (plasticity == true) {

    double eps_dev[6];
    double eps_p_dev_1[6], eps_p_1[6], eps_p[6];
    double normal[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    double alpha_1, alpha, dl=0.0;
    double sig_dev_trial[6], sig_dev_trial_norm;

    if (*int_vars == NULL) {
      for (int i=0; i<6; i++)
	eps_p_1[i] = 0.0;
      alpha_1 = 0.0;
    } else {
      for (int i=0; i<6; i++)
	eps_p_1[i] = (*int_vars)[intvar_ix(e, gp, i)];
      alpha_1 = (*int_vars)[intvar_ix(e, gp, 6)];
    }

    for (int i=0; i<6; i++)
      eps_p_dev_1[i] = eps_p_1[i];
    for (int i=0; i<3; i++)
      eps_p_dev_1[i] -= (1/3.0) * (eps_p_1[0] + eps_p_1[1] + eps_p_1[2]);

    for (int i=0; i<6; i++)
      eps_dev[i] = strain_gp[i];
    for (int i=0; i<3; i++)
      eps_dev[i] -= (1/3.0) * (strain_gp[0] + strain_gp[1] + strain_gp[2]);

    for (int i=0; i<6; i++) 
      sig_dev_trial[i] = 2 * material.mu * (eps_dev[i] - eps_p_dev_1[i]);

    sig_dev_trial_norm = sqrt( \
	  sig_dev_trial[0]*sig_dev_trial[0] + \
	  sig_dev_trial[1]*sig_dev_trial[1] + \
	  sig_dev_trial[2]*sig_dev_trial[2] + \
	2*sig_dev_trial[3]*sig_dev_trial[3] + \
	2*sig_dev_trial[4]*sig_dev_trial[4] + \
	2*sig_dev_trial[5]*sig_dev_trial[5]);

    double f_trial = sig_dev_trial_norm - (material.Sy + material.K_alpha * alpha_1);

    if (f_trial > 0) {

      // cout << "LINEAR = NO" << endl;

      if (write_int_vars == true)
	if (*int_vars == NULL)
	  *int_vars = (double*)malloc(nelem * 8 * INT_VARS_GP * sizeof(double));

      for (int i=0; i<6; i++)
	normal[i] = sig_dev_trial[i] / sig_dev_trial_norm ;

      // cout << "sig_dev_trial_norm = " << sig_dev_trial_norm << endl;

      int its = 0;
      double g, dg;
      alpha = alpha_1;

      do {
	g   = sig_dev_trial_norm  - sqrt(2.0/3) * (material.Sy + material.K_alpha * alpha) - 2 * material.mu * dl;
	dg  = - 2*material.mu;
	dl -= g/dg;
	alpha += sqrt(2.0/3) * dl;
	its ++;
	// cout << "g = " << g << endl;
      } while (fabs(g)>1.0 && its<4);

      for (int i=0; i<6; i++)
	eps_p[i] = eps_p_1[i] + dl*normal[i];

    }
    else {

      // cout << "LINEAR = YES" << endl;

      for (int i=0; i<6; i++)
	eps_p[i] = eps_p_1[i];
      alpha = alpha_1;
    }

    //sig_2   = K * tr(eps) * 1 + s_trial - 2*mu*dl*normal;
    for (int i=0; i<6; i++)
      stress_gp[i] = sig_dev_trial[i];
    for (int i=0; i<3; i++)
      stress_gp[i] += material.k * (strain_gp[0] + strain_gp[1] + strain_gp[2]);
    for (int i=0; i<6; i++)
      stress_gp[i] -= 2 * material.mu * dl * normal[i];

    if (write_int_vars == true)
      if (*int_vars != NULL) {
	for (int i=0; i<6; i++)
	  (*int_vars)[intvar_ix(e, gp, i)] = eps_p[i];
	(*int_vars)[intvar_ix(e, gp, 6)] = alpha;
      }

  } else {

    ctan[0][0]=(1-nu); ctan[0][1]=nu    ; ctan[0][2]=nu      ; ctan[0][3]=0         ; ctan[0][4]=0         ; ctan[0][5]=0         ;
    ctan[1][0]=nu    ; ctan[1][1]=(1-nu); ctan[1][2]=nu      ; ctan[1][3]=0         ; ctan[1][4]=0         ; ctan[1][5]=0         ;
    ctan[2][0]=nu    ; ctan[2][1]=nu    ; ctan[2][2]=(1-nu)  ; ctan[2][3]=0         ; ctan[2][4]=0         ; ctan[2][5]=0         ;
    ctan[3][0]=0     ; ctan[3][1]=0     ; ctan[3][2]=0       ; ctan[3][3]=(1-2*nu)/2; ctan[3][4]=0         ; ctan[3][5]=0         ;
    ctan[4][0]=0     ; ctan[4][1]=0     ; ctan[4][2]=0       ; ctan[4][3]=0         ; ctan[4][4]=(1-2*nu)/2; ctan[4][5]=0         ;
    ctan[5][0]=0     ; ctan[5][1]=0     ; ctan[5][2]=0       ; ctan[5][3]=0         ; ctan[5][4]=0         ; ctan[5][5]=(1-2*nu)/2;

    for (int i=0; i<6; i++)
      for (int j=0; j<6; j++)
	ctan[i][j] *= E/((1+nu)*(1-2*nu));

    for (int i=0; i<6; i++) {
      stress_gp[i] = 0.0;
      for (int j=0; j<6; j++) {
	stress_gp[i] += ctan[i][j] * strain_gp[j];
      }
    }
  }

}

void Problem::getMaterial (int e, material_t &material)
{
  int mat_num;
  if (micro_type == 0) {
    if (elem_type[e] == 0) {
      mat_num = 0;
    } else {
      mat_num = 1;
    }
  } else if (micro_type == 1) {
    if (elem_type[e] == 0) {
      mat_num = 0;
    } else {
      mat_num = 1;
    }
  }
  material.E          = material_list[mat_num].E;
  material.nu         = material_list[mat_num].nu;
  material.Sy         = material_list[mat_num].Sy;
  material.K_alpha    = material_list[mat_num].K_alpha;
  material.mu         = material_list[mat_num].mu;
  material.k          = material_list[mat_num].k;
  material.plasticity = material_list[mat_num].plasticity;
}

void Problem::getStrain (int ex, int ey, int gp, double *strain_gp)
{
  double elem_disp[2*4];
  getElemDisp (ex, ey, elem_disp);

  double xg[4][2] = {
    {-0.577350269189626, -0.577350269189626},
    {+0.577350269189626, -0.577350269189626},
    {+0.577350269189626, +0.577350269189626},
    {-0.577350269189626, +0.577350269189626}};

  double dsh[4][2], bmat[3][8], cxb[3][8];

  dsh[0][0] = -(1-xg[gp][1])/4*2/dx ; dsh[0][1] = -(1-xg[gp][0])/4*2/dy;
  dsh[1][0] = +(1-xg[gp][1])/4*2/dx ; dsh[1][1] = -(1+xg[gp][0])/4*2/dy;
  dsh[2][0] = +(1+xg[gp][1])/4*2/dx ; dsh[2][1] = +(1+xg[gp][0])/4*2/dy;
  dsh[3][0] = -(1+xg[gp][1])/4*2/dx ; dsh[3][1] = +(1-xg[gp][0])/4*2/dy;

  for (int i=0; i<4; i++) {
    bmat[0][i*dim] = dsh[i][0]; bmat[0][i*dim + 1] = 0        ;
    bmat[1][i*dim] = 0        ; bmat[1][i*dim + 1] = dsh[i][1];
    bmat[2][i*dim] = dsh[i][1]; bmat[2][i*dim + 1] = dsh[i][0];
  }
  for (int v=0; v<nvoi; v++) {
    strain_gp[v] = 0.0;
    for (int i=0; i<npe*dim; i++) {
      strain_gp[v] += bmat[v][i] * elem_disp[i];
    }
  }

}

void Problem::getStrain (int ex, int ey, int ez, int gp, double *strain_gp)
{

  double elem_disp[3*8];
  getElemDisp (ex, ey, ez, elem_disp);

  double bmat[6][3*8];
  calc_bmat_3D (gp, bmat);

  for (int v=0; v<nvoi; v++) {
    strain_gp[v] = 0.0;
    for (int i=0; i<npe*dim; i++)
      strain_gp[v] += bmat[v][i] * elem_disp[i];
  }

}

void Problem::getElemDisp (int ex, int ey, double *elem_disp)
{
  int n0 = ey * nx     + ex;
  int n1 = ey * nx     + ex + 1;
  int n2 = (ey+1) * nx + ex + 1;
  int n3 = (ey+1) * nx + ex;

  for (int d=0; d<dim; d++) {
    elem_disp[0*dim + d] = u[n0*dim + d];
    elem_disp[1*dim + d] = u[n1*dim + d];
    elem_disp[2*dim + d] = u[n2*dim + d];
    elem_disp[3*dim + d] = u[n3*dim + d];
  }
}

void Problem::getElemDisp (int ex, int ey, int ez, double *elem_disp)
{
  int n0 = ez*(nx*ny) + ey*nx     + ex;
  int n1 = ez*(nx*ny) + ey*nx     + ex + 1;
  int n2 = ez*(nx*ny) + (ey+1)*nx + ex + 1;
  int n3 = ez*(nx*ny) + (ey+1)*nx + ex;
  int n4 = n0 + nx*ny;
  int n5 = n1 + nx*ny;
  int n6 = n2 + nx*ny;
  int n7 = n3 + nx*ny;

  for (int d=0; d<dim; d++) {
    elem_disp[0*dim + d] = u[n0*dim + d];
    elem_disp[1*dim + d] = u[n1*dim + d];
    elem_disp[2*dim + d] = u[n2*dim + d];
    elem_disp[3*dim + d] = u[n3*dim + d];
    elem_disp[4*dim + d] = u[n4*dim + d];
    elem_disp[5*dim + d] = u[n5*dim + d];
    elem_disp[6*dim + d] = u[n6*dim + d];
    elem_disp[7*dim + d] = u[n7*dim + d];
  }
}
