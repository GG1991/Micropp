/*
 *  This source code is part of MicroPP: a finite element library
 *  to solve microstructural problems for composite materials.
 *
 *  Copyright (C) - 2018 - Jimmy Aguilar Mena <kratsbinovish@gmail.com>
 *                         Guido Giuntoli <gagiuntoli@gmail.com>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
#include "micro.hpp"

using namespace std;

template <int tdim>
void micropp<tdim>::output(int time_step, int gp_id)
{
	INST_START;

	assert(gp_id < ngp);
	assert(gp_id >= 0);

	if (!gp_list[gp_id].allocated) {
		vars_old = vars_old_aux;
		vars_new = vars_new_aux;
		memset(vars_old, 0, num_int_vars * sizeof(double));
	} else {
		vars_old = gp_list[gp_id].int_vars_n;
		vars_new = gp_list[gp_id].int_vars_k;
	}

	double *u = gp_list[gp_id].u_k;

	calc_fields(u);
	write_vtu(u, time_step, gp_id);
}


template <int tdim>
void micropp<tdim>::write_vtu(const double *u, int time_step, int gp_id)
{
	assert(gp_id < ngp);
	assert(gp_id >= 0);

	std::stringstream fname_vtu_s;
	fname_vtu_s << "micropp_" << gp_id << "_" << time_step << ".vtu";
	std::string fname_vtu = fname_vtu_s.str();

	ofstream file;
	file.open(fname_vtu);

	file << "<?xml version=\"1.0\"?>\n"
	     <<	"<VTKFile type=\"UnstructuredGrid\" version=\"0.1\" "
	     << "byte_order=\"LittleEndian\">\n"
	     << "<UnstructuredGrid>\n"
	     << "<Piece NumberOfPoints=\"" << nn << "\" NumberOfCells=\""
	     << nelem << "\">\n"
	     << "<Points>\n"
	     << "<DataArray type=\"Float32\" Name=\"Position\" "
	     << "NumberOfComponents=\"3\" format=\"ascii\">" << endl;

	for (int k = 0; k < nz; ++k) {
		for (int j = 0; j < ny; ++j) {
			for (int i = 0; i < nx; ++i) {
				const double coor[3] = { i * dx, j * dy, k * dz };
				for (int d = 0; d < 3; d++)
					file << coor[d] << " ";
				file << endl;
			}
		}
	}
	file << "</DataArray>\n</Points>\n<Cells>" << endl;

	file
		<< "<DataArray type=\"Int32\" Name=\"connectivity\" "
		<< "NumberOfComponents=\"1\" format=\"ascii\">" << endl;
	for (int ez = 0; ez < nez; ++ez) {
		for (int ey = 0; ey < ney; ++ey) {
			for (int ex = 0; ex < nex; ++ex) {
				int n[8];
				get_elem_nodes(n, ex, ey, ez);
				for (int i = 0; i < npe; ++i)
					file << n[i] << ' ';
				file << endl;
			}
		}
	}
	file << "</DataArray>" << endl;

	int ce = npe;
	file
		<< "<DataArray type=\"Int32\" Name=\"offsets\" "
		<< "NumberOfComponents=\"1\" format=\"ascii\">" << endl;
	for (int e = 0; e < nelem; ++e) {
		file << ce << " ";
		ce += npe;
	}
	file << "\n</DataArray>" << endl;

	file
		<< "<DataArray type=\"UInt8\"  Name=\"types\" NumberOfComponents=\"1\" "
		<< "format=\"ascii\">" << endl;
	const int vtk_code = (dim == 2) ? 9 : 12;
	for (int e = 0; e < nelem; ++e)
		file << vtk_code << " ";
	file << "\n</DataArray>" << endl;
	file << "</Cells>" << endl;

	file << "<PointData Vectors=\"displ\" >>" << endl;
	file
		<< "<DataArray type=\"Float64\" Name=\"displ\" "
		<< "NumberOfComponents=\"3\" format=\"ascii\" >" << endl;
	for (int n = 0; n < nn; ++n) {
		for (int d = 0; d < MAX_DIM; ++d)
			file << (dim == 2 && d == 2 ? 0.0 : u[n * dim + d]) << " ";
 		file << endl;
	}
	file << "</DataArray>" << endl;

	//  Deleted here
	file << "</PointData>" << endl;

	file << "<CellData>" << endl;
	file
		<< "<DataArray type=\"Float64\" Name=\"strain\" "
		<< "NumberOfComponents=\"" << nvoi << "\" format=\"ascii\">" << endl;
	for (int e = 0; e < nelem; ++e) {
		for (int v = 0; v < nvoi; ++v)
			file << elem_strain[e * nvoi + v] << " ";
		file << endl;
	}
	file << "</DataArray>";

	file
		<< "<DataArray type=\"Float64\" Name=\"stress\" "
		<< "NumberOfComponents=\"" << nvoi << "\" format=\"ascii\">" << endl;
	for (int e = 0; e < nelem; ++e) {
		for (int v = 0; v < nvoi; ++v)
			file << elem_stress[e * nvoi + v] << " ";
		file << endl;
	}
	file << "</DataArray>";

	file
		<< "<DataArray type=\"Int32\" Name=\"elem_type\" "
		<< "NumberOfComponents=\"1\" format=\"ascii\">" << endl;
	for (int e = 0; e < nelem; ++e) {
		file << elem_type[e] << " ";
	}
	file << "\n</DataArray>" << endl;

	file
		<< "<DataArray type=\"Float64\" Name=\"plasticity\" "
		<< "NumberOfComponents=\"1\" format=\"ascii\">" << endl;
	for (int e = 0; e < nelem; ++e) {
		double plasticity = 0.;
		for (int gp = 0; gp < npe; ++gp) {
			double tmp = 0.0;
			for (int v = 0; v < nvoi; ++v)
				tmp += vars_old[intvar_ix(e, gp, v)] *\
					   vars_old[intvar_ix(e, gp, v)];
			plasticity += sqrt(tmp);
		}
		file << plasticity / npe << " ";
	}
	file << "\n</DataArray>" << endl;

	file
		<< "<DataArray type=\"Float64\" Name=\"hardening\" "
		<< "NumberOfComponents=\"1\" format=\"ascii\">" << endl;
	for (int e = 0; e < nelem; ++e) {
		double hardening = 0.;
		for (int gp = 0; gp < npe; ++gp)
			hardening += vars_old[intvar_ix(e, gp, 6)];
		file << hardening / npe << " ";
	}
	file << "\n</DataArray>" << endl;
	file << "</CellData>" << endl;
	file << "</Piece>" << endl;
	file << "</UnstructuredGrid>" << endl;
	file << "</VTKFile>" << endl;

	file.close();
}


template <int tdim>
void micropp<tdim>::write_info_files()
{
	INST_START;

	ofstream file;
	if (output_files_header == false) {
		output_files_header = true;

		file.open("micropp_convergence.dat", std::ios_base::app);
		file
			<< "# gp_id    non-linear   sigma-newton-its  sigma-newton-tol"
		    << endl;
		file.close();

		file.open("micropp_eps_sig_ctan.dat", std::ios_base::app);
		file << "# gp_id : ";
		for (int igp = 0 ; igp < ngp; ++igp)
			file << igp << " ";
		file
			<< "\n# epsxx [1] # epsyy [2] # epszz[3]"
			<< "# epsxy[4]  # epsxz[5]  # epsyz[6]"
			<< "\n# sigxx [7] # sigyy [8] # sigzz[9]"
			<< "# sigxy[10] # sigxz[11] # sigyz[12]" << endl;
		file.close();

		file.open("micropp_int_vars_n.dat", std::ios_base::app);
		file << "# gp_id : ";
		for (int igp = 0 ; igp < ngp; ++igp)
			file << igp << " ";

		file
			<< "\n# eps_p_xx [1]  # eps_p_yy [2]  # eps_p_zz[3] "
			<< "# eps_p_xy[4]  # eps_p_xz[5]  # eps_p_yz[6]"
			<< "# alpha[7]"  << endl;

		file.close();
	}

	file.open("micropp_eps_sig_ctan.dat", std::ios_base::app);
	for (int igp = 0 ; igp < ngp; ++igp) {
		for (int i = 0; i < nvoi; ++i)
			file << setw(14) << gp_list[igp].macro_strain[i] << "\t";
		for (int i = 0; i < nvoi; ++i)
			file << setw(14) << gp_list[igp].macro_stress[i] << "\t";
		for (int i = 0; i < nvoi * nvoi; ++i)
			file << setw(14) << gp_list[igp].macro_ctan[i] << "\t";
		file << " | ";
	}
	file << endl;
	file.close();

	file.open("micropp_int_vars_n.dat", std::ios_base::app);
	for (int igp = 0 ; igp < ngp; ++igp) {
		for (int i = 0; i < num_int_vars; ++i) {
			if (gp_list[igp].allocated)
				file << setw(14) << gp_list[igp].int_vars_n[i] << " ";
			else
				file << setw(14) << 0.0 << " ";
		}
		file << "\t|\t";
	}
	file << endl;
	file.close();
}


template <int tdim>
void micropp<tdim>::write_convergence_file(int tstep, int rank)
{
	ofstream file;
	file.open("micropp_convergence_" + std::to_string(rank) + ".dat",
			  std::ios_base::app);
    file << "Time Step : " << setw(3) << tstep << endl;
	for (int gp = 0 ; gp < ngp; ++gp) {
		file
			<< gp << "\t"
			<< ((gp_list[gp].allocated) ? 1 : 0) << "\t"
			<< gp_list[gp].sigma_newton_its << "\t"
			<< setw(14) << scientific
			<< gp_list[gp].sigma_newton_err[gp_list[gp].sigma_newton_its]
			<< endl;
	}
	file << endl;
	file.close();
}


template class micropp<2>;
template class micropp<3>;
