#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

int main(int argc, char const *argv[])
{
    if (argc < 2) {
        cout<<"Usage: "<<argv[0]<< " graph.txt    coloring.txt"<<endl;
        return 1;
    }

    ifstream graph (argv[1]);
    if (!graph){
        cerr<<"Cannot open file "<<argv[1]<<endl;
        return 1;
    }
    ifstream colors (argv[2]);
    if (!colors){
        cerr<<"Cannot open file "<<argv[2]<<endl;
        return 1;
    }
    
    vector<int> vertices_coloring;
    int k = 0;
    colors>>k;     // Check if graph is k-colorable?

    // 1. Check if k is maximum color.
    int c=0;
    while (colors>>c)
    {
        if (c > k || c < 1)
        {
            cerr<<"ERR: the color is invalid!"<<endl;
            return 1;
        }
        vertices_coloring.push_back(c);     // vertices_coloring[i] = color of vertice i 
    }
 
    int number_of_vertices = 0;
    graph>>number_of_vertices;

    int number_of_edges = 0;
    graph >> number_of_edges;

    // 2. Check that each vertice has a color 
    if (vertices_coloring.size() != number_of_vertices)
    {
        cerr<<"ERR: The number of vertices are invalid!"<<endl;
        return 0;
    }

    // 3. Check that no two adjacent vertices are of the same color 
    int u, v;
    for (size_t i = 0; i < number_of_edges; i++)
    {
        graph>>u>>v;
        if (vertices_coloring[u-1] == vertices_coloring[v-1])
        {
            cerr<<"ERR: The colors of "<<u<<" and " <<v<<" are the same!"<<endl;
            return 0;
        }
    }
    
    // Everything is OK
    cout<<argv[2]<<": OK"<<endl;
    return 0;
}
