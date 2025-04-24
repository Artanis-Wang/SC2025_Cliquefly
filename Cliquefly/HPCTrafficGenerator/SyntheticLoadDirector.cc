

#include <models/nic/SyntheticLoadDirector.h>
#include <booksim_globals.h>
#include <config_utils.h>
#include <random_utils.h>
#include <platform/common/commons.h>
#include <limits>

//#define _FORCE_DRAIN
#define _WAIT_DRAIN

using namespace omnetpp;


void SyntheticLoadDirector::_GenerateHpcPacket(int stype, int cl, int time, vector<Flit *> &vec) {

	assert(stype != 0);
	Flit::FlitType packet_type = Flit::ANY_TYPE;
	int size = _GetNextPacketSize(cl); //input size
	int pid = _cur_pid++;
	assert(_cur_pid);
	bool record = false;
	int packet_destination = nowSendAddr;   //, clientRef->getCycle()
	int packet_hpc_dest = nowSendAddr;
	if(!(nowSendCnt > 0)){
		cerr<<"!(nowSendCnt > 0)!!! clientref->src = "<<clientRef->getSrcId()<<", nowSendAddr = "<<nowSendAddr<<", nowSendCnt = "<<nowSendCnt<<", totalSendCnt = "<<totalSendCnt<<endl;
		for(int i = 0;i<neighborCnt;i++) cerr<<i<<":"<<neighbors[i]<<" "<<sendMap[neighbors[i]]<<", ";
		cerr<<endl;
	}
	assert(nowSendCnt > 0);
	int hpcSize = 10;
	if(nowSendCnt >= 10){
		hpcSize = 10;
	}
	else{
		hpcSize = nowSendCnt;
	}
	nowSendCnt -= hpcSize;
	totalSendCnt -= hpcSize;
	int packet_hpc_dest_temp = phy2vir[packet_hpc_dest];

	if(totalSendCnt == 0){
		cerr<<"Send all packets ！！ src = "<<clientRef->getSrcId()<<", virSource = "<<phy2vir[clientRef->getSrcId()]<<", group = "<<int(clientRef->getSrcId()/72)<<", time = "<<time<<endl;
	}
	if(nowSendCnt == 0){
		int flag = 0;
		sendMap[nowSendAddr] = 0;
		sendCommuCnt++;
		if(ringRepeat){
			sendMap[nowSendAddr] = clientRef->commu;
			ringRepeat--;
		}

		int begin = 0;
		for(int i = 0;i<neighborCnt;i++){
			if(sendMap[neighbors[(i+begin)%neighborCnt]]){
				nowSendAddr = neighbors[(i+begin)%neighborCnt];
				nowSendCnt = sendMap[neighbors[(i+begin)%neighborCnt]];
				flag = 1;
			}
		}
	}


	int flitSizeInBytes=clientRef->par("flitSizeInBytes");


	bool watch = false;
	if (_use_read_write[cl]) {
		if (stype > 0) {
			if (stype == 1) {
				packet_type = Flit::READ_REQUEST;
				size = _read_request_size[cl];
			} else if (stype == 2) {
				packet_type = Flit::WRITE_REQUEST;
				size = _write_request_size[cl];
			} else {
				ostringstream err;
				err << "Invalid packet type: " << packet_type;
				throw cRuntimeError(err.str().c_str());
			}
		} else {
			PacketReplyInfo* rinfo = _repliesPending.front();
			if (rinfo->type == Flit::READ_REQUEST) { //read reply
				size = _read_reply_size[cl];
				packet_type = Flit::READ_REPLY;
			} else if (rinfo->type == Flit::WRITE_REQUEST) { //write reply
				size = _write_reply_size[cl];
				packet_type = Flit::WRITE_REPLY;
			} else {
				ostringstream err;
				err << "Invalid packet type: " << rinfo->type;
				throw cRuntimeError(err.str().c_str());
			}
			packet_destination = rinfo->source;
			time = rinfo->time;
			record = rinfo->record;
			_repliesPending.pop_front();
			rinfo->Free();
		}
	}

	if ((packet_destination < 0) || (packet_destination >= _totalNumHosts)) {
		ostringstream err;
		err << "Incorrect packet destination " << packet_destination << " for stype " << packet_type;
		throw cRuntimeError(err.str().c_str());
	}

	int _cur_fid = 0;

	for (int i = 0; i < hpcSize; ++i) {
		Flit * f = new Flit();
		f->id = _cur_fid++;
		assert(_cur_fid);
		f->pid = pid;
		f->btime=clientRef->cycle;
		f->watch = false;
		f->src = clientRef->getSrcId();
		f->ctime = time;
		f->record = record;
		f->cl = cl;
		f->stage = clientRef->stage;
		f->setByteLength(flitSizeInBytes);
		f->type = packet_type;

		if (i == 0) { // Head flit
			f->head = true;
			f->dest = packet_hpc_dest;
			f->dtime = omnetpp::getSimulation()->getSimTime().raw();
		} else {
			f->head = false;
			f->dest = packet_hpc_dest;
			f->dtime = omnetpp::getSimulation()->getSimTime().raw();
		}
		switch (clientRef->_pri_type) {
		case NI_Base::class_based:
			f->pri = _class_priority[cl];
			assert(f->pri >= 0);
			break;
		case NI_Base::age_based:
			f->pri = numeric_limits<int>::max() - time;
			assert(f->pri >= 0);
			break;
		case NI_Base::sequence_based:
			f->pri = numeric_limits<int>::max() - _packet_seq_no;
			assert(f->pri >= 0);
			break;
		default:
			f->pri = 0;
		}

		if (i == (hpcSize - 1)) { // Tail flit
			f->tail = true;
		} else {
			f->tail = false;
		}

		f->numFlits = i;
		f->vc = -1;

#ifndef __DELAY__STATS__
		if (f->head)
		{
			if(!timeDeque.empty())
			{
				f->omnet_begin_t = timeDeque.front();
				timeDeque.pop_front();
				if (f->dest == f->src) {
					clientRef->minusSendFlitsNum();
					delete f;
					return;
				}
			} else {
+				f->omnet_begin_t = simTime().dbl();
 			}
		}
#else

#endif
		vec.push_back(f);
	}
}

void SyntheticLoadDirector::_GenerateHpcNoisePacket(int stype, int cl, int time, vector<Flit *> &vec) {//第三百九十五执行
	int source = clientRef->getSrcId();
	RandomSeed(source*source*source+time*time);
	int sendFlag = RandomInt(99);
	if(sendFlag >= 20){    // generate noise at the injection rate of 20%
		return;
	}
	int packet_destination = 0; 
	int packet_hpc_dest = 0;
	int bias = 1;
	RandomSeed(source*source*source+time*time+bias);

	// new
	while (true){
		bias++;
		packet_hpc_dest =  RandomInt(_totalNumHosts-1);
		RandomSeed(source*source*source+time*time+bias);
		if(packet_hpc_dest >= hpcTaskNodesGate){
			break;
		}
	}

	packet_hpc_dest = vir2phy[packet_hpc_dest];

	assert(stype != 0);
	Flit::FlitType packet_type = Flit::ANY_TYPE;
	int size = _GetNextPacketSize(cl); //input size
	int pid = _cur_pid++;
	assert(_cur_pid);
	bool record = false;

	int flitSizeInBytes=clientRef->par("flitSizeInBytes");

	bool watch = false;
	if (_use_read_write[cl]) {
		if (stype > 0) {
			if (stype == 1) {
				packet_type = Flit::READ_REQUEST;
				size = _read_request_size[cl];
			} else if (stype == 2) {
				packet_type = Flit::WRITE_REQUEST;
				size = _write_request_size[cl];
			} else {
				ostringstream err;
				err << "Invalid packet type: " << packet_type;
				throw cRuntimeError(err.str().c_str());
			}
		} else {
			PacketReplyInfo* rinfo = _repliesPending.front();
			if (rinfo->type == Flit::READ_REQUEST) { //read reply
				size = _read_reply_size[cl];
				packet_type = Flit::READ_REPLY;
			} else if (rinfo->type == Flit::WRITE_REQUEST) { //write reply
				size = _write_reply_size[cl];
				packet_type = Flit::WRITE_REPLY;
			} else {
				ostringstream err;
				err << "Invalid packet type: " << rinfo->type;
				throw cRuntimeError(err.str().c_str());
			}
			packet_destination = rinfo->source;
			time = rinfo->time;
			record = rinfo->record;
			_repliesPending.pop_front();
			rinfo->Free();
		}
	}

	if ((packet_destination < 0) || (packet_destination >= _totalNumHosts)) {
		ostringstream err;
		err << "Incorrect packet destination " << packet_destination << " for stype " << packet_type;
		throw cRuntimeError(err.str().c_str());
	}

	int _cur_fid = 0;
	int hpcSize = 1;
	for (int i = 0; i < hpcSize; ++i) {
		Flit * f = new Flit();
		f->id = _cur_fid++;
		assert(_cur_fid);
		f->pid = pid;
		f->btime=clientRef->cycle;
		f->watch = false;
		f->src = clientRef->getSrcId();
		f->ctime = time;
		f->record = record;
		f->cl = cl;
		f->stage = clientRef->stage;
		f->setByteLength(flitSizeInBytes);
		f->type = packet_type;

		if (i == 0) { // Head flit
			f->head = true;
			f->dest = packet_hpc_dest;
			f->dtime = omnetpp::getSimulation()->getSimTime().raw();
		} else {
			f->head = false;
			f->dest = packet_hpc_dest;
			f->dtime = omnetpp::getSimulation()->getSimTime().raw();
		}
		switch (clientRef->_pri_type) {
		case NI_Base::class_based:
			f->pri = _class_priority[cl];
			assert(f->pri >= 0);
			break;
		case NI_Base::age_based:
			f->pri = numeric_limits<int>::max() - time;
			assert(f->pri >= 0);
			break;
		case NI_Base::sequence_based:
			f->pri = numeric_limits<int>::max() - _packet_seq_no;
			assert(f->pri >= 0);
			break;
		default:
			f->pri = 0;
		}
		
		// if (i == (size - 1)) { // Tail flit
		if (i == (hpcSize - 1)) { // Tail flit
			f->tail = true;
		} else {
			f->tail = false;
		}

		f->numFlits = i;
		f->vc = -1;

#ifndef __DELAY__STATS__
		if (f->head)
		{
			if(!timeDeque.empty())
			{
				f->omnet_begin_t = timeDeque.front();
				timeDeque.pop_front();
				if (f->dest == f->src) {
					clientRef->minusSendFlitsNum();
					delete f;
					return;
				}
			} else {
+				f->omnet_begin_t = simTime().dbl();
 			}
		}
#else

#endif
		vec.push_back(f);
	}
}

void SyntheticLoadDirector::_GeneratePacket(int stype, int cl, int time, vector<Flit *> &vec) {

	assert(stype != 0);
	Flit::FlitType packet_type = Flit::ANY_TYPE;
	int size = _GetNextPacketSize(cl); //input size
	int pid = _cur_pid++;
	assert(_cur_pid);
	bool record = false;
	int packet_destination = _traffic_pattern[cl]->dest(clientRef->getSrcId(),time);   //, clientRef->getCycle()

	int flitSizeInBytes=clientRef->par("flitSizeInBytes");
	if(beingStats){
		record = true;
	}
	bool watch = false;
	if (_use_read_write[cl]) {
		if (stype > 0) {
			if (stype == 1) {
				packet_type = Flit::READ_REQUEST;
				size = _read_request_size[cl];
			} else if (stype == 2) {
				packet_type = Flit::WRITE_REQUEST;
				size = _write_request_size[cl];
			} else {
				ostringstream err;
				err << "Invalid packet type: " << packet_type;
				throw cRuntimeError(err.str().c_str());
			}
		} else {
			PacketReplyInfo* rinfo = _repliesPending.front();
			if (rinfo->type == Flit::READ_REQUEST) { //read reply
				size = _read_reply_size[cl];
				packet_type = Flit::READ_REPLY;
			} else if (rinfo->type == Flit::WRITE_REQUEST) { //write reply
				size = _write_reply_size[cl];
				packet_type = Flit::WRITE_REPLY;
			} else {
				ostringstream err;
				err << "Invalid packet type: " << rinfo->type;
				throw cRuntimeError(err.str().c_str());
			}
			packet_destination = rinfo->source;
			time = rinfo->time;
			record = rinfo->record;
			_repliesPending.pop_front();
			rinfo->Free();
		}
	}

	if ((packet_destination < 0) || (packet_destination >= _totalNumHosts)) {
		ostringstream err;
		err << "Incorrect packet destination " << packet_destination << " for stype " << packet_type;
		throw cRuntimeError(err.str().c_str());
	}

	if (watch) {
		*gWatchOut << simTime() << " | " << "node" << clientRef->getSrcId() << " | " << "Enqueuing packet " << pid << " at time " << time << "." << endl;
	}

	int _cur_fid = 0;

	for (int i = 0; i < size; ++i) {
		Flit * f = new Flit();
		f->id = _cur_fid++;
		assert(_cur_fid);
		f->pid = pid;
		f->btime=clientRef->cycle;
		f->watch = false;
		f->src = clientRef->getSrcId();
		f->ctime = time;
		f->record = record;
		f->cl = cl;
		f->stage = clientRef->stage;
		f->setByteLength(flitSizeInBytes);
		if (gTrace) {
			cout << "New Flit " << f->src << endl;
		}
		f->type = packet_type;

		if (i == 0) { // Head flit
			f->head = true;
			f->dest = packet_destination;
			f->dtime = omnetpp::getSimulation()->getSimTime().raw();
		} else {
			f->head = false;
			f->dest = packet_destination;
			f->dtime = omnetpp::getSimulation()->getSimTime().raw();
		}
		switch (clientRef->_pri_type) {
		case NI_Base::class_based:
			f->pri = _class_priority[cl];
			assert(f->pri >= 0);
			break;
		case NI_Base::age_based:
			f->pri = numeric_limits<int>::max() - time;
			assert(f->pri >= 0);
			break;
		case NI_Base::sequence_based:
			f->pri = numeric_limits<int>::max() - _packet_seq_no;
			assert(f->pri >= 0);
			break;
		default:
			f->pri = 0;
		}
		if (i == (size - 1)) { // Tail flit
			f->tail = true;
		} else {
			f->tail = false;
		}

		f->numFlits = i;
		f->vc = -1;

		if (f->watch) {
			*gWatchOut << simTime() << " | " << "node" << clientRef->getSrcId() << " | " << "Enqueuing flit " << f->id << " (packet " << f->pid << ") at time " << time << "." << endl;
		}
#ifndef __DELAY__STATS__
		if (f->head)
		{
			if(!timeDeque.empty())
			{
				f->omnet_begin_t = timeDeque.front();
				timeDeque.pop_front();
				if (f->dest == f->src) {
					clientRef->minusSendFlitsNum();
					delete f;
					return;
				}
			} else {
+				f->omnet_begin_t = simTime().dbl();
 			}
		}
#else

		f->delayType = 0;
		if (f->record) {
			f->delayType = 1;
			f->omnet_begin_t = simTime().dbl();


			// if(!initial_lize){
			// 	initial(f->src);
			// }
			// if(permDest == 1){
			// 	f->res_isV6 = 1000;
			// 	clientRef->addSendFlitsNum();
			// }
			clientRef->addSendFlitsNum();


			// if(f->src % 6 == 0) clientRef->addSendFlitsNum();   // clientRef->addSendFlitsNum();  wzy


	/*	if (f->dest == f->src) {
				clientRef->minusSendFlitsNum();
				delete f;
				return;
			}*/

		}
#endif
		vec.push_back(f);
	}
}

int SyntheticLoadDirector::_GetNextPacketSize(int cl) const {
	assert(cl >= 0 && cl < clientRef->_classes);

	vector<int> const & psize = _packet_size[cl];
	int sizes = psize.size();


	if (sizes == 1) {
		return psize[0];
	}

	vector<int> const & prate = _packet_size_rate[cl];
	int max_val = _packet_size_max_val[cl];

	int pct = RandomInt(max_val);

	for (int i = 0; i < (sizes - 1); ++i) {
		int const limit = prate[i];
		if (limit > pct) {
			return psize[i];
		} else {
			pct -= limit;
		}
	}
	assert(prate.back() > pct);
	return psize.back();
}

int SyntheticLoadDirector::_IssuePacket(int cl) {  // 决定是否产生数据包
	int result = 0;
	if (_use_read_write[cl]) { //use read and write
		if (!_repliesPending.empty()) {
			if (_repliesPending.front()->time <= clientRef->getCycle()) {
				result = -1;
			}
		} else {
			if (_injection_process[cl]->test(clientRef->getSrcId())) {//,clientRef->getCycle()
				result = (RandomFloat() < _write_fraction[cl]) ? 2 : 1;
			}
		}
	} else {
		result = _injection_process[cl]->test(clientRef->getSrcId(),_qtime[cl]) ? 1 : 0;   //,clientRef->getCycle()
	}

	if (result != 0) {
		_packet_seq_no++;
	}
	return result;
}

double SyntheticLoadDirector::_GetAveragePacketSize(int cl) const {
	vector<int> const & psize = _packet_size[cl];
	int sizes = psize.size();
	if (sizes == 1) {
		return (double) psize[0];
	}
	vector<int> const & prate = _packet_size_rate[cl];
	int sum = 0;
	for (int i = 0; i < sizes; ++i) {
		sum += psize[i] * prate[i];
	}
	return (double) sum / (double) (_packet_size_max_val[cl] + 1);
}

void SyntheticLoadDirector::initialize() {

	_cur_pid = 0;
	stop_mark_point = clientRef->getStopMarkPoint();
	config = Configuration::GetRouterConfig(clientRef);
	assert(config != nullptr);
	int _classes = clientRef->_classes;

	_qtime.resize(_classes, 0);

	_include_queuing = config->GetInt("include_queuing");

	_use_read_write = config->GetIntArray("use_read_write");
	if (_use_read_write.empty()) {
		_use_read_write.push_back(config->GetInt("use_read_write"));
	}
	_use_read_write.resize(_classes, _use_read_write.back());

	_write_fraction = config->GetFloatArray("write_fraction");
	if (_write_fraction.empty()) {
		_write_fraction.push_back(config->GetFloat("write_fraction"));
	}
	_write_fraction.resize(_classes, _write_fraction.back());

	_read_request_size = config->GetIntArray("read_request_size");
	if (_read_request_size.empty()) {
		_read_request_size.push_back(config->GetInt("read_request_size"));
	}
	_read_request_size.resize(_classes, _read_request_size.back());

	_read_reply_size = config->GetIntArray("read_reply_size");
	if (_read_reply_size.empty()) {
		_read_reply_size.push_back(config->GetInt("read_reply_size"));
	}
	_read_reply_size.resize(_classes, _read_reply_size.back());

	_write_request_size = config->GetIntArray("write_request_size");
	if (_write_request_size.empty()) {
		_write_request_size.push_back(config->GetInt("write_request_size"));
	}
	_write_request_size.resize(_classes, _write_request_size.back());

	_write_reply_size = config->GetIntArray("write_reply_size");
	if (_write_reply_size.empty()) {
		_write_reply_size.push_back(config->GetInt("write_reply_size"));
	}
	_write_reply_size.resize(_classes, _write_reply_size.back());

	string packet_size_str = config->GetStr("packet_size");
	if (packet_size_str.empty()) {
		_packet_size.push_back(vector<int>(1, config->GetInt("packet_size")));
	} else {
		vector<string> packet_size_strings = tokenize_str(packet_size_str);
		for (size_t i = 0; i < packet_size_strings.size(); ++i) {
			_packet_size.push_back(tokenize_int(packet_size_strings[i]));
		}
	}
	_packet_size.resize(_classes, _packet_size.back());

	string packet_size_rate_str = config->GetStr("packet_size_rate");
	if (packet_size_rate_str.empty()) {
		int rate = config->GetInt("packet_size_rate");
		assert(rate >= 0);
		for (int c = 0; c < _classes; ++c) {
			int size = _packet_size[c].size();
			_packet_size_rate.push_back(vector<int>(size, rate));
			_packet_size_max_val.push_back(size * rate - 1);
		}
	} else {
		vector<string> packet_size_rate_strings = tokenize_str(packet_size_rate_str);
		packet_size_rate_strings.resize(_classes, packet_size_rate_strings.back());
		for (int c = 0; c < _classes; ++c) {
			vector<int> rates = tokenize_int(packet_size_rate_strings[c]);
			rates.resize(_packet_size[c].size(), rates.back());
			_packet_size_rate.push_back(rates);
			int size = rates.size();
			int max_val = -1;
			for (int i = 0; i < size; ++i) {
				int rate = rates[i];
				assert(rate >= 0);
				max_val += rate;
			}
			_packet_size_max_val.push_back(max_val);
		}
	}

	for (int c = 0; c < _classes; ++c) {
		if (_use_read_write[c]) {
			_packet_size[c] = vector<int>(1, (_read_request_size[c] + _read_reply_size[c] + _write_request_size[c] + _write_reply_size[c]) / 2);
			_packet_size_rate[c] = vector<int>(1, 1);
			_packet_size_max_val[c] = 0;
		}
	}

	vector<double> _load;
	_load.push_back(clientRef->get_inj_rate());
	if (_load.empty()) {
		throw cRuntimeError("Injection rate is not set yet!!!\n");
	}
	_load.resize(_classes, _load.back());

	if (config->GetInt("injection_rate_uses_flits")) {
		for (int c = 0; c < _classes; ++c)
			_load[c] /= _GetAveragePacketSize(c);
	}

	vector<string> _traffic;
	_traffic.push_back(clientRef->par("traffic_pattern")); //wzh modify
	_traffic.resize(_classes, _traffic.back());

	_class_priority = config->GetIntArray("class_priority");
	if (_class_priority.empty()) {
		_class_priority.push_back(config->GetInt("class_priority"));
	}
	_class_priority.resize(_classes, _class_priority.back());

	vector<string> injection_process = config->GetStrArray("injection_process");
	injection_process.resize(_classes, injection_process.back());


	_totalNumHosts = getSimulation()->getSystemModule()->par("totalNumNetworkNode");

	_injection_process.resize(_classes);
	_traffic_pattern.resize(_classes);

	for (int c = 0; c < _classes; ++c) {
		_traffic_pattern[c] = TrafficPattern::New(_traffic[c], _totalNumHosts, config);
		_injection_process[c] = InjectionProcess::New(injection_process[c], _totalNumHosts, _load[c], config);
		_traffic_pattern[c]->reset_srand();
	}

	string traffic_p = _traffic[0];

	if (traffic_p == "halo2D" || traffic_p == "ringPattern" || traffic_p == "a2a_2d"){
		initial_hpc_task(traffic_p);
	}

}

std::string SyntheticLoadDirector::getLogPrefix() {
	string prefix = "syntheticload_";
	prefix.append(client->par("traffic_pattern").str());
	return prefix;
}

void SyntheticLoadDirector::shuffleSend(){
	int source = clientRef->getSrcId()+10;
	int bias = 0;
	int biasBegin = source * source;
	for(int i = 0;i<neighborCnt;i++){
		RandomSeed(biasBegin + bias);
		int AA = RandomInt(neighborCnt-1);
		bias++;
		RandomSeed(biasBegin + bias);
		int BB = RandomInt(neighborCnt-1);
		bias++;
		int tempA = neighbors[AA];
		neighbors[AA] = neighbors[BB];
		neighbors[BB] = tempA;
	}

}
void SyntheticLoadDirector::initial_hpc_task(string nowHpcTask){
	hpcFlag = 1;
	int commu = 0;
	int maxZ;
	receiveCommuCnt = 0;
	sendCommuCnt = 0;

	int hpcGroups = 21;
	int groupSize = gN*gK;
	hpcTaskNodesGate = hpcGroups * groupSize;
	commu =  config->GetInt("communication_num");

	if(topo == "dragonfly"){
		total_nodes = 13284;
		vir2phy = vir2phy_matrix;
		phy2vir = phy2vir_matrix;
	}
	else{
		total_nodes = 13770;
		vir2phy = vir2phy_matrix;
		phy2vir = phy2vir_matrix;
	}

	if(nowHpcTask == "ringPattern"){
		
		sendCommuCnt = 0;
		clientRef->receiveCommuCnt = 0;
		int numNeighbors = 1;
		neighborCnt = numNeighbors;
		int nodes = clientRef->getSrcId();
		nodes = phy2vir[nodes];
		
		int ringSize = 72;
		if(hpcTaskNodesGate >= 3000) ringSize = 256;
		hpcTaskNodesGate -= (hpcTaskNodesGate % ringSize);
		if(nodes >= hpcTaskNodesGate){
			clientRef->totalRcvCnt = 0;
			clientRef->hpcFlag = 1;
			totalSendCnt = -1000;
			return;
		}
		int ringId = nodes / ringSize;
		totalSendCnt = commu * ringSize;
		ringRepeat = ringSize;
		clientRef->ringRepeat = ringSize;
		neighbors = new int[numNeighbors];
		neighbors[0] = ringId * ringSize + ((nodes + 1) % ringSize);
		assert(neighbors[0] < hpcTaskNodesGate);
		neighbors[0] = vir2phy[neighbors[0]];
		int ni_receive = ringId * ringSize + ((ringSize + nodes - 1) % ringSize);
		clientRef->receiveMap[vir2phy[ni_receive]] += commu; 
		for(int i = 0;i<numNeighbors;i++){
			sendMap[neighbors[i]] += commu;
		}

		nowSendAddr = neighbors[0];
		nowSendCnt = sendMap[neighbors[0]];
		clientRef->totalRcvCnt = ringSize*commu;
		clientRef->commu = commu;
		clientRef->hpcFlag = 1;
	}
	else if(nowHpcTask == "a2a_2d"){
		int nodes = clientRef->getSrcId();
		nodes = phy2vir[nodes];
		if(nodes >= hpcTaskNodesGate){
			clientRef->totalRcvCnt = 0;
			clientRef->hpcFlag = 1;
			totalSendCnt = -1000;
			return;
		}
		sendCommuCnt = -1000;
		clientRef->receiveCommuCnt = 0;
		int maxX = groupSize;
		int maxY = hpcTaskNodesGate / groupSize;
		int groupId = nodes / groupSize;
		int numNeighbors = maxX + maxY - 2;
		neighborCnt = numNeighbors;
		neighbors = new int[numNeighbors];
		
		int groupBegin = groupId * groupSize;
		int cnt = 0;
		for(int i = 0; i<groupSize;i++){
			if(i + groupBegin != nodes){
				neighbors[cnt] = i + groupBegin;
				cnt++;
			}
		}
		for(int i = 0;i<maxY;i++){
			if(i*groupSize + (nodes % groupSize) != nodes){
				neighbors[cnt] = i*groupSize + (nodes % groupSize);
				cnt++;
			}
		}
		assert(cnt == numNeighbors);
		for(int i = 0;i<cnt;i++){
			neighbors[i] = vir2phy[neighbors[i]];
			clientRef->receiveMap[neighbors[i]] = commu;
		}
		shuffleSend();
		for(int i = 0;i<numNeighbors;i++){
			sendMap[neighbors[i]] += commu;
		}
		nowSendAddr = neighbors[0];
		nowSendCnt = sendMap[neighbors[0]];
		clientRef->commu = commu;
		clientRef->hpcFlag = 1;
		clientRef->totalRcvCnt = commu * cnt;
		totalSendCnt = cnt * commu;
	}
	else if (nowHpcTask == "halo2D"){


		totalSendCnt = commu * 4;
		int numNeighbors = 4;
		neighborCnt = numNeighbors;
		int nodes = clientRef->getSrcId();
		nodes = phy2vir[nodes];

		hpcTaskNodesGate -= (hpcTaskNodesGate % groupSize);
		if(nodes >= hpcTaskNodesGate){
			clientRef->totalRcvCnt = 0;
			clientRef->hpcFlag = 1;
			totalSendCnt = -1000;
			return;
		}
		int maxX = gN*gK;
		int maxY = hpcTaskNodesGate / groupSize;
		// int maxY = gN*gN+1;
		int myX = nodes % maxX;
		int myY = (nodes / maxX) % maxY;

		neighbors = new int[numNeighbors];
		neighbors[0] = (((myX-1) + maxX) % maxX) + myY*groupSize                        ;
		neighbors[1] = ((myX+1) % maxX)          + myY*groupSize                        ;
		neighbors[2] = myX                       + (((myY-1) + maxY) % maxY) * groupSize;
		neighbors[3] = myX                       + (((myY+1)) % maxY) * groupSize       ;
		for(int i = 0;i<numNeighbors;i++){
			assert(neighbors[i] < total_nodes);
			assert(vir2phy[neighbors[i]] < total_nodes);
			clientRef->receiveMap[vir2phy[neighbors[i]]] += commu; 
		}
		shuffleSend();
		for(int i = 0;i<numNeighbors;i++) neighbors[i] = vir2phy[neighbors[i]];

		for(int i = 0;i<numNeighbors;i++){
			sendMap[neighbors[i]] += commu;
		}
		nowSendAddr = neighbors[0];
		nowSendCnt = sendMap[neighbors[0]];
		clientRef->totalRcvCnt = numNeighbors*commu;
		clientRef->hpcFlag = 1;
	}
	else{
		cerr<<"wrong hpc pattern"<<endl;
		assert(0);
	}
}
void SyntheticLoadDirector::set_hpc_msg(int hpc_next_send_time, int hpc_single_msg_num, int hpc_send_msg_num, int hpc_msg_internal_time){   // wzy , HPC task, 2024.3.17
	next_send_time = hpc_next_send_time;
	single_msg_num = hpc_single_msg_num;
	send_msg_num = hpc_send_msg_num;
	msg_internal_time = hpc_msg_internal_time;
}

void SyntheticLoadDirector::getMsgToSend(vector<Flit*>& vec) {
#ifdef __DELAY__STATS__

	beingStats = clientRef->isRunnig() && clientRef->isNetWarmUpEnd() && (simTime().dbl() < stop_mark_point);  // 执行这里

#else
	beingStats = clientRef->isRunnig() && clientRef->isNetWarmUpEnd();
#endif

	for (int c = 0; c < clientRef->_classes; ++c) {
		if (clientRef->_partial_packets[c].empty()) {
			bool generated = false;
			while (!generated && (_qtime[c] <= clientRef->getCycle())) {
				if(hpcFlag){
					generated = true;
					if(totalSendCnt>0 && sendCommuCnt <= clientRef->receiveCommuCnt){
						_GenerateHpcPacket(1, c, _include_queuing == 1 ? _qtime[c] : clientRef->getCycle(), vec);
					}
					else if(totalSendCnt == -1000){  // generate noise
						_GenerateHpcNoisePacket(1, c, _include_queuing == 1 ? _qtime[c] : clientRef->getCycle(), vec);
					}
					++_qtime[c];
				}
				else{
					int stype =_IssuePacket(0);
					if (stype != 0) { 
						_GeneratePacket(stype, c, _include_queuing == 1 ? _qtime[c] : clientRef->getCycle(), vec);
						generated = true;
					}
					if (!_use_read_write[c] || (stype >= 0)) {
						++_qtime[c];

					}
				}

			}
		}
	}
}

#ifdef _WAIT_DRAIN
void SyntheticLoadDirector::wait_drain() {
	for (auto iter : _injection_process)
		delete iter;
	vector<double> _load;
	// i = 0;
	int _classes = clientRef->_classes;
	clientRef->cycle = 0;
	_qtime[0] = 0;
	_load.push_back(clientRef->get_inj_rate());
	stop_mark_point = clientRef->getStopMarkPoint();
	if (_load.empty()) {
		throw cRuntimeError("Injection rate is not set yet!!!\n");
	}

	_load.resize(_classes, _load.back());
	_qtime.resize(_classes, 0);
		if(config->GetInt("injection_rate_uses_flits"))//这里
	{
		for(int c=0;c<_classes;++c)
		{
			_load[c]/=_GetAveragePacketSize(c);
		}
	}

	vector<string> injection_process = config->GetStrArray("injection_process");
	injection_process.resize(_classes, injection_process.back());

	_injection_process.resize(_classes);
	start_time = simTime().dbl();
	for (int c = 0; c < _classes; ++c) {
		_injection_process[c] = InjectionProcess::New(injection_process[c], _totalNumHosts, _load[c], config);
		_traffic_pattern[c]->reset_srand();
	}
}
#endif
