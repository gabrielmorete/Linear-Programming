/*  Exercise 7.8.13 Interger Programming, L. Wosey 
    Author : Gabriel Morete 
*/

/* This example formulates and solves the following simple MIP model:
  12228 x1 + 36679 x2 + 36682 x3 + 48908 x4 + 61139 x5 + 73365 x6 = 89716837
  x1, x2, x3, x4. x5, x6 binary
*/

#include "gurobi_c++.h"
using namespace std;

int
main(int   argc,
     char *argv[])
{
  try {

    // Create an environment
    GRBEnv env = GRBEnv(true);
    env.start();

    // Create an empty model
    GRBModel model = GRBModel(env);

    // Create variables
    GRBVar x1 = model.addVar(0.0, GRB_INFINITY, 0.0, GRB_INTEGER, "x1");
    GRBVar x3 = model.addVar(0.0, GRB_INFINITY, 0.0, GRB_INTEGER, "x3");
    GRBVar x4 = model.addVar(0.0, GRB_INFINITY, 0.0, GRB_INTEGER, "x4");
    GRBVar x5 = model.addVar(0.0, GRB_INFINITY, 0.0, GRB_INTEGER, "x5");
    GRBVar x6 = model.addVar(0.0, GRB_INFINITY, 0.0, GRB_INTEGER, "x6");
    GRBVar x2 = model.addVar(0.0, GRB_INFINITY, 0.0, GRB_INTEGER, "x2");

    // Add constraints
    model.addConstr(12228 * x1 + 36679 * x2 + 36682 * x3 + 
      48908 * x4 + 61139 * x5 + 73365 * x6 == 89716837);

    // Optimize model
    model.optimize();

    cout << x1.get(GRB_DoubleAttr_X) << " "
         << x2.get(GRB_DoubleAttr_X) << " "
         << x3.get(GRB_DoubleAttr_X) << " "
         << x4.get(GRB_DoubleAttr_X) << " "
         << x5.get(GRB_DoubleAttr_X) << " "
         << x6.get(GRB_DoubleAttr_X) << endl;
   

    cout << "Obj: " << model.get(GRB_DoubleAttr_ObjVal) << endl;

  } catch(GRBException e) {
    cout << "Error code = " << e.getErrorCode() << endl;
    cout << e.getMessage() << endl;
  } catch(...) {
    cout << "Exception during optimization" << endl;
  }

  return 0;
}
