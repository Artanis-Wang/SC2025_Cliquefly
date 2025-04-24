package xnetsim.models.topologies;
import xnetsim.models.topologies.nodes.Node;
import xnetsim.models.router.Routerconnection;
import xnetsim.models.stats.*;
//import xnetsim.models.stats.TraceStats.Tracestatis;
import xnetsim.models.stats.SyntheticStats.Synstatis;
//import xnetsim.models.stats.AccNioStats.Accstatis;


network cliquefly_big  // cliquefly(9,18,9,85)
{
    parameters:
        string topoName = default("cliquefly_big");
        int gK;              //number of router in each group
        int gN;              //number of global connection of each router  
        int A = gK;      //number of router in each group
        int H = gN; //number of global connection of each router
        int numHosts = default(gK/2);
        int grpsNum = 85;
        int totalNumNetworkNode = default(gK*grpsNum*numHosts);
        int coSize = 5;  // Number of groups in each supergroup
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
        //intra group connections 
        for grpId=0..grpsNum-1, for srcNode=grpId*gK..(gK-1+grpId*gK), for dstNode=srcNode+1..(gK-1+grpId*gK) {
            router[srcNode].R_outputFlitChannel[dstNode-srcNode-1+srcNode%gK] --> FlitLink_local --> router[dstNode].R_inputFlitChannel[srcNode%gK];
            router[srcNode].R_outputCreditChannel[dstNode-srcNode-1+srcNode%gK] --> FlitLink_local --> router[dstNode].R_inputCreditChannel[srcNode%gK];
            router[dstNode].R_outputFlitChannel[srcNode%gK] --> CreditLink_local --> router[srcNode].R_inputFlitChannel[dstNode-srcNode-1+srcNode%gK];
            router[dstNode].R_outputCreditChannel[srcNode%gK] --> CreditLink_local --> router[srcNode].R_inputCreditChannel[dstNode-srcNode-1+srcNode%gK];
        }

        // intra-supergroup connections
        for co=0..gK-2, for grpId = 0..coSize-1, for destGrpId = grpId+1..coSize-1, for curRouter = 0..gK-3 {  // 这里是gK-3，因为最后2个路由器另有打算
            router[co*coSize*gK+grpId*gK+curRouter].R_outputFlitChannel[gK-1+destGrpId-1] --> FlitLink_global --> router[co*coSize*gK+destGrpId*gK+curRouter].R_inputFlitChannel[gK-1+grpId];
            router[co*coSize*gK+grpId*gK+curRouter].R_inputFlitChannel[gK-1+destGrpId-1] <-- FlitLink_global <-- router[co*coSize*gK+destGrpId*gK+curRouter].R_outputFlitChannel[gK-1+grpId];
            router[co*coSize*gK+grpId*gK+curRouter].R_outputCreditChannel[gK-1+destGrpId-1] --> CreditLink_global --> router[co*coSize*gK+destGrpId*gK+curRouter].R_inputCreditChannel[gK-1+grpId];
            router[co*coSize*gK+grpId*gK+curRouter].R_inputCreditChannel[gK-1+destGrpId-1] <-- CreditLink_global <-- router[co*coSize*gK+destGrpId*gK+curRouter].R_outputCreditChannel[gK-1+grpId];

        }
        // inter-supergroup connections
        for co=0..gK-1, for dst_co=co+1..gK-2, for grpId=0..coSize-1, for destGrpId=0..coSize-1 {
            router[co*coSize*gK+grpId*gK+dst_co-1].R_outputFlitChannel[gK-1+coSize+destGrpId] --> FlitLink_global --> router[dst_co*coSize*gK+destGrpId*gK+co].R_inputFlitChannel[gK-1+coSize+grpId];
            router[co*coSize*gK+grpId*gK+dst_co-1].R_inputFlitChannel[gK-1+coSize+destGrpId] <-- FlitLink_global <-- router[dst_co*coSize*gK+destGrpId*gK+co].R_outputFlitChannel[gK-1+coSize+grpId];
            router[co*coSize*gK+grpId*gK+dst_co-1].R_outputCreditChannel[gK-1+coSize+destGrpId] --> CreditLink_global --> router[dst_co*coSize*gK+destGrpId*gK+co].R_inputCreditChannel[gK-1+coSize+grpId];
            router[co*coSize*gK+grpId*gK+dst_co-1].R_inputCreditChannel[gK-1+coSize+destGrpId] <-- CreditLink_global <-- router[dst_co*coSize*gK+destGrpId*gK+co].R_outputCreditChannel[gK-1+coSize+grpId];

            //router[co*coSize*gK+grpId*gK+dst_co-1].R_outputFlitChannel[gK-1+coSize-1] --> FlitLink_global --> router[dst_co*coSize*gK+destGrpId*gK+co].R_inputFlitChannel[gK-1+coSize-1] if grpId == destGrpId;
            //router[co*coSize*gK+grpId*gK+dst_co-1].R_inputFlitChannel[gK-1+coSize-1] <-- FlitLink_global <-- router[dst_co*coSize*gK+destGrpId*gK+co].R_outputFlitChannel[gK-1+coSize-1] if grpId == destGrpId;
            //router[co*coSize*gK+grpId*gK+dst_co-1].R_outputCreditChannel[gK-1+coSize-1] --> CreditLink_global --> router[dst_co*coSize*gK+destGrpId*gK+co].R_inputCreditChannel[gK-1+coSize-1] if grpId == destGrpId;
            //router[co*coSize*gK+grpId*gK+dst_co-1].R_inputCreditChannel[gK-1+coSize-1] <-- CreditLink_global <-- router[dst_co*coSize*gK+destGrpId*gK+co].R_outputCreditChannel[gK-1+coSize-1] if grpId == destGrpId;
        }


        // shortcut links，  edge groups
        for co=0..gK-2, for gate=0..gN-1{
            router[co*coSize*gK+gK-1].R_outputFlitChannel[gK-1+gate] --> FlitLink_global --> router[((co+gK-2)%(gK-1))*coSize*gK+(coSize-1)*gK+gK-1].R_inputFlitChannel[gK-1+gate];
            router[co*coSize*gK+gK-1].R_inputCreditChannel[gK-1+gate] <-- CreditLink_global <-- router[((co+gK-2)%(gK-1))*coSize*gK+(coSize-1)*gK+gK-1].R_outputCreditChannel[gK-1+gate];
            router[co*coSize*gK+gK-1].R_inputFlitChannel[gK-1+gate] <-- FlitLink_global <-- router[((co+gK-2)%(gK-1))*coSize*gK+(coSize-1)*gK+gK-1].R_outputFlitChannel[gK-1+gate];
            router[co*coSize*gK+gK-1].R_outputCreditChannel[gK-1+gate] --> CreditLink_global --> router[((co+gK-2)%(gK-1))*coSize*gK+(coSize-1)*gK+gK-1].R_inputCreditChannel[gK-1+gate];

            router[co*coSize*gK+gK-2].R_outputFlitChannel[gK-1+gate] --> FlitLink_global --> router[((co+gK-2)%(gK-1))*coSize*gK+(coSize-1)*gK+gK-2].R_inputFlitChannel[gK-1+gate];
            router[co*coSize*gK+gK-2].R_inputCreditChannel[gK-1+gate] <-- CreditLink_global <-- router[((co+gK-2)%(gK-1))*coSize*gK+(coSize-1)*gK+gK-2].R_outputCreditChannel[gK-1+gate];
            router[co*coSize*gK+gK-2].R_inputFlitChannel[gK-1+gate] <-- FlitLink_global <-- router[((co+gK-2)%(gK-1))*coSize*gK+(coSize-1)*gK+gK-2].R_outputFlitChannel[gK-1+gate];
            router[co*coSize*gK+gK-2].R_outputCreditChannel[gK-1+gate] --> CreditLink_global --> router[((co+gK-2)%(gK-1))*coSize*gK+(coSize-1)*gK+gK-2].R_inputCreditChannel[gK-1+gate];
        }

        // shortcut links，  non-edge groups
        for co=0..gK-2, for grpId = 1..coSize-2, for gate=0..gN-1{
            router[co*coSize*gK+gK*grpId+gK-1].R_outputFlitChannel[gK-1+gate] --> FlitLink_global --> router[((co+gK-2)%(gK-1))*coSize*gK+gK*grpId+gK-1].R_inputFlitChannel[gK-1+gate+coSize] if gate < 4;
            router[co*coSize*gK+gK*grpId+gK-1].R_inputCreditChannel[gK-1+gate] <-- CreditLink_global <-- router[((co+gK-2)%(gK-1))*coSize*gK+gK*grpId+gK-1].R_outputCreditChannel[gK-1+gate+coSize] if gate < 4;
            router[co*coSize*gK+gK*grpId+gK-1].R_inputFlitChannel[gK-1+gate] <-- FlitLink_global <-- router[((co+gK-2)%(gK-1))*coSize*gK+gK*grpId+gK-1].R_outputFlitChannel[gK-1+gate+coSize] if gate < 4;
            router[co*coSize*gK+gK*grpId+gK-1].R_outputCreditChannel[gK-1+gate] --> CreditLink_global --> router[((co+gK-2)%(gK-1))*coSize*gK+gK*grpId+gK-1].R_inputCreditChannel[gK-1+gate+coSize] if gate < 4;

            router[co*coSize*gK+gK*grpId+gK-2].R_outputFlitChannel[gK-1+gate] --> FlitLink_global --> router[((co+gK-2)%(gK-1))*coSize*gK+gK*grpId+gK-2].R_inputFlitChannel[gK-1+gate+coSize] if gate < 4;
            router[co*coSize*gK+gK*grpId+gK-2].R_inputCreditChannel[gK-1+gate] <-- CreditLink_global <-- router[((co+gK-2)%(gK-1))*coSize*gK+gK*grpId+gK-2].R_outputCreditChannel[gK-1+gate+coSize] if gate < 4;
            router[co*coSize*gK+gK*grpId+gK-2].R_inputFlitChannel[gK-1+gate] <-- FlitLink_global <-- router[((co+gK-2)%(gK-1))*coSize*gK+gK*grpId+gK-2].R_outputFlitChannel[gK-1+gate+coSize] if gate < 4;
            router[co*coSize*gK+gK*grpId+gK-2].R_outputCreditChannel[gK-1+gate] --> CreditLink_global --> router[((co+gK-2)%(gK-1))*coSize*gK+gK*grpId+gK-2].R_inputCreditChannel[gK-1+gate+coSize] if gate < 4;

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
