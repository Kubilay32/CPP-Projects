#include "MultiGraph.h"
#include "Exceptions.h"
#include "IntPair.h"
#include <iostream>
#include <iomanip>
#include <fstream>

//=======================//
// Implemented Functions //
//=======================//
MultiGraph::MultiGraph()
{}

MultiGraph::MultiGraph(const std::string& filePath)
{
    // ============================= //
    // This function is implemented  //
    // Do not edit this function !   //
    // ============================= //
    // Tokens
    std::string tokens[5];
    std::ifstream mapFile(filePath.c_str());

    if(!mapFile.is_open())
    {
        std::cout << "Unable to open " << filePath << std::endl;
        return;
    }

    // Read line by line
    std::string line;
    while (std::getline(mapFile, line))
    {
        // Empty Line Skip
        if(line.empty()) continue;
        // Comment Skip
        if(line[0] == '#') continue;

        // Tokenize the line
        int i = 0;
        std::istringstream stream(line);
        while(stream >> tokens[i]) i++;

        // Single token (Meaning it is a vertex)
        if(i == 1)
        {
            InsertVertex(tokens[0]);
        }
        // Exactly 5 tokens (Meaning it is an edge)
        else if(i == 5)
        {
            // Rename vars for readablity
            const std::string& vertexFromName = tokens[0];
            const std::string& vertexToName = tokens[1];
            const std::string& edgeName = tokens[2];
            float weight0 = static_cast<float>(std::atof(tokens[3].c_str()));
            float weight1 = static_cast<float>(std::atof(tokens[4].c_str()));
            AddEdge(edgeName, vertexFromName, vertexToName,
                    weight0, weight1);
        }
        else std::cerr << "Token Size Mismatch" << std::endl;
    }
}

void MultiGraph::PrintPath(const std::vector<int>& orderedVertexEdgeIndexList,
                           float heuristicWeight,
                           bool sameLine) const
{
    // ============================= //
    // This function is implemented  //
    // Do not edit this file !       //
    // ============================= //

    // Name is too long
    const std::vector<int>& ove = orderedVertexEdgeIndexList;
    // Invalid list
    // At least three items should be available
    if(ove.size() < 3) return;

    // Check vertex and an edge
    for(size_t i = 0; i < orderedVertexEdgeIndexList.size(); i += 2)
    {
        int vertexId = ove[i];
        if(vertexId >= static_cast<int>(vertexList.size()))
        {
            // Return if there is a bad vertex id
            std::cout << "VertexId " << vertexId
                      << " not found!" << std::endl;
            return;
        }


        const GraphVertex& vertex = vertexList[vertexId];
        std::cout << vertex.name;
        if(!sameLine) std::cout << "\n";
        // Only find and print the weight if next is available
        if(i == ove.size() - 1) break;
        int nextVertexId = ove[i + 2];
        if(nextVertexId >= static_cast<int>(vertexList.size()))
        {
            // Return if there is a bad vertex id
            std::cout << "VertexId " << vertexId
                    << " not found!" << std::endl;
            return;
        }

        // Find the edge between these two vertices
        int localEdgeId = ove[i + 1];
        if(localEdgeId >= static_cast<int>(vertex.edges.size()))
        {
            // Return if there is a bad vertex id
            std::cout << "EdgeId " << localEdgeId
                      << " not found in " << vertexId << "!" << std::endl;
            return;
        }

        const GraphEdge& edge = vertex.edges[localEdgeId];

        // Combine with heuristic (linear interpolation)
        float weight = Lerp(edge.weight[0], edge.weight[1],
                            heuristicWeight);

        std::cout << "-" << std::setfill('-')
                  << std::setw(4)
                  << weight << "->";
    }
    // Print endline on the last vertex if same line is set
    if(sameLine) std::cout << "\n";
    // Reset fill value because it "sticks" to the std out
    std::cout << std::setfill(' ');
    std::cout.flush();
}

void MultiGraph::PrintEntireGraph() const
{
    // ============================= //
    // This function is implemented  //
    // Do not edit this function !   //
    // ============================= //
    for(size_t i = 0; i < vertexList.size(); i++)
    {
        const GraphVertex& v = vertexList[i];
        std::cout << v.name << "\n";
        for(size_t j = 0; j < v.edges.size(); j++)
        {
            const GraphEdge& edge = v.edges[j];

            // List the all vertex names and weight
            std::cout << "    -"
                      << std::setfill('-')
                      << std::setw(4) << edge.weight[0]
                      << "-"
                      << std::setw(4) << edge.weight[1]
                      << "-> ";
            std::cout << vertexList[edge.endVertexIndex].name;
            std::cout << " (" << edge.name << ")" << "\n";
        }
    }
    // Reset fill value because it "sticks" to the std out
    std::cout << std::setfill(' ');
    std::cout.flush();
}

//=======================//
//          TODO         //
//=======================//
float MultiGraph::Lerp(float w0, float w1, float alpha)
{
    /* TODO */
    float beta = w1 * alpha + w0 * (1-alpha);
    return beta;
}

void MultiGraph::InsertVertex(const std::string& vertexName)
{
    /* TODO */
    for(int i = 0; i < vertexList.size(); i++){
        if(vertexList[i].name == vertexName){
            throw DuplicateVertexException(vertexName);
        }
    }
    
    GraphVertex newVertex;
    
    newVertex.name = vertexName;
    
    vertexList.push_back(newVertex);
}

void MultiGraph::RemoveVertex(const std::string& vertexName)
{
    /* TODO */
    for(int i = 0; i < vertexList.size(); i++){
        if(vertexList[i].name == vertexName){
            while(!vertexList[i].edges.empty()){
                vertexList[i].edges.pop_back();
            }
            
            vertexList.erase(vertexList.begin() + i);
            return;
        }
    }
    
    throw VertexNotFoundException(vertexName);
}

void MultiGraph::AddEdge(const std::string& edgeName,
                         const std::string& vertexFromName,
                         const std::string& vertexToName,
                         float weight0, float weight1)
{
    /* TODO */
    int isExistFrom = 0;
    int isExistTo = 0;
    int indexFrom;
    int indexTo;
    
    for(int i = 0; i < vertexList.size(); i++){
        if(vertexList[i].name == vertexFromName){
            isExistFrom = 1;
            indexFrom = i;
        }
        if(vertexList[i].name == vertexToName){
            isExistTo = 1;
            indexTo = i;      
        }
    }
    
    if(isExistTo == 0 || isExistFrom == 0){
        throw VertexNotFoundException(vertexFromName);
    }

    
    for(int j = 0; j < vertexList[indexFrom].edges.size(); j++){
        if(vertexList[indexFrom].edges[j].name == edgeName && vertexList[indexFrom].edges[j].endVertexIndex == indexTo){
            throw SameNamedEdgeException(edgeName, vertexFromName, vertexToName);
        }
    }
    
    GraphEdge newEdge;
    
    newEdge.name = edgeName;
    newEdge.weight[0] = weight0; newEdge.weight[1] = weight1;
    newEdge.endVertexIndex = indexTo; 
    
    vertexList[indexFrom].edges.push_back(newEdge);

}

void MultiGraph::RemoveEdge(const std::string& edgeName,
                            const std::string& vertexFromName,
                            const std::string& vertexToName)
{
    /* TODO */
    int isExistFrom = 0;
    int isExistTo = 0;
    int indexFrom;
    int indexTo;
    
    for(int i = 0; i < vertexList.size(); i++){
        if(vertexList[i].name == vertexFromName){
            isExistFrom = 1;
            indexFrom = i;
        }
        if(vertexList[i].name == vertexToName){
            isExistTo = 1;
            indexTo = i;      
        }
    }
    
    if(isExistTo == 0 || isExistFrom == 0){
        throw VertexNotFoundException(vertexFromName);
    }

    
    int isExistEdge = 0;
    for(int j = 0; j < vertexList[indexFrom].edges.size(); j++){
        if(vertexList[indexFrom].edges[j].name == edgeName){
            isExistEdge = 1;
        }
    }
    
    if(isExistEdge == 0){
        throw EdgeNotFoundException(vertexFromName, edgeName);
    }
    
    for(int j = 0; j < vertexList[indexFrom].edges.size(); j++){
        if(vertexList[indexFrom].edges[j].name == edgeName && vertexList[indexFrom].edges[j].endVertexIndex == indexTo){
            vertexList[indexFrom].edges[j].weight[0] = -1;
            vertexList[indexFrom].edges[j].weight[1] = -1;
            vertexList[indexFrom].edges[j].endVertexIndex = -1;
            vertexList[indexFrom].edges.erase(vertexList[indexFrom].edges.begin() + j);
            break;
        }
    }
    
    
}

bool MultiGraph::HeuristicShortestPath(std::vector<int>& orderedVertexEdgeIndexList,
                                       const std::string& vertexNameFrom,
                                       const std::string& vertexNameTo,
                                       float heuristicWeight) const
{
    /* TODO */
    #define infi 10000000
    int indexFrom = -1, indexTo = -1;
    
    for (int i = 0; i < vertexList.size(); ++i){
        if (vertexList[i].name == vertexNameFrom){
            indexFrom = i;
        }
        
        if (vertexList[i].name == vertexNameTo){
            indexTo = i;
        }
    }


    if (indexFrom == -1 || indexTo == -1){
        throw VertexNotFoundException(vertexNameFrom);
    }

    std::vector<float> distance(vertexList.size(), infi);
    std::vector<int> previous(vertexList.size(), -1); 
    std::vector<int> isVis(vertexList.size(), 0);
    std::vector<int> edgeIndex(vertexList.size(), -1);
    
    distance[indexFrom] = 0;
    
    while(true){
        int index = nvFindMinIndex(distance, isVis);
        
        for(int i = 0; i < vertexList[index].edges.size(); i++){
            float weight = Lerp(vertexList[index].edges[i].weight[0], vertexList[index].edges[i].weight[1], heuristicWeight);
            float new_distance = distance[index] + weight;
            if(new_distance < distance[vertexList[index].edges[i].endVertexIndex]){
                distance[vertexList[index].edges[i].endVertexIndex] = new_distance;
                previous[vertexList[index].edges[i].endVertexIndex] = index;
                edgeIndex[vertexList[index].edges[i].endVertexIndex] = i;
            }
            
        }
        
        isVis[index] = 1;
        if(isAllVisited(isVis) == true){
            break;
        }
    }
    
    if(distance[indexTo] == infi){
        return false;
    }
    
    int x = indexTo;
    std::vector<int> v;

    v.push_back(x);
    while(previous[x] != -1){
        int prev = previous[x];
        
        
        v.push_back(edgeIndex[x]);
        v.push_back(prev);
        x = prev;
    }
    
    orderedVertexEdgeIndexList.clear();
    for(int i = v.size() - 1; i >= 0; i--){
        orderedVertexEdgeIndexList.push_back(v[i]);
    }

    
    return true;

}

bool MultiGraph::FilteredShortestPath(std::vector<int>& orderedVertexEdgeIndexList,
                                      const std::string& vertexNameFrom,
                                      const std::string& vertexNameTo,
                                      float heuristicWeight,
                                      const std::vector<std::string>& edgeNames) const
{
    /* TODO */
    #define infi 10000000
    int indexFrom = -1, indexTo = -1;
    
    for (int i = 0; i < vertexList.size(); ++i){
        if (vertexList[i].name == vertexNameFrom){
            indexFrom = i;
        }
        
        if (vertexList[i].name == vertexNameTo){
            indexTo = i;
        }
    }


    if (indexFrom == -1 || indexTo == -1){
        throw VertexNotFoundException(vertexNameFrom);
    }

    std::vector<float> distance(vertexList.size(), infi);
    std::vector<int> previous(vertexList.size(), -1); 
    std::vector<int> isVis(vertexList.size(), 0);
    std::vector<int> edgeIndex(vertexList.size(), -1);

    distance[indexFrom] = 0;
    
    while(true){
        int index = nvFindMinIndex(distance, isVis);
        
        
        for(int i = 0; i < vertexList[index].edges.size(); i++){
            int flag = 0;
            for(int j = 0; j < edgeNames.size(); j++){
                if(edgeNames[j] == vertexList[index].edges[i].name){
                    flag = 1;
                }
            }

            if(flag == 0){
                float weight = Lerp(vertexList[index].edges[i].weight[0], vertexList[index].edges[i].weight[1], heuristicWeight);
                float new_distance = distance[index] + weight;
                if(new_distance < distance[vertexList[index].edges[i].endVertexIndex]){
                    distance[vertexList[index].edges[i].endVertexIndex] = new_distance;
                    previous[vertexList[index].edges[i].endVertexIndex] = index;
                    edgeIndex[vertexList[index].edges[i].endVertexIndex] = i;
                }
            }
        }
        
        isVis[index] = 1;
        if(isAllVisited(isVis) == true){
            break;
        }
    }
    
    if(distance[indexTo] == infi){
        return false;
    }
    
    int x = indexTo;
    std::vector<int> v;

    v.push_back(x);
    while(previous[x] != -1){
        int prev = previous[x];
        
        v.push_back(edgeIndex[x]);
        v.push_back(prev);
        x = prev;
    }
    
    if(vertexList[x].name != vertexNameFrom){
        return false;
    }
    
    orderedVertexEdgeIndexList.clear();
    for(int i = v.size() - 1; i >= 0; i--){
        orderedVertexEdgeIndexList.push_back(v[i]);
    }

    
    return true;

}

int MultiGraph::BiDirectionalEdgeCount() const
{
    /* TODO */
    int i = 0;
    int count = 0;
    while(i != vertexList.size()){
        std::string nameFrom = vertexList[i].name;
        for(int j = 0; j < vertexList[i].edges.size(); j++){
            std::string edgeName = vertexList[i].edges[j].name;
            int index = vertexList[i].edges[j].endVertexIndex;
            for(int k = 0; k < vertexList[index].edges.size(); k++){
                if(vertexList[index].edges[k].name == edgeName && vertexList[index].edges[k].endVertexIndex == i){
                    count++;
                }
                
            }
        }
        i++;
    }
    return count / 2;
}

int MultiGraph::MaxDepthViaEdgeName(const std::string& vertexName,
                                    const std::string& edgeName) const
{
    /* TODO */
    int indexFrom = -1;
    
    for (int i = 0; i < vertexList.size(); ++i){
        if (vertexList[i].name == vertexName){
            indexFrom = i;
            break;
        }
    }


    if (indexFrom == -1){
        throw VertexNotFoundException(vertexName);
    }
    
    std::vector<int> vector;
    std::vector<bool> isVisited(vertexList.size(), false); 
    vector.push_back(indexFrom);
    std::vector<int> depth(vertexList.size(), -1);
    int edge_count = 0;
    depth[indexFrom] = 0;
    
    while(!vector.empty()){
        int index = vector.front();
        GraphVertex temp = vertexList[vector.front()];
        vector.erase(vector.begin());

        for(int i = 0; i < temp.edges.size(); i++){
            if(temp.edges[i].name == edgeName && isVisited[temp.edges[i].endVertexIndex] == false){
                vector.push_back(temp.edges[i].endVertexIndex);
                depth[temp.edges[i].endVertexIndex] = depth[index] + 1;
            }
        }

        isVisited[index] = true;
    }
    
    int max = -1;
    
    for(int j = 0; j < depth.size(); j++){
        if(depth[j] > max){
            max = depth[j];
        }
    }
    
    return max;
    
}
