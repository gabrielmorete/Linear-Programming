/*	Exercise 3.8.6 Interger Programming, L. Wosey 
	Author : Gabriel Morete 
*/

#include "gurobi_c++.h"
#include <tuple>
#include <queue>
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

        void add_arc(int a, int b) {
            adj[a].push_back({b, 1});
            edg.push_back({1, a, b});
            A++;
            cst++;
        }

        void add_arc(int a, int b, int c) {
            adj[a].push_back({b, c});
            edg.push_back({c, a, b});
            A++;
            cst += c;
        }

		void read_file() {
			if (freopen("3.8.6_data", "r", stdin) == NULL) {
				cout<<"Erro : Arquivo não existe"<<endl;
				return;
			}
			int v, a;
			cin>>v>>a;
			V = v;
			adj.resize(v);

			int x, y, z;
			for (int i = 0; i < a; i++){
				cin>>x>>y>>z;
				add_arc(x, y, z);
			}
			fclose(stdin);    
		}

		void show() {
			cout<<V<<' '<<A<<endl;
			for (int i = 0; i < V; i++){
				for (auto x : adj[i])
					cout<<x.second<<' ';
			cout<<endl;
			}
		}

		vector<int> dijkstra(int src) {
        	vector<int> dist(V, -1);
        	priority_queue< pair<int,int>,vector< pair<int,int> >,greater<pair<int,int> > > pq;
        	pq.push({0,src});

        	pair<int, int> aux;
        	int dst, vtx;
        	while (!pq.empty()) {
        		aux = pq.top();
        		pq.pop();
        		dst = aux.first;
        		vtx = aux.second;

        		if (dist[vtx] != -1)
        			continue;

        		dist[vtx] = dst;
        		for (auto x : adj[vtx])
        			if (dist[x.first] == -1) {
        				pq.push({dst + x.second, x.first});
        			}
        	}
        	return dist;
        }   
};


int main(int argc, char *argv[]) {
	Graph G = Graph();
	G.read_file();

	G.show();

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

		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				vars[i][j] = model.addVar(0.0, 1.0, G.adjm[i][j],
				GRB_BINARY, "x_"+to_string(i)+"_"+to_string(j));
			}
		}

		// Set inexisting arcs to zero

		for (int i = 0; i < n; i++)
			for (int j = 0; j < n; j++)
				if (G.adjm[i][j] == IMPOSSIBLE){
					vars[i][j].set(GRB_DoubleAttr_UB, 0);
				}

		G.delete_adjm();


		// Initial vertex

		GRBLinExpr expr = 0;
		for (int i = 0; i < n; i++) {
			expr += vars[0][i];
			expr -= vars[i][0];
		}
		model.addConstr(expr == 1, "src");
		
		// Middle veticies
		
		for (int j = 1; j < n - 1; j++) {
			expr = 0;
			for (int i = 0; i < n; i++) {
				expr += vars[j][i];
				expr -= vars[i][j];
			}
			model.addConstr(expr == 0, "mid_" + to_string(j));
		}

		// End vertex

		for (int i = 0; i < n; i++) {
			expr += vars[n - 1][i];
			expr -= vars[i][n - 1];
		}
		model.addConstr(expr == -1, "snk");

		// Optimize model

		model.optimize();

		// Print solution

		for (int i = 0; i < n; i++){
			for (int j = 0; j < n; j++)
				cout<<abs(vars[i][j].get(GRB_DoubleAttr_X))<<' ';
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