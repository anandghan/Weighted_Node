/*
 * Developer : Anandghan W
 * This program finds the shortest path using Dijkstra's Algo in the constructor of 'Node' object and
 * also handles and routes packets from other nodes
 */

#include "packet_m.h"
#include <omnetpp.h>
#include <string.h>


using namespace std;

class Node : public cSimpleModule
{
	private:
		int myAdd;

		struct dj
		{
				int dist[6];
				int prev[6];
				int next[6];
				int chk[6];
		};
		struct dj nd;

	public:
		struct routingtable
		{
			int dest_addr;
			int gate;
		};
		struct routingtable routing[6];


	protected:
		// The following redefined virtual function holds the algorithm.
		virtual void initialize();
		virtual void handleMessage(cMessage *msg);
		int Dijkstras(int src , int dest , int tot);
};

// The module class needs to be registered with OMNeT++
Define_Module(Node);

int Node::Dijkstras(int src , int dest , int tot)
{
	int wt[6][6] ;
    //Assigning Weights to edges
	wt[0][0]=0;		wt[0][1]=2;		wt[0][2]=-1;	wt[0][3]=6;	    wt[0][4]=-1;	wt[0][5]=4;
	wt[1][0]=2;		wt[1][1]=0;		wt[1][2]=3;		wt[1][3]=-1;	wt[1][4]=1; 	wt[1][5]=-1;
	wt[2][0]=-1;	wt[2][1]=3;		wt[2][2]=0;		wt[2][3]=7;		wt[2][4]=-1;	wt[2][5]=-1;
	wt[3][0]=6;	    wt[3][1]=-1;	wt[3][2]=7;		wt[3][3]=0;		wt[3][4]=8;	    wt[3][5]=-1;
	wt[4][0]=-1;	wt[4][1]=1;  	wt[4][2]=-1;	wt[4][3]=8;	    wt[4][4]=0;		wt[4][5]=5;
	wt[5][0]=4;	    wt[5][1]=-1;	wt[5][2]=-1;    wt[5][3]=-1;	wt[5][4]=5;		wt[5][5]=0;

	int min,min_index=0;
	min=nd.dist[0];

	for(int i=0;i<tot;i++)//running for all nodes
	{
		//finding the min dist node

		for(int j=0;j<tot;j++)
		{
			if(nd.chk[j]==0)
			{
				min=nd.dist[j];
				min_index=j;
			}
		}

		for(int j=0;j<tot;j++)
		{
			if(nd.dist[j]<min && nd.chk[j]==0)
			{
				min_index=j;
				min=nd.dist[j];
			}
		}


		if(min==99999)
			return -1; //no route to host


		//for all neighbours
		for(int n=0;n<tot;n++)
		{
			if(wt[min_index][n]!=-1)
			{
				int alt=min + wt[min_index][n];
				if(alt<nd.dist[n])
				{
					nd.dist[n]=alt;
					nd.prev[n]=min_index;
					nd.next[min_index]=n;
				}
			}
		}

		if(min_index==dest)
		{
				int d=dest,p;
				while(1)
				{
					p=nd.prev[d];
					if(p==src)
						return d;
					d=p;
				}
		}

		nd.chk[min_index]=-1;//Removing node
	}
	return -1;
};

void Node::initialize()
{
	myAdd = this->par("address"); //finding own add
	printf("\n\n--------------------------------------Table for Node : %d--------------------------------------\n\n",myAdd);

	cTopology *topo = new cTopology();


	//Extracting the network info for ned type 'node'
	std::vector<std::string> nedTypes;
	nedTypes.push_back(this->getNedTypeName());
	topo->extractByNedTypeName(nedTypes);

	cTopology::Node *thisNode = topo->getNode(myAdd); //getting info abt current node and selecting it as source

	// Find the shortest paths and fill the routing table
	// construct the packet and send it to destination node

	int k;

	for (int i=0; i<topo->getNumNodes(); i++) //till all nodes
	    {
	        if (topo->getNode(i)==thisNode) continue; // skip ourselves

	        //---------------------------
	        for (int j=0; j<topo->getNumNodes(); j++) //till all nodes
	        {
	        	nd.dist[j]=99999;
	        	nd.prev[j]=99999;
	        	nd.chk[j]=0;
	        }
	        nd.dist[myAdd]=0;
	        int next = Dijkstras(myAdd,i,topo->getNumNodes());
	        //cout << "Node :" << i << " next : " << next << endl;

	        if(next==-1)continue;

	        cTopology::Node *node = topo->getNode(next); //pointer to next node
	        for (k=0; k< thisNode->getNumOutLinks(); k++)
	        {
	        	cTopology::Node *neighbour = thisNode->getLinkOut(k)->getRemoteNode();//current node
	        	if(node==neighbour)
	        	{
	        		break;
	        	}
	        }
	        cGate *gate = thisNode->getLinkOut(k)->getLocalGate();
	        int gateIndex = gate->getIndex();
	        int address=i;

	        //------------------------------

	        /*
	        topo->calculateUnweightedSingleShortestPathsTo(topo->getNode(i)); // shortest path

	        if (thisNode->getNumPaths()==0) continue; // not connected

	        //finding the gate index in the gate vector

	        cGate *parentModuleGate = thisNode->getPath(0)->getLocalGate();
	        int gateIndex = parentModuleGate->getIndex();
	        int address = topo->getNode(i)->getModule()->par("address");
			*/

	        //----------------------------------------

	        //filling the routing table
	        routing[i].dest_addr=address;
	        routing[i].gate=gateIndex;

	        cout << "Address : " << address << "	|	Next Hop : " << next << "	|	Gate Name : " << gate->getFullName() << "	|	Gate Index: " << gateIndex << endl;

	    }

		//sending test packets
		if(myAdd == 0)
		{
			myPacket *pck = new myPacket("test1");
			pck->setSrcAdd(myAdd);
			pck->setDestAdd(4);
			send(pck,"out",0);
		}

		if(myAdd == 5)
		{
			myPacket *pck = new myPacket("test2");
			pck->setSrcAdd(myAdd);
			pck->setDestAdd(1);
			send(pck,"out",1);
		}

		if(myAdd == 2)
		{
			myPacket *pck = new myPacket("test3");
			pck->setSrcAdd(myAdd);
			pck->setDestAdd(1);
			send(pck,"out",0);
		}

	    delete topo;
}

void Node::handleMessage(cMessage *msg)
{
	cout<<endl;

    myPacket *pck = check_and_cast<myPacket *>(msg); //Type casting
    myAdd = this->par("address"); //finding own add

    cTopology *topo = new cTopology();


   	//Extracting the network info for ned type 'node'
   	std::vector<std::string> nedTypes;
   	nedTypes.push_back(this->getNedTypeName());
   	topo->extractByNedTypeName(nedTypes);

    int destAdd = pck->getDestAdd();
    int srcAdd = pck->getSrcAdd();

    if (destAdd == myAdd)
    {
        cout << "Node " << myAdd << " Received packet with name <" << pck->getName() << "> from node "<< srcAdd << endl;
        delete topo;
        delete pck;
        return;
    }

    for (int i=0; i<topo->getNumNodes(); i++) //till all nodes
    {
    	if(routing[i].dest_addr==destAdd)
    	{
    		send(pck,"out",routing[i].gate);
    		delete topo;
    		return;
    	}
    }

    cout << "Node " << myAdd << " Received packet with name <" << pck->getName() << "> for node " << destAdd << " from node " << srcAdd << endl;
    cout << "No Route to host!" << endl;

    delete pck;
    delete topo;
}

