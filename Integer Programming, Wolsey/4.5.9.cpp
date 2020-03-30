/*	Exercise 4.5.9 Interger Programming, L. Wosey 
	Author : Gabriel Morete 
*/

#include "gurobi_c++.h"
#include <tuple>
using namespace std;

#define IMPOSSIBLE -1

class Graph {
	private:
		int V{0}, A{0}, cst{0};
		vector< vector< pair<int, int> > > adj;
		vector< tuple<int, int, int> > edg;
	public:
		int **adjm = NULL;

		Graph(int vtx) {
			V = vtx;
			adj.resize(vtx);
		}

		Graph() {};

		int vertex() { return V;}

		int edge() { return A;}

		int cost() { return cst;}

		vector< pair<int, int> > get_adj(int vtx) { return adj[vtx];}

		vector< tuple<int, int, int> > get_edg() { return edg;}

		int** get_adjm(int init_cost) {
			adjm = new int*[V];
			for (int i = 0; i < V; i++) {
				adjm[i] = new int[V];
				for (int j = 0; j < V; j++)
					adjm[i][j] = init_cost;
			}    
			for (int i = 0; i < V; i++)
				for (auto x : adj[i])
					adjm[i][x.first] = x.second;    
			return adjm;
		}

		void delete_adjm(){
			for (int i = 0; i < V; i++)
				delete [] adjm[i];
			delete [] adjm;
			adjm = NULL;
		}

		void add_edge(int a, int b) {
			adj[a].push_back({b, 1});
			adj[b].push_back({a, 1});
			edg.push_back({1, a, b});
			A++;
			cst++;
		}

		void add_edge(int a, int b, int c) {
			adj[a].push_back({b, c});
			adj[b].push_back({a, c});
			edg.push_back({c, a, b});
			A++;
			cst += c;
		}

		void read_file() {
			freopen("4.5.9_data", "r", stdin);
			int v, a;
			cin>>v>>a;
			V = v;
			adj.resize(v);

			int x, y, z;
			for (int i = 0; i < a; i++){
				cin>>x>>y>>z;
				add_edge(x, y + V/2, z); // Shift the second layer
			}
			fclose(stdin);    
		}

		void show() {
			cout<<V<<' '<<A<<endl;
			for (int i = 0; i < V; i++){
				for (auto x : adj[i])
					cout<<x.first<<' ';
			cout<<endl;
			}
		}
};


int main(int argc, char *argv[]) {
	Graph G = Graph();
	G.read_file();

	int n = G.vertex();

	GRBEnv *env = NULL;
	GRBVar **vars = NULL;

	vars = new GRBVar*[n];
	for (int i = 0; i < n; i++)
		vars[i] = new GRBVar[n];

	try {

		env = new GRBEnv();
		GRBModel model = GRBModel(*env);

		// Create binary decision variables

		G.get_adjm(IMPOSSIBLE);

		GRBLinExpr expr_obj = 0;

		for (int i = 0; i < n; i++) {
			for (int j = 0; j <= i; j++) {
				vars[i][j] = model.addVar(0.0, 1.0, 0.0,
				GRB_BINARY, "x_"+to_string(i)+"_"+to_string(j));
				vars[j][i] = vars[i][j];
				expr_obj += G.adjm[i][j] * vars[i][j];
			}
		}

		// Set objective function
		model.setObjective( expr_obj, GRB_MINIMIZE);
		

		// Constraints

		for (int i = 0; i < n; i++)
			for (int j = 0; j <= i; j++)
				if (G.adjm[i][j] == IMPOSSIBLE){
					vars[i][j].set(GRB_DoubleAttr_UB, 0);
				}

		for (int i = 0; i < n; i++) {
			GRBLinExpr expr = 0;
			for (int j = 0; j < n; j++)
				expr += vars[i][j];
			model.addConstr(expr <= 1, "disjoint_"+to_string(i));
		}

		G.delete_adjm();

		// Must be a perfect matching

		GRBLinExpr expr = 0;
		for (int i = 0; i < n; i++) {
			for (int j = 0; j <= i; j++) {
				expr += vars[i][j];
			}
		}
		model.addConstr(expr == n/2, "perfect matching");

		// Optimize model

		model.optimize();

		// Print solution

		cout<<"  ";
		for (int i = 0; i < n/2; i++)
			cout<<i<<' ';
		cout<<endl;
		for (int i = 0; i < n/2; i++){
			cout<<i<<' ';
			for (int j = 0; j < n/2; j++)
				cout<<abs(vars[i][j + n/2].get(GRB_DoubleAttr_X))<<' ';
		cout<<endl;   
		}

		cout << "Obj : " << model.get(GRB_DoubleAttr_ObjVal) << endl;

	} catch (GRBException e) {
		cout << "Error number: " << e.getErrorCode() << endl;
		cout << e.getMessage() << endl;
	} catch (...) {
		cout << "Error during optimization" << endl;
	}

	for (int i = 0; i < n; i++)
		delete[] vars[i];
	delete[] vars;
	delete env;
	return 0;
}