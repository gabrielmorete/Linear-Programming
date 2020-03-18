/*	Exercise 1.9.13 Interger Programming, L. Wosey 
	Author : Gabriel Morete 
*/

#include "gurobi_c++.h"
using namespace std;

void read_input(int &n, int &m, vector<int> &d, vector<int> &f, vector<int> &h, vector<int> &p){
	cout << "Do ./1.9.13 < 1.9.13_data" << endl;
	cin >> n >> m;
	d.resize(n + 1);
	f.resize(n + 1);
	h.resize(n + 1);
	p.resize(n + 1);
	
	for (int j = 1; j < n + 1; j++)
		cin >> d[j];
	for (int j = 1; j < n + 1; j++)
		cin >> f[j];
	for (int j = 1; j < n + 1; j++)
		cin >> h[j];
	for (int j = 1; j < n + 1; j++)
		cin >> p[j];
	d[0] = f[0] = h[0] = p[0] = 0;
}	

int main() {
	try {

		// Create an environment
		GRBEnv env = GRBEnv(true);
		env.set("LogFile", "1.9.13.log");
		env.start();

		// Create an empty model
		GRBModel model = GRBModel(env);

		// Data variables	
		int n, m;
		vector<int> d, f, h, p;
		
		read_input(n, m, d, f, h, p);

		// Create variables
		GRBVar y[n + 1];
		GRBVar x[n + 1];
		GRBVar s[n + 1];
		for (int j = 0; j < n + 1; j++){
			y[j] = model.addVar(0.0, 1.0, f[j], GRB_BINARY, "y" + to_string(j));
			x[j] = model.addVar(0.0, GRB_INFINITY, p[j], GRB_INTEGER, "x" + to_string(j));
			s[j] = model.addVar(0.0, GRB_INFINITY, h[j], GRB_INTEGER, "s" + to_string(j));
		}

			
		// Add constraints
		model.addConstr( s[0], GRB_EQUAL, 0, "s0");

		for (int i = 1; i < n + 1; i++) {
			model.addConstr(s[i - 1] + x[i] - s[i], GRB_EQUAL, d[i], "demand" + to_string(i));
			model.addConstr(x[i] - m * y[i], GRB_LESS_EQUAL, 0 , "prod" + to_string(i));
		}

		// Optimize model
		model.optimize();


		// Print solution
		cout << "y : ";
		for (int j = 0; j < n; j++)
			cout << y[j + 1].get(GRB_DoubleAttr_X)<< ' ';
		cout << endl;

		cout << "x : ";
		for (int j = 0; j < n; j++)
			cout << x[j + 1].get(GRB_DoubleAttr_X)<< ' ';
		cout << endl;
		
		cout << "s : ";
		for (int j = 0; j < n; j++)
			cout << s[j + 1].get(GRB_DoubleAttr_X)<< ' ';
		cout << endl;
		

		cout << "Obj : " << model.get(GRB_DoubleAttr_ObjVal) << endl;

	} catch(GRBException e) {
		cout << "Error code = " << e.getErrorCode() << endl;
		cout << e.getMessage() << endl;
	} catch(...) {
		cout << "Exception during optimization" << endl;
}

return 0;
}
