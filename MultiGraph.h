#ifndef MULTI_GRAPH_H
#define MULTI_GRAPH_H

#include <vector>
#include <string>

struct GraphEdge
{
    std::string name;       // Name of the vertex
    float       weight[2];  // Weights of the edge
                            // (used on shortest path)
    int         endVertexIndex;
};

struct GraphVertex
{
    std::vector<GraphEdge> edges; // Adjacency List
    std::string            name;  // Name of the vertex
};

class MultiGraph
{
    private:
    std::vector<GraphVertex>    vertexList;

    static float Lerp(float w0, float w1, float alpha);

    protected:
    public:

    bool find(const std::string& Name)const{
        for(int i = 0; i < vertexList.size(); i++){
            if(vertexList[i].name == Name){
                return true;
            }
        }
        return false;
    }
    
    bool isAllVisited(std::vector<int> isVis)const{
        bool flag = true;
        for(int i = 0; i < isVis.size(); i++){
            if(isVis[i] == 0){
                flag = false;
            }
        }
        return flag;
    }
    
    int nvFindMinIndex(std::vector<float>& distanceVector, std::vector<int> isVisited)const{
        float min = 100000000;
        int index = -1;
        
        for(int i = 0; i < distanceVector.size(); i++){
            if(distanceVector[i] < min && isVisited[i] == 0){
                min = distanceVector[i];
                index = i;
            }    
        }
        
        return index;
    }

    
    std::vector<GraphVertex> getVertexList()const{
        return vertexList;
    } 
    void removeEdge(const std::string& edgeName, const std::string& vertexFromName, const std::string& vertexToName, int indexFrom, int j){
        vertexList[indexFrom].edges.erase(vertexList[indexFrom].edges.begin() + j);
    }
    // Constructors & Destructor
                MultiGraph();
                MultiGraph(const std::string& filePath);

    // Connect Vertices
    void        InsertVertex(const std::string& vertexName);
    void        RemoveVertex(const std::string& vertexName);

    // Connect Vertices
    void        AddEdge(const std::string& edgeName,
                        const std::string& vertexFromName,
                        const std::string& vertexToName,
                        float weight0, float weight1);
    void        RemoveEdge(const std::string& edgeName,
                           const std::string& vertexFromName,
                           const std::string& vertexToName);

    // Shortest Path Functions
    bool        HeuristicShortestPath(std::vector<int>& orderedVertexEdgeIndexList,
                                      const std::string& vertexNameFrom,
                                      const std::string& vertexNameTo,
                                      float heuristicWeight) const;
    bool        FilteredShortestPath(std::vector<int>& orderedVertexEdgeIndexList,
                                     const std::string& vertexNameFrom,
                                     const std::string& vertexNameTo,
                                     float heuristicWeight,
                                     const std::vector<std::string>& edgeNames) const;

    // Other functions
    int         BiDirectionalEdgeCount() const;
    int         MaxDepthViaEdgeName(const std::string& vertexName,
                                    const std::string& edgeName) const;

    // Implemented Functions for Debugging
    void        PrintPath(const std::vector<int>& orderedVertexEdgeIndexList,
                          float heuristicWeight,
                          bool sameLine = false) const;
    void        PrintEntireGraph() const;
};

#endif // MULTI_GRAPH_H
