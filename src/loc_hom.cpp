#include "micro.h"

void Problem::loc_hom_Stress (int macroGp_id, double *MacroStrain, double *MacroStress)
{
  double *int_vars;

  // search for the macro gauss point, if not found just create and insert
  std::list<MacroGp_t>::iterator it;
  for (it=MacroGp_list.begin(); it !=  MacroGp_list.end(); it++) {
    if (it->id == macroGp_id) {
     cout << "The macro_id "<< macroGp_id << " was found in the list" << endl; 
     int_vars = it->int_vars;
     break;
    }
  }
  if (it ==  MacroGp_list.end()) {
    cout << "The macro_id "<< macroGp_id << " was NOT found in the list, inserting..." << endl; 
    MacroGp_t macroGp_new;
    macroGp_new.id = macroGp_id;
    macroGp_new.int_vars = NULL;
    MacroGp_list.push_back(macroGp_new);
    int_vars = NULL;
  }

  setDisp(MacroStrain);
  newtonRaphson();
  calcAverageStress();
  for (int v=0; v<nvoi; v++)
    MacroStress[v] = stress_ave[v];
}

void Problem::loc_hom_Ctan (int macroGp_id, double *MacroStrain, double *MacroCtan)
{
  double *int_vars;

  // search for the macro gauss point, if not found just create and insert
  std::list<MacroGp_t>::iterator it;
  for (it=MacroGp_list.begin(); it !=  MacroGp_list.end(); it++) {
    if (it->id == macroGp_id) {
     int_vars = it->int_vars;
     break;
    }
  }
  if (it ==  MacroGp_list.end()) {
    MacroGp_t macroGp_new;
    macroGp_new.id = macroGp_id;
    macroGp_new.int_vars = NULL;
    MacroGp_list.push_back(macroGp_new);
    int_vars = NULL;
  }

  double Strain_pert[6], Stress_0[6];
  double delta_Strain = 0.00001;

  // calculate Stress_0 

  setDisp(MacroStrain);
  newtonRaphson();
  calcAverageStress();
  for (int v=0; v<nvoi; v++)
    Stress_0[v] = stress_ave[v];

  // calculate Stress_1 .. 2 .. 3 , we calc directly MacroCtan

  for (int i=0; i<nvoi; i++) {

    for (int v=0; v<nvoi; v++)
      Strain_pert[v] = MacroStrain[v];

    Strain_pert[i] += delta_Strain; // we pertubate only one direction

    setDisp(Strain_pert);
    newtonRaphson();
    calcAverageStress();
    for (int v=0; v<nvoi; v++)
      MacroCtan[v*nvoi + i] = (stress_ave[v] - Stress_0[v]) / delta_Strain;

  }

}
