#include <cmath>
#include "micro.h"

//double assembly_b (double *b, Problem &problem)
//{
//  int npe = problem.npe;
//  int dim = problem.dim;
//  int nx = problem.nx;
//  int ny = problem.ny;
//  int nn = problem.nn;
//  int nelem = problem.nelem;
//  int index[8];
//  double be[8];
//
//  for (int i=0 ; i<nn*dim; i++) {
//    b[i] = 0.0;
//  }
//
//  for (int e=0; e<nelem; e++) {
//
//    int xfactor = e%(nx-1);
//    int yfactor = e/(ny-1);
//
//    int n0 = yfactor     * nx + xfactor     ;
//    int n1 = yfactor     * nx + xfactor + 1 ;
//    int n2 = (yfactor+1) * nx + xfactor + 1 ;
//    int n3 = (yfactor+1) * nx + xfactor     ;
//    index[0] = n0*dim; index[1] = n0*dim + 1;
//    index[2] = n1*dim; index[3] = n1*dim + 1;
//    index[4] = n2*dim; index[5] = n2*dim + 1;
//    index[6] = n3*dim; index[7] = n3*dim + 1;
//    //cout << "e : n0="<<n0<<" n1="<<n1<<" n2="<<n2<<" n3="<<n3<<endl;
//    
//    get_elemental_b(e, be, problem);
//         
////    for (int gp = 0 ; gp < ngp ; gp++) { // integration res_e
////      get_strain(e, gp, strain_gp);
////      get_stress(e, gp, strain_gp, stress_gp);
////      for (int i = 0 ; i < (npe*dim) ; i++)
////	for (int j = 0 ; j < nvoi ; j++)
////	  res_e[i] += struct_bmat[j][i][gp] * stress_gp[j] * struct_wp[gp];
////    }
//    for (int i=0 ; i<npe*dim; i++) {
//      b[index[i]] += be[i]; // assembly
//    }
//  }
//
////  for (int d = 0; d < dim ; d++) {
////    for (int n = 0 ; n < (mesh_struct.ny - 2) ; n++) {
////      b[problem.nods_x0[n]*dim + d] = 0.0;
////      b[problem.nods_x1[n]*dim + d] = 0.0;
////    }
////    for (int n = 0 ; n < (problem.nx - 2) ; n++) {
////      b[problem.nods_y0[n]*dim + d] = 0.0;
////      b[problem.nods_y1[n]*dim + d] = 0.0;
////    }
////    b[problem.nod_x0y0*dim + d] = 0.0;
////    b[problem.nod_x1y0*dim + d] = 0.0;
////    b[problem.nod_x1y1*dim + d] = 0.0;
////    b[problem.nod_x0y1*dim + d] = 0.0;
////  }
//
//  double norm = 0.0;
//  for (int i=0; i<nn*dim; i++) {
//    norm += b[i]*b[i];
//  }
//  norm = sqrt(norm);
//
//  return norm;
//}

void Problem::assembly_A(void)
{
  int index[8];
  double Ae[8][8], Ae_vec[64];

  ell_set_zero_mat(&A_ell);
  for (int e = 0 ; e < nelem ; e++) {

    int xfactor = e%(nx-1);
    int yfactor = e/(ny-1);

    int n0 = yfactor     * nx + xfactor     ;
    int n1 = yfactor     * nx + xfactor + 1 ;
    int n2 = (yfactor+1) * nx + xfactor + 1 ;
    int n3 = (yfactor+1) * nx + xfactor     ;
    index[0] = n0*dim; index[1] = n0*dim + 1;
    index[2] = n1*dim; index[3] = n1*dim + 1;
    index[4] = n2*dim; index[5] = n2*dim + 1;
    index[6] = n3*dim; index[7] = n3*dim + 1;
    //cout << "e : n0="<<n0<<" n1="<<n1<<" n2="<<n2<<" n3="<<n3<<endl;

    get_elemental_A (e, Ae);
    for (int i=0; i<npe*dim; i++)
      for (int j=0; j<npe*dim; j++)
	Ae_vec[i*8+j] = Ae[i][j];

    ell_add_vals(&A_ell, index, npe*dim, index, npe*dim, Ae_vec); // assembly
  }

//  for (int d = 0; d < dim ; d++) {
//    for (int n = 0; n < mesh_struct.ny - 2 ; n++) {
//      ell_set_zero_row (&jac_ell, mesh_struct.nods_x0[n]*dim + d, 1.0);
//      ell_set_zero_col (&jac_ell, mesh_struct.nods_x0[n]*dim + d, 1.0);
//      ell_set_zero_row (&jac_ell, mesh_struct.nods_x1[n]*dim + d, 1.0);
//      ell_set_zero_col (&jac_ell, mesh_struct.nods_x1[n]*dim + d, 1.0);
//    }
//    for (int n = 0; n < mesh_struct.nx - 2 ; n++) {
//      ell_set_zero_row (&jac_ell, mesh_struct.nods_y0[n]*dim + d, 1.0);
//      ell_set_zero_col (&jac_ell, mesh_struct.nods_y0[n]*dim + d, 1.0);
//      ell_set_zero_row (&jac_ell, mesh_struct.nods_y1[n]*dim + d, 1.0);
//      ell_set_zero_col (&jac_ell, mesh_struct.nods_y1[n]*dim + d, 1.0);
//    }
//    ell_set_zero_row (&jac_ell, mesh_struct.nod_x0y0*dim + d, 1.0);
//    ell_set_zero_col (&jac_ell, mesh_struct.nod_x0y0*dim + d, 1.0);
//    ell_set_zero_row (&jac_ell, mesh_struct.nod_x1y0*dim + d, 1.0);
//    ell_set_zero_col (&jac_ell, mesh_struct.nod_x1y0*dim + d, 1.0);
//    ell_set_zero_row (&jac_ell, mesh_struct.nod_x1y1*dim + d, 1.0);
//    ell_set_zero_col (&jac_ell, mesh_struct.nod_x1y1*dim + d, 1.0);
//    ell_set_zero_row (&jac_ell, mesh_struct.nod_x0y1*dim + d, 1.0);
//    ell_set_zero_col (&jac_ell, mesh_struct.nod_x0y1*dim + d, 1.0);
//  }

}

void Problem::get_elemental_A (int e, double (&Ae)[8][8])
{
/*   // for debugging
     for (int i=0; i<8; i++)
     for (int j=0; j<8; j++)
     Ae[i][j] = 1;
 */
  
  double nu = 0.3, E;
  double ctan[3][3];
  E  = 1e7;
  if (distance(e) < -1.75) {
    E  = 1.0e7;
  }
  else {
     E  = 1.0e6;
  }
  ctan[0][0]=(1-nu); ctan[0][1]=nu    ; ctan[0][2]=0;
  ctan[1][0]=nu    ; ctan[1][1]=(1-nu); ctan[1][2]=0;
  ctan[2][0]=0     ; ctan[2][1]=0     ; ctan[2][2]=(1-2*nu)/2;
  for (int i=0; i<3; i++) {
    for (int j=0; j<3; j++) {
      ctan[i][j] *= E/((1+nu)*(1-2*nu));
    }
  }

  double xg[4][2] = {
    {-0.577350269189626, -0.577350269189626},
    {+0.577350269189626, -0.577350269189626},
    {+0.577350269189626, +0.577350269189626},
    {-0.577350269189626, +0.577350269189626}};

  double dsh[4][2], b_mat[3][8], cxb[3][8];

  for (int i=0; i<8; i++) 
    for (int j=0; j<8; j++) 
      Ae[i][j] = 0.0;

  for (int gp=0; gp<4; gp++) {

    dsh[0][0] = -(1-xg[gp][1])/4*2/dx ; dsh[0][1] = -(1-xg[gp][0])/4*2/dy;
    dsh[1][0] = +(1-xg[gp][1])/4*2/dx ; dsh[1][1] = -(1+xg[gp][0])/4*2/dy;
    dsh[2][0] = +(1+xg[gp][1])/4*2/dx ; dsh[2][1] = +(1+xg[gp][0])/4*2/dy;
    dsh[3][0] = -(1+xg[gp][1])/4*2/dx ; dsh[3][1] = +(1-xg[gp][0])/4*2/dy;

    for (int i=0; i<4; i++) {
      b_mat[1][i*dim] = dsh[i][0]; b_mat[1][i*dim + 1] = 0        ;
      b_mat[2][i*dim] = 0        ; b_mat[2][i*dim + 1] = dsh[i][1];
      b_mat[3][i*dim] = dsh[i][1]; b_mat[3][i*dim + 1] = dsh[i][0];
    }

    for (int i=0; i<3; i++) {
      for (int j=0; j<8; j++) {
	for (int k=0; k<3; k++) {
	  cxb[i][j] += ctan[i][k] * b_mat[k][j];
	}
      }
    }

    for (int i=0; i<8; i++) {
      for (int j=0; j<8; j++) {
	for (int m=0; m<3; m++) {
	    Ae[i][j] += b_mat[m][i] * cxb[m][j] * wg[gp];
	}
      }
    }
  } // gp loop
}

double Problem::distance (int e)
{
  int xfactor = e%(nx-1);
  int yfactor = e/(ny-1);
  double xdis = pow(xfactor*dx + dx/2 - lx/2,2);
  double ydis = pow(yfactor*dy + dy/2 - ly/2,2);
  return sqrt(xdis + ydis);
}