#include <iostream>
#include <map>
#include <set>
#include <cmath>
#include <cstdlib>
#include <limits.h>
#include <format>
#include <time.h>
#include "hash_funcs/hash_funcs.h"
using namespace std;

// Simple container for server requests, will be used to maintain sessions.
class ServerRequest {
    public:
    int userId;
    ServerRequest(int uId) {
        userId = uId;
    }

    bool operator <(const ServerRequest s1) const {
        return s1.userId < userId;
    }

    int getId() {
        return userId;
    }
};

/*
 Simulates a load balancer by providing the following functionality:
 - Maps requests to some server specified by an int 
 - Tracks the "load" i.e number of active sessions tracked by each server.
 - Determines expected load E(X) where X = server load as a running mean
*/
class LoadBalancer {
    // The number of servers to maintain while allocating requests/sessions.
    int numServers;
    // For each server indexed by i, maintains the list of active sessions on the same
    map< int, set<ServerRequest> > serverSessionMaintainer;
    // Hash to map a ServerRequest to a specific server
    // The input is always a server request and the output is always the id for a server.
    function<int(ServerRequest)> requestToServerMapper;

    int maxLoad;
    float expectedLoad;

    public:

    LoadBalancer(int nServers, function<int(ServerRequest)> requestMapper) {
        numServers = nServers;
        requestToServerMapper = requestMapper;

        maxLoad = expectedLoad = 0;
    }

    void serveRequest(ServerRequest serverRequest) {
        int mappedServer = requestToServerMapper(serverRequest);
        
        serverSessionMaintainer[mappedServer].insert(serverRequest);
        maxLoad = max(maxLoad,  (int) serverSessionMaintainer[mappedServer].size());
    }

    int getMaxLoad() {
        return maxLoad;
    }

    int getExpectedLoad() {
        return expectedLoad;
    }

    // Once any server's load exceeds this limit, the requests start breaking down.
    int getDisallowedLoadForServer() {
        return sqrt(numServers) * expectedLoad;
    }

    void runSimulation(vector<ServerRequest> simulatedRequests) {
        for(ServerRequest serverRequest: simulatedRequests) {
            serveRequest(serverRequest);
        }
        expectedLoad = 0 ;
        for(auto it: serverSessionMaintainer) {
            expectedLoad += it.second.size();
        }
        expectedLoad /= numServers;
        cout<< "Max Load: " << maxLoad<< " Expected Load: "<< expectedLoad << endl;
    }

};

vector<pair<string,function<int(ServerRequest)>>> hashFuncs () {
    vector<pair<string, function<int(ServerRequest)>>> funcs;

    // a*x + b modulo p where p is prime.
    function <int(ServerRequest)> serverMappingWithSimplePolynomialHash = [](ServerRequest s) -> int {
        return polynomialHash(10, 10, 4091, s.getId()); 
    };  funcs.push_back({"SimplePolynomial", serverMappingWithSimplePolynomialHash});

    return funcs;
}

vector<ServerRequest> getRandomStreamOfRequests(int numRequests) {
        int maxUsers = 1000000;
        vector<ServerRequest> requests;
        for(int i=0; i< numRequests; i++) requests.push_back(ServerRequest(rand() % maxUsers));
        return requests;
    }

int main(int argc, char ** argv) {
    srand(time(0)); cout<<endl;

    auto requests = getRandomStreamOfRequests(1000000);
    int numServers = 4091;
    for(auto func: hashFuncs()) {
        auto loadBalancer = LoadBalancer(numServers, func.second);
        cout<<"Hashing function: "<<func.first<<" ";
        loadBalancer.runSimulation(requests);
    }
    return 0;
}
