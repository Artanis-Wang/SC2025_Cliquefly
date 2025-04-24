package xnetsim.models.topologies;
import xnetsim.models.topologies.nodes.Node;
import xnetsim.models.router.Routerconnection;
import xnetsim.models.stats.*;
//import xnetsim.models.stats.TraceStats.Tracestatis;
import xnetsim.models.stats.SyntheticStats.Synstatis;
//import xnetsim.models.stats.AccNioStats.Accstatis;


network dragonfly_ur_global  // dragonfly(6,12,6,37)  or  dragonfly(9,18,9,82)
{
    parameters:
        string topoName = default("dragonfly_ur_global");
        int gK;              //number of router in each group
        int gN;              //number of global connection of each router  
        int A = gK;      //number of router in each group
        int H = gN; //number of global connection of each router
        
        int numHosts = default(gK/2);
        int grpsNum = default(numHosts*gN+1); // number of groups				  default(gK*gN+1); 	
        int totalNumNetworkNode = default(gK*grpsNum*numHosts);

        string statisType;
        volatile double testTimeout = default(0);
        bool testing = default(false);
        int tcpPort = default(20000);
        int tcpDmaPort = default(20100);
        double timeFactor = default(1.0);
        int tfType = default(1);
        bool outgoing = default(true);
        int numProcPerNode = default(1);
        int numNioPerNode = default(1);
        int numProcs = totalNumNetworkNode * numProcPerNode;
        int fifo_depth_hnr = default(64);
        bool tlp_only = default(false);
        int logging = default(0);
        int timeAlgorithm = default(1);
        string ivshmemName = default("ivshmem");
        double ivshmemLength = default(67108864);
        string executionMode = default("IPC");
        double glexRingBufLen = default(168000);
        double dmaRingBufLen = default(23040);
        string programType = default("MPIPro");
        bool isModifyPollReq = default(false);
        bool outputstatisinfo = default(false);
        int num_lps = default(1);
        bool enableRecordLog = default(false);

        @display("bgb=844,407");
    types:
       
       channel FlitLink_global extends ned.DatarateChannel	
        {
         parameters:
                delay = 200ns;
        }
        channel CreditLink_global extends ned.DatarateChannel
        {
           parameters:
                delay = 200ns; 
        }
        channel FlitLink_local extends ned.DatarateChannel
        {
           parameters:
                 delay = 20ns;  
        }
        channel CreditLink_local extends ned.DatarateChannel
        {
         parameters:
                delay = 20ns; 
        }
        channel FlitLink1 extends ned.DatarateChannel
        {
         parameters:
                delay = 2ns;
        }
        channel CreditLink1 extends ned.DatarateChannel
        {
        parameters:
                 delay = 2ns;  
        }
        
    submodules:
        router[gK*grpsNum]: Routerconnection {
            parameters:
                id = index;
                numPorts = gK+gN+numHosts-1;
                @display("p=108,118,ri,300,300");
            gates:
                R_outputFlitChannel[gK+gN+numHosts-1];
                R_inputCreditChannel[gK+gN+numHosts-1];
                R_inputFlitChannel[gK+gN+numHosts-1];
                R_outputCreditChannel[gK+gN+numHosts-1];
        }
        Node[totalNumNetworkNode]: Node {
            parameters:
                id = index;
                //                numHosts = 1;
                @display("p=85,85,ri,300,300");
            gates:
                Node_outputFlitChannel[2];
                Node_inputCreditChannel[2];
                Node_inputFlitChannel[2];
                Node_outputCreditChannel[2];
        }
        statis[num_lps]: <statisType> like statis_Ifc {
            parameters:
                num_lps = num_lps;
                id = index;
                @display("p=467,30");
            gates:
                statis_out[num_lps];
                statis_in[num_lps];
        }
    connections allowunconnected:
        //connect intra-group routers
        for grpId=0..grpsNum-1, for srcNode=grpId*gK..(gK-1+grpId*gK), for dstNode=srcNode+1..(gK-1+grpId*gK) {
            router[srcNode].R_outputFlitChannel[dstNode-srcNode-1+srcNode%gK] --> FlitLink_local --> router[dstNode].R_inputFlitChannel[srcNode%gK];
            router[srcNode].R_outputCreditChannel[dstNode-srcNode-1+srcNode%gK] --> FlitLink_local --> router[dstNode].R_inputCreditChannel[srcNode%gK];
            router[dstNode].R_outputFlitChannel[srcNode%gK] --> CreditLink_local --> router[srcNode].R_inputFlitChannel[dstNode-srcNode-1+srcNode%gK];
            router[dstNode].R_outputCreditChannel[srcNode%gK] --> CreditLink_local --> router[srcNode].R_inputCreditChannel[dstNode-srcNode-1+srcNode%gK];
        }

        // connect in first cycle
        for grpId=0..grpsNum-1, for m=0..int(gK/2)-1, for j=0..gN-1 {
            router[gK*grpId+int(gK/2)-1-m].R_outputFlitChannel[gK+j-1] --> FlitLink_global --> router[(grpId*gK+gN*gK*m+m+j*gK+gK)%(gK*grpsNum)].R_inputFlitChannel[gK+gN-j-2];
            router[gK*grpId+int(gK/2)-1-m].R_outputCreditChannel[gK+j-1] --> CreditLink_global --> router[(gK*grpId+gN*gK*m+m+j*gK+gK)%(gK*grpsNum)].R_inputCreditChannel[gK+gN-j-2];
        }

        // connect in second cycle
        for grpId=0..grpsNum-1, for m=0..int(gK/2)-1, for j=0..gN-1 {  //for grpId=0..grpsNum-1, for m=0..int(gK/2)-2, for j=0..gN-1 {
            router[gK*grpId+gK-1-m].R_outputFlitChannel[gK+j-1] --> FlitLink_global --> router[(grpId*gK+gN*gK*m+int(gK/2)+m+j*gK+gK)%(gK*grpsNum)].R_inputFlitChannel[gK+gN-j-2];
            router[gK*grpId+gK-1-m].R_outputCreditChannel[gK+j-1] --> CreditLink_global --> router[(gK*grpId+gN*gK*m+int(gK/2)+m+j*gK+gK)%(gK*grpsNum)].R_inputCreditChannel[gK+gN-j-2];
        }
        // connect to nodes
        for node=0..gK*grpsNum-1, for gateId=0..gN-1 {
            Node[node*gN+gateId].Node_outputFlitChannel[0] --> FlitLink1 --> router[node].R_inputFlitChannel[gK+gN-1+gateId];
            Node[node*gN+gateId].Node_outputCreditChannel[0] --> CreditLink1 --> router[node].R_inputCreditChannel[gK+gN-1+gateId];
            router[node].R_outputFlitChannel[gK+gN-1+gateId] --> FlitLink1 --> Node[node*gN+gateId].Node_inputFlitChannel[0];
            router[node].R_outputCreditChannel[gK+gN-1+gateId] --> CreditLink1 --> Node[node*gN+gateId].Node_inputCreditChannel[0];
        }
        for i=1..num_lps-1 {
            statis[i].statis_out[0] --> FlitLink1 --> statis[0].statis_in[i];
            statis[0].statis_out[i] --> FlitLink1 --> statis[i].statis_in[0];
        }
    }
}
