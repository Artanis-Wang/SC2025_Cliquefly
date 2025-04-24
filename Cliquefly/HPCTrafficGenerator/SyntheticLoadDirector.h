

#ifndef CORES_SYNTHETICLOADDIRECTOR_H_
#define CORES_SYNTHETICLOADDIRECTOR_H_

#include <models/nic/NI_Base.h>
#include <models/nic/TrafficDirector.h>
#include <list>
#include <set>

#include "traffic.h"
#include "packet_reply_info.h"
#include "injection.h"

#define _WAIT_DRAIN
//#define _FORCE_DRAIN

class SyntheticLoadDirector: public TrafficDirector {
private:
	bool beingStats = false;
	int i = 0;
	int _cur_pid;
	double start_time;
	int _packet_seq_no;
	int _include_queuing;
//	int flitSizeInBytes;
	int _totalNumHosts;
	double stop_mark_point;
	deque<double> timeDeque;
	vector<vector<int> > _packet_size;
	vector<vector<int> > _packet_size_rate;
	vector<int> _packet_size_max_val;

	vector<int> _use_read_write;

	vector<int> _read_request_size;
	vector<int> _write_request_size;
	vector<int> _read_reply_size;
	vector<int> _write_reply_size;

	vector<TrafficPattern *> _traffic_pattern;
	list<PacketReplyInfo *> _repliesPending;

	vector<int> _class_priority;

	vector<InjectionProcess *> _injection_process;
	vector<double> _write_fraction;

	vector<uint64_t> _qtime;
	
	Configuration *config;

	int _IssuePacket(int cl);
	void _GeneratePacket(int stype, int cl, int time, vector<Flit *> &vec);
	void _GenerateHpcPacket(int stype, int cl, int time, vector<Flit *> &vec);
	void _GenerateHpcNoisePacket(int stype, int cl, int time, vector<Flit *> &vec);
	void shuffleSend();


	int _GetNextPacketSize(int cl) const;
	double _GetAveragePacketSize(int cl) const;

protected:

	NI_Base *& clientRef;
	virtual void initialize() override;

public:

	int next_send_time = -1;  // wzy , HPC task, 2024.3.17
	int single_msg_num = -1;
	int send_msg_num = -1;
	int msg_internal_time = 0;
	int send_msg_num_now = 0;

	// hpc task 2025.1.15
	int hpcFlag = 0;
	int sendMap[20000] = {0};
	int receiveCommuCnt = 0;
	int sendCommuCnt = 0;
	int ringRepeat = 0;



	string placeMent = "random";  // linear   random

	string topo = "dragonfly";  //  cliquefly  dragonfly

	int * vir2phy;
	int * phy2vir;
	int * linear_vir2phy;
	int * linear_phy2vir;

	int *neighbors;
	int total_nodes = 0;
	int hpcTaskNodesGate = 0;
	int neighborCnt = 0;
	int nowSendAddr = -1;
	int nowSendCnt = -1;
	int totalSendCnt = -1;
	void initial_hpc_task(string nowHpcTask);

	void set_hpc_msg(int hpc_next_send_time, int hpc_single_msg_num, int hpc_send_msg_num, int hpc_msg_internal_time);

	SyntheticLoadDirector();
	virtual ~SyntheticLoadDirector();

	virtual std::string getLogPrefix() override;
	virtual void getMsgToSend(vector<Flit *> &vec) override;
	virtual void handlePopMsg(omnetpp::cMessage *msg) override;

	virtual void initial(int source);
	int permDest = 0;
	int initial_lize = 0;

#ifdef _WAIT_DRAIN
	virtual void wait_drain();
#endif
#ifdef _FORCE_DRAIN
	virtual void force_drain();
#endif
};

static int vir2phy_matrix[13770];
static int phy2vir_matrix[13770];