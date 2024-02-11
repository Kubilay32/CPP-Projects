#include "CENGFlight.h"
#include <iostream>

//=======================//
// Implemented Functions //
//=======================//
void CENGFlight::PrintCanNotHalt(const std::string& airportFrom,
                                 const std::string& airportTo,
                                 const std::string& airlineName)
{
    std::cout << "A flight path between \""
              << airportFrom << "\" and \""
              << airportTo << "\" via "
              << airlineName
              << " airlines is not found and cannot be halted"
              << std::endl;
}

void CENGFlight::PrintCanNotResumeFlight(const std::string& airportFrom,
                                         const std::string& airportTo,
                                         const std::string& airlineName)
{
    std::cout << "A flight path between \""
              << airportFrom << "\" and \""
              << airportTo << "\" via "
              << airlineName
              << " airlines cannot be resumed"
              << std::endl;
}

void CENGFlight::PrintFlightFoundInCache(const std::string& airportFrom,
                                         const std::string& airportTo,
                                         bool isCostWeighted)
{
    std::cout << "A flight path between \""
              << airportFrom << "\" and \""
              << airportTo << "\" using "
              << ((isCostWeighted) ? "cost" : "price")
              <<  " is found in cache." << std::endl;
}

void CENGFlight::PrintFlightCalculated(const std::string& airportFrom,
                                       const std::string& airportTo,
                                       bool isCostWeighted)
{
    std::cout << "A flight path is calculated between \""
              << airportFrom << "\" and \""
              << airportTo << "\" using "
              << ((isCostWeighted) ? "cost" : "price")
              <<  "." << std::endl;
}

void CENGFlight::PrintPathDontExist(const std::string& airportFrom,
                                    const std::string& airportTo)
{
    std::cout << "A flight path does not exists between \""
              << airportFrom << "\" and \""
              << airportTo <<"\"." << std::endl;
}

void CENGFlight::PrintSisterAirlinesDontCover(const std::string& airportFrom)
{
    std::cout << "Could not able to generate sister airline list from \""
              << airportFrom <<"\"." << std::endl;
}

void CENGFlight::PrintMap()
{
    navigationMap.PrintEntireGraph();
}

void CENGFlight::PrintCache()
{
    lruTable.PrintTable();
}

CENGFlight::CENGFlight(const std::string& flightMapPath)
    : navigationMap(flightMapPath)
{}

//=======================//
//          TODO         //
//=======================//
void CENGFlight::HaltFlight(const std::string& airportFrom,
                            const std::string& airportTo,
                            const std::string& airlineName)
{
    /* TODO */
    int isExistFrom = 0;
    int isExistTo = 0;
    int indexFrom;
    int indexTo;
    
    for(int i = 0; i < navigationMap.getVertexList().size(); i++){
        if(navigationMap.getVertexList()[i].name == airportFrom){
            isExistFrom = 1;
            indexFrom = i;
        }
        if(navigationMap.getVertexList()[i].name == airportTo){
            isExistTo = 1;
            indexTo = i;      
        }
    }
    
    if(isExistTo == 0 || isExistFrom == 0){
        PrintCanNotHalt(airportFrom, airportTo, airlineName);
        return;
    }

    
    int isExistEdge = 0;
    for(int j = 0; j < navigationMap.getVertexList()[indexFrom].edges.size(); j++){
        if(navigationMap.getVertexList()[indexFrom].edges[j].name == airlineName){
            isExistEdge = 1;
        }
    }
    
    if(isExistEdge == 0){
        PrintCanNotHalt(airportFrom, airportTo, airlineName);
        return;
    }
    
    int j = 0;
    for(j; j < navigationMap.getVertexList()[indexFrom].edges.size(); j++){
        if(navigationMap.getVertexList()[indexFrom].edges[j].name == airlineName){
            break;
        }
    }
    
    HaltedFlight temp;
    temp.airportFrom = airportFrom;
    temp.airportTo = airportTo;
    temp.airline = airlineName;
    temp.w0 = navigationMap.getVertexList()[indexFrom].edges[j].weight[0];
    temp.w1 = navigationMap.getVertexList()[indexFrom].edges[j].weight[1];
    haltedFlights.push_back(temp);


    navigationMap.removeEdge(airlineName, airportFrom, airportTo, indexFrom, j);
}

    // (Direct Function call)
void CENGFlight::ContinueFlight(const std::string& airportFrom,
                                const std::string& airportTo,
                                const std::string& airlineName)
{
    /* TODO */
    int index = -1;
    
    for(int j = 0; j < navigationMap.getVertexList().size(); j++){
        if(navigationMap.getVertexList()[j].name == airportTo){
            index = j;
        }
    }
    
    if(index != -1){
        for(int i = 0; i < haltedFlights.size(); i++){
            if(haltedFlights[i].airportFrom == airportFrom && haltedFlights[i].airportTo == airportTo && haltedFlights[i].airline == airlineName){
                GraphEdge newEdge;
                newEdge.name = airlineName;
                newEdge.weight[0] = haltedFlights[i].w0; newEdge.weight[1] = haltedFlights[i].w1;
                newEdge.endVertexIndex = index; 
                navigationMap.getVertexList()[i].edges.push_back(newEdge);
                haltedFlights.erase(haltedFlights.begin() + i);
                return;
            }
        }
    }   
    
    PrintCanNotResumeFlight(airportFrom, airportTo, airlineName);
}

void CENGFlight::FindFlight(const std::string& startAirportName,
                            const std::string& endAirportName,
                            float alpha)
{
    /* TODO */
    std::vector<int> v;
    
    bool is = navigationMap.HeuristicShortestPath(v, startAirportName, endAirportName, alpha);
    
    if(is == false){
        PrintPathDontExist(startAirportName, endAirportName);
        return;
    }
    
    bool isWeighted = false;
    
    if(alpha == 0 || alpha == 1){
        isWeighted = true;
    }
    
    if(alpha == 0 || alpha == 1){
        if(lruTable.Find(v, v[0], v[v.size() - 1], isWeighted, true) == true){
            PrintFlightFoundInCache(startAirportName, endAirportName, true);
            navigationMap.PrintPath(v, alpha, true);
            return;
        }
        PrintFlightCalculated(startAirportName, endAirportName, isWeighted);
    }
    
    
    navigationMap.PrintPath(v, alpha, true);
    
    

    if(alpha == 0 || alpha == 1){
        if(lruTable.isFull() == true){
            lruTable.RemoveLRU(1);
        }
        lruTable.Insert(v, true);
    }
    

    
}

void CENGFlight::FindSpecificFlight(const std::string& startAirportName,
                                    const std::string& endAirportName,
                                    float alpha,
                                    const std::vector<std::string>& unwantedAirlineNames) const
{
    /* TODO */
    std::vector<int> v;
    
    bool is = navigationMap.FilteredShortestPath(v, startAirportName, endAirportName, alpha, unwantedAirlineNames);
    
    if(is == false){
        PrintPathDontExist(startAirportName, endAirportName);
        return;
    }
    
    bool isWeighted = false;
    
    if(alpha == 0 || alpha == 1){
        isWeighted = true;
    }

    navigationMap.PrintPath(v, alpha, true);

}

void CENGFlight::FindSisterAirlines(std::vector<std::string>& airlineNames,
                               const std::string& startAirlineName,
                               const std::string& airportName) const
{
    /* TODO */
    
    int indexFrom = -1;
    for(int i = 0; i < navigationMap.getVertexList().size(); i++){
        if(navigationMap.getVertexList()[i].name == airportName){
            indexFrom = i;
        }
    }
    
    if(indexFrom == -1){
        PrintSisterAirlinesDontCover(airportName);
    }
    
}

int CENGFlight::FurthestTransferViaAirline(const std::string& airportName,
                                           const std::string& airlineName) const
{
    /* TODO */
    if(!navigationMap.find(airportName)){
        return -1;
    }
    
    int count = navigationMap.MaxDepthViaEdgeName(airportName, airlineName);
    
    return count;
}
