/*	Exercise 1.9.10 Interger Programming, L. Wosey 
	Author : Gabriel Morete 
*/

#include "gurobi_c++.h"
using namespace std;

void read_input(int &n, int &m, vector<int> &f, vector<vector<int>> &c){
	cout << "Do ./1.9.10 < input.txt" << endl;
	cin >> n >> m;
	f.resize(n);
	c.resize(m);
	for (int i = 0; i < m; i++)
		c[i].resize(n);
	for (int j = 0; j < n; j++)
		cin >> f[j];
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
			cin >> c[i][j];
}

int main() {
	try {

		// Create an environment
		GRBEnv env = GRBEnv(true);
		env.set("LogFile", "1.9.10.log");
		env.start();

		// Create an empty model
		GRBModel model = GRBModel(env);

		// Data variables	
		int n, m;
		vector<int> f;
		vector<vector<int>> c;

		read_input(n, m, f, c);

		// Create variables
		GRBVar y[n];
		for (int j = 0; j < n; j++)
			y[j] = model.addVar(0.0, 1.0, f[j], GRB_BINARY, "y" + to_string(j));

		GRBVar x[m][n];
		for (int i = 0; i < m; i++)
			for (int j = 0; j < n; j++)
				x[i][j] = model.addVar(0.0, 1.0, c[i][j], GRB_CONTINUOUS, "x" + to_string(i) + "," + to_string(j));		

		// Add constraints
		double ones[n];
		for (int j = 0; j < n; j++) 
			ones[j] = 1.0;

		for (int i = 0; i < m; i++) {
			GRBLinExpr exp;
			exp.addTerms(ones, x[i], n);
			model.addConstr(exp, GRB_EQUAL, 1, "demand" + to_string(i));
		}

		for (int j = 0; j < n; j++) {
			GRBLinExpr exp;
			for (int i = 0; i < m; i++)
				exp += x[i][j];
			exp -= m * y[j];
			model.addConstr(exp, GRB_LESS_EQUAL, 0, "max" + to_string(j));
		}

		// Optimize model
		model.optimize();


		// Print solution
		cout << "y : ";
		for (int j = 0; j < n; j++)
			cout << y[j].get(GRB_DoubleAttr_X)<< ' ';
		cout << endl;

		cout<< "x : ";
		for (int i = 0; i < m; i++) {
			cout << (i == 0? "" : "    ");
			for (int j = 0; j < n; j++)
				cout << x[i][j].get(GRB_DoubleAttr_X) << ' ';
			cout << endl;	 	
		}

		cout << "Obj : " << model.get(GRB_DoubleAttr_ObjVal) << endl;

	} catch(GRBException e) {
		cout << "Error code = " << e.getErrorCode() << endl;
		cout << e.getMessage() << endl;
	} catch(...) {
		cout << "Exception during optimization" << endl;
}

return 0;
}
