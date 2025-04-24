
// ------------------------------Dragonfly--------------------------------------
//   This function is used to get the minimal output port according to the routing table
int min_df_ur_global_port(const Router *r, const Flit *f, int dest,  int grp_ID, int dest_grp_ID, int cur, int dest_router, int _grp_num_routers,  int nodeGateSize, int numHosts){
	int out_port = -1;
	if(grp_ID == dest_grp_ID){
		if (cur == dest_router){
			out_port = nodeGateSize + (dest % numHosts);
		} 
		else{
			int pos = dest_router % _grp_num_routers;
			out_port = r->oRouter->intra_group_port[pos];
		}
	}else{
		vector<int> temp = r->oRouter->inter_group_port[dest_grp_ID];
		RandomSeed(r->GetSimTime() + f->pid + f->src * f->dest + r->GetSimTime());
		int randRoute = RandomInt(temp.size() - 1);
		out_port = temp[randRoute];
	}
	return out_port;
}

// This function is used to generate the midrouter of dragonfly
int choose_mid_router_ur_df_global(const Router *r, const Flit *f, const IQRouter *rr, int dest_grp_ID, int grp_ID, int _grp_num_routers = 12, int total_grp = 37, int total_routers = 444){
	int intm_router_ID;
	_grp_num_routers = omnetpp::getSimulation()->getSystemModule()->par("gK");
	total_grp = 1 + _grp_num_routers * _grp_num_routers / 4;
	total_routers = _grp_num_routers * total_grp;
	if(0){
		RandomSeed(f->btime + r->GetSimTime() + f->pid + rr->oRouter->getSimulation()->getSimTime().raw());
		intm_router_ID = RandomInt(total_routers - 1);  // choose a midrouter which can be in the source group or the destination group, however, this is not used.
	}
	else{
		int bias = 0;
		int intm_grp_ID = 0;
		while (1)
		{
			RandomSeed(f->btime + r->GetSimTime() + f->pid + rr->oRouter->getSimulation()->getSimTime().raw() + f->src * f->dest +bias);
			intm_router_ID = RandomInt(total_routers - 1); 
			intm_grp_ID = intm_router_ID / _grp_num_routers;
			if (intm_grp_ID != dest_grp_ID && intm_grp_ID != grp_ID) // choose a midrouter which is not in the source group and the destination group
			{
				break;
			}
			else
			{
				bias++;
			}	
		}
	}
	return intm_router_ID;
}

// PAR routing of Dragonfly
void choosePort_par_df_ur_global(const Router *r, const Flit *f, int, OutputSet *outputs, bool inject){
	const IQRouter *rr = dynamic_cast<const IQRouter *>(r); 

	int cur = getOmnetRoute(r)->getParentModule()->par("id");

	int _grp_num_routers = omnetpp::getSimulation()->getSystemModule()->par("gK");
	int gN = omnetpp::getSimulation()->getSystemModule()->par("gN");
	int numHosts = omnetpp::getSimulation()->getSystemModule()->par("numHosts");
	int in_channel = f->getArrivalGate()->getIndex();

	int _grp_num_nodes = _grp_num_routers * gN;
	int grp_ID = (int)(cur / _grp_num_routers);
	int nodeGateSize = _grp_num_routers + gN - 1;
	int dest_grp_ID = int(f->dest / _grp_num_nodes);
	int out_port = -1;
	int total_grp = 1 + _grp_num_routers * gN / 2;
	int total_routers=_grp_num_routers * total_grp;
	int dest_router = f->dest / gN;
	int midRouter = -1;
	if(in_channel >= nodeGateSize){  // this means the current packet is during inject, and the current router is the source router
		
		// intra-group adaptive routing
		if(dest_grp_ID == grp_ID){
			int min_router_output = min_df_ur_global_port(r, f, f->dest, grp_ID, dest_grp_ID, cur, dest_router, _grp_num_routers, nodeGateSize, numHosts);;
			int min_queue_size = max(r->GetUsedCredit(min_router_output), 0);
			int bias = 0;
			int routerBegin = _grp_num_routers * grp_ID;
			while (1){
				RandomSeed(f->btime + r->GetSimTime() + f->pid + rr->oRouter->getSimulation()->getSimTime().raw() + f->src * f->dest +bias);
				midRouter = routerBegin + RandomInt(_grp_num_routers - 1);
				if (midRouter != cur && midRouter != dest_router){
					break;
				}
				else{
					bias++;
				}
			}
			f->intm = midRouter * numHosts;
			int nonmin_router_output = min_df_ur_global_port(r, f, f->intm, grp_ID, int(midRouter/_grp_num_routers), cur, midRouter, _grp_num_routers, nodeGateSize, numHosts);
			int nonmin_queue_size = max(r->GetUsedCredit(nonmin_router_output), 0);
			if(min_queue_size <= nonmin_queue_size * 2){
				f->min_or_not = 1;
				f->ph = 4;
			}
			else{
				f->min_or_not = 0;
				f->ph = 0;
			}
		}
		else{
			midRouter = choose_mid_router_ur_df_global(r, f, rr, dest_grp_ID, grp_ID);
			f->intm = midRouter * numHosts;
			int min_router_output = min_df_ur_global_port(r, f, f->dest, grp_ID, dest_grp_ID, cur, dest_router, _grp_num_routers, nodeGateSize, numHosts);;
			int min_queue_size = max(r->GetUsedCredit(min_router_output), 0);
			int nonmin_router_output = min_df_ur_global_port(r, f, f->intm, grp_ID, int(midRouter/_grp_num_routers), cur, midRouter, _grp_num_routers, nodeGateSize, numHosts);
			int nonmin_queue_size = max(r->GetUsedCredit(nonmin_router_output), 0);
			if(min_queue_size <= nonmin_queue_size * 2){
				f->min_or_not = 1;
				f->ph = 0;
			}
			else{
				f->min_or_not = 0;
				f->ph = 1;
			}
		}
	}

	// PAR
	if(in_channel < nodeGateSize && f->min_or_not && dest_grp_ID != grp_ID){
		assert(f->ph == 0);
		midRouter = choose_mid_router_ur_df_global(r, f, rr, dest_grp_ID, grp_ID);
		f->intm = midRouter * numHosts;
		int min_router_output = min_df_ur_global_port(r, f, f->dest, grp_ID, dest_grp_ID, cur, dest_router, _grp_num_routers, nodeGateSize, numHosts);
		assert(min_router_output >= gK-1 && min_router_output < gK-1+gN);
		int min_queue_size = max(r->GetUsedCredit(min_router_output), 0);
		int nonmin_router_output = min_df_ur_global_port(r, f, f->intm, grp_ID, int(midRouter/_grp_num_routers), cur, midRouter, _grp_num_routers, nodeGateSize, numHosts);
		int nonmin_queue_size = max(r->GetUsedCredit(nonmin_router_output), 0);
		if(min_queue_size <= nonmin_queue_size * 2){
			f->min_or_not = 1;
			f->ph = 3;
		}
		else{
			f->min_or_not = 0;
			f->ph = 1;
		}
	}

	int intm_rID = int(f->intm / numHosts);
	int intm_grp = int(intm_rID / _grp_num_routers);
	if (f->ph <= 2 && !f->min_or_not)
	{
		if (grp_ID == intm_grp && cur != intm_rID)
		{
				f->ph = 2;
		}
		if (cur == intm_rID)
		{
				f->ph = 3;
		}
	}

	if(grp_ID == dest_grp_ID && f->ph >= 3){ 
		f->ph = 4;
	}

	if (f->min_or_not){
		out_port = min_df_ur_global_port(r, f, f->dest, grp_ID, dest_grp_ID, cur, dest_router, _grp_num_routers, nodeGateSize, numHosts);
	}
	else{
		if (f->ph <= 2){
			out_port = min_df_ur_global_port(r, f, f->intm, grp_ID, intm_grp, cur, intm_rID, _grp_num_routers, nodeGateSize, numHosts);
		}
		else{
			out_port = min_df_ur_global_port(r, f, f->dest, grp_ID, dest_grp_ID, cur, dest_router, _grp_num_routers, nodeGateSize, numHosts);
		}
	}

	assert(f->hops <= 7);
	int vcBegin = f->ph;
	int vcEnd = f->ph;


	if(out_port >= nodeGateSize){
		vcBegin = 0;
		vcEnd = gNumVCs - 1;
	}

	outputs->Clear();
	outputs->AddRange(out_port, vcBegin, vcEnd, 0);
}

// BIAS routing for dragonfly
void choosePort_BIAS_df_ur_global(const Router *r, const Flit *f, int, OutputSet *outputs, bool inject){
	const IQRouter *rr = dynamic_cast<const IQRouter *>(r); // lanbinyan add
	// const IQRouter* rr = dynamic_cast<const IQRouter*>(r);//lanbinyan add
	int cur = getOmnetRoute(r)->getParentModule()->par("id");

	int _grp_num_routers = omnetpp::getSimulation()->getSystemModule()->par("gK");
	int gN = omnetpp::getSimulation()->getSystemModule()->par("gN");
	int numHosts = omnetpp::getSimulation()->getSystemModule()->par("numHosts");
	int in_channel = f->getArrivalGate()->getIndex();

	// int in_channel = f->getArrivalGate()->getIndex();
	int _grp_num_nodes = _grp_num_routers * gN;
	int grp_ID = (int)(cur / _grp_num_routers);
	int nodeGateSize = _grp_num_routers + gN - 1;
	int dest_grp_ID = int(f->dest / _grp_num_nodes);
	int out_port = -1;
	int total_grp = 1 + _grp_num_routers * gN / 2;
	int total_routers=_grp_num_routers * total_grp;
	int dest_router = f->dest / gN;
	int midRouter = -1;

	if(in_channel >= nodeGateSize){


		if(dest_grp_ID == grp_ID){
			// f->ph = 3;
			// f->min_or_not = true;
			int min_router_output = min_df_new_global_port(r, f, f->dest, grp_ID, dest_grp_ID, cur, dest_router, _grp_num_routers, nodeGateSize, numHosts);;
			int bias = 0;
			int routerBegin = _grp_num_routers * grp_ID;
			while (1){
				RandomSeed(f->btime + r->GetSimTime() + f->pid + rr->oRouter->getSimulation()->getSimTime().raw() + f->src * f->dest +bias);
				midRouter = routerBegin + RandomInt(_grp_num_routers - 1);
				if (midRouter != cur && midRouter != dest_router){
					break;
				}
				else{
					bias++;
				}
			}
			f->intm = midRouter * numHosts;
			int nonmin_router_output = min_df_new_global_port(r, f, f->intm, grp_ID, int(midRouter/_grp_num_routers), cur, midRouter, _grp_num_routers, nodeGateSize, numHosts);
			RandomSeed(f->btime + r->GetSimTime() + f->pid + rr->oRouter->getSimulation()->getSimTime().raw() + f->src * f->dest +100);
			
			int randomP = RandomInt(9999);
			double rate = 10000 * (1/total_grp);
			if(randomP < rate){ 
				f->min_or_not = 1;
				f->ph = 3;
			}
			else{
				f->min_or_not = 0;
				f->ph = 0;
			}
		}
		else{
			midRouter = choose_mid_router_ur_df_global(r, f, rr, dest_grp_ID, grp_ID);
			f->intm = midRouter * numHosts;
			int min_router_output = min_df_ur_global_port(r, f, f->dest, grp_ID, dest_grp_ID, cur, dest_router, _grp_num_routers, nodeGateSize, numHosts);;
			int min_queue_size = max(r->GetUsedCredit(min_router_output), 0);
			int nonmin_router_output = min_df_ur_global_port(r, f, f->intm, grp_ID, int(midRouter/_grp_num_routers), cur, midRouter, _grp_num_routers, nodeGateSize, numHosts);
			int nonmin_queue_size = max(r->GetUsedCredit(nonmin_router_output), 0);
			RandomSeed(f->btime + r->GetSimTime() + f->pid + rr->oRouter->getSimulation()->getSimTime().raw() + f->src * f->dest +100);
			
			int randomP = RandomInt(9999);
			double rate = 10000 * (1/total_grp);
			if(randomP < rate){ 
				f->min_or_not = 1;
				f->ph = 2;
			}
			else{
				f->min_or_not = 0;
				f->ph = 0;
			}
		}
	}
	int intm_rID = int(f->intm / numHosts);
	int intm_grp = int(intm_rID / _grp_num_routers);
	if (f->ph == 0 || f->ph == 1)
	{
		if (grp_ID == intm_grp && cur != intm_rID)
		{
				f->ph = 1;
		}
		if (cur == intm_rID)
		{
				f->ph = 2;
		}
	}
	if(grp_ID == dest_grp_ID && f->ph >=2){  // check  wzy
		f->ph = 3;
	}
	if (f->min_or_not){
		out_port = min_df_ur_global_port(r, f, f->dest, grp_ID, dest_grp_ID, cur, dest_router, _grp_num_routers, nodeGateSize, numHosts);
	}
	else{
		if (f->ph == 0 || f->ph == 1){
			out_port = min_df_ur_global_port(r, f, f->intm, grp_ID, intm_grp, cur, intm_rID, _grp_num_routers, nodeGateSize, numHosts);
		}
		else{
			out_port = min_df_ur_global_port(r, f, f->dest, grp_ID, dest_grp_ID, cur, dest_router, _grp_num_routers, nodeGateSize, numHosts);
		}
	}
	assert(f->hops <= 6);
	int vcBegin = f->ph;
	int vcEnd = f->ph;

	f->tprCnt_bro.push_back(cur);
	f->tprCnt_bro.push_back(out_port);

	if(out_port >= nodeGateSize){
		vcBegin = 0;
		vcEnd = gNumVCs - 1;
	}

	outputs->Clear();
	outputs->AddRange(out_port, vcBegin, vcEnd, 0);

}


// --------------------------Cliquefly------------------------


// This function is used to get the minimal output port according to the routing table
int min_df_new_global_port(const Router *r, const Flit *f, int dest,  int grp_ID, int dest_grp_ID, int cur, int dest_router, int _grp_num_routers,  int nodeGateSize, int numHosts){
	int out_port = -1;
	if(grp_ID == dest_grp_ID){
		if (cur == dest_router){
			out_port = nodeGateSize + (dest % numHosts);
		} 
		else{
			int pos = dest_router % _grp_num_routers;
			out_port = r->oRouter->intra_group_port[pos];
		}
	}else{
		vector<int> temp = r->oRouter->inter_group_port[dest_grp_ID];
		RandomSeed(r->GetSimTime() + f->pid + f->src * f->dest + r->GetSimTime());
		assert(temp.size()!=0);
		int randRoute = RandomInt(temp.size() - 1);
		out_port = temp[randRoute];
	}
	return out_port;
}


// This function is used to generate the midrouter of cliquefly
int choose_mid_router_new_df_global(const Router *r, const Flit *f, const IQRouter *rr, int dest_grp_ID, int grp_ID, int min_port, int cur,  int _grp_num_routers = 12, int total_grp = 36, int total_routers = 432){
	int intm_router_ID;
	_grp_num_routers = omnetpp::getSimulation()->getSystemModule()->par("gK");
	total_grp = _grp_num_routers * _grp_num_routers / 4;
	total_routers = _grp_num_routers * total_grp;
	int bias = 0;
	int intm_grp_ID = 0;
	int intm_router = 0;
	int routerPos = -1;
	int superGroupSize = _grp_num_routers / 4;
	if(_grp_num_routers % 4!=0){
		superGroupSize += 1;
	}
	int nowSuperGroup = grp_ID / superGroupSize;
	int destSuperGroup = dest_grp_ID / superGroupSize;
	if(min_port >= gK-1){  // if the global link directly connect to the current router, choose a midrouter in this clique
		while(1){
			RandomSeed(f->btime + r->GetSimTime() + f->pid + rr->oRouter->getSimulation()->getSimTime().raw() + f->src * f->dest +bias);
			assert(r->oRouter->inter_group_connect.size() >= 4);
			int intm_idx = RandomInt(r->oRouter->inter_group_connect.size()-1); // 除去最短路径只适用于i=1
			intm_grp_ID =  r->oRouter->inter_group_connect[intm_idx];
			if(intm_grp_ID != dest_grp_ID){
				break;
			}
			bias++;
		}
		assert(intm_grp_ID != grp_ID);
		int intmSuperGroup = intm_grp_ID / superGroupSize;
		if(cur % gK == gK-1){
			routerPos = gK - 1;
		}
		else{
			if(intmSuperGroup == nowSuperGroup){
				routerPos = cur % gK;
			}
			else if(intmSuperGroup > nowSuperGroup){
				routerPos = nowSuperGroup;
			}
			else{
				routerPos = nowSuperGroup - 1;
			}
		}
		return intm_grp_ID*gK+routerPos;
	}
	else{  // choose a midrouter which is not in the source group and the destination group
		while(1){
			RandomSeed(f->btime + r->GetSimTime() + f->pid + rr->oRouter->getSimulation()->getSimTime().raw() + f->src * f->dest +bias);
			assert(r->oRouter->inter_group_connect.size() >= 4);
			intm_router = RandomInt(total_routers-1); 
			intm_grp_ID = intm_router / gK;
			int flag = 0;
			for (int i = 0;i<r->oRouter->inter_group_connect.size();i++){
				if (r->oRouter->inter_group_connect[i] == intm_grp_ID || grp_ID == intm_grp_ID || dest_grp_ID == intm_grp_ID){
					flag = 1;
				}
			}
			if(!flag){
				break;
			}
			else{
				bias++;
			}
		}
		assert(intm_grp_ID != grp_ID);
		return intm_router;
	}
	return -1;
}


// RPAR for cliquefly
void choosePort_par_df_new_global(const Router *r, const Flit *f, int, OutputSet *outputs, bool inject){
	const IQRouter *rr = dynamic_cast<const IQRouter *>(r);
	int cur = getOmnetRoute(r)->getParentModule()->par("id");
	int _grp_num_routers = omnetpp::getSimulation()->getSystemModule()->par("gK");
	int gN = omnetpp::getSimulation()->getSystemModule()->par("gN");
	int numHosts = omnetpp::getSimulation()->getSystemModule()->par("numHosts");
	int in_channel = f->getArrivalGate()->getIndex();
	int _grp_num_nodes = _grp_num_routers * gN;
	int grp_ID = (int)(cur / _grp_num_routers);
	int nodeGateSize = _grp_num_routers + gN - 1;
	int dest_grp_ID = int(f->dest / _grp_num_nodes);
	int out_port = -1;
	int total_grp = _grp_num_routers * gN / 2;
	int total_routers=_grp_num_routers * total_grp;
	int dest_router = f->dest / gN;
	int midRouter = -1;

	if(in_channel >= nodeGateSize){
		if(dest_grp_ID == grp_ID){
			f->ph = 3;
			f->min_or_not = true;
		}
		else{
			int min_router_output = min_df_new_global_port(r, f, f->dest, grp_ID, dest_grp_ID, cur, dest_router, _grp_num_routers, nodeGateSize, numHosts);;
			int min_queue_size = max(r->GetUsedCredit(min_router_output), 0);
			midRouter = choose_mid_router_new_df_global(r, f, rr, dest_grp_ID, grp_ID, min_router_output, cur);
			assert(midRouter != -1);
			f->intm = midRouter * numHosts;
			int nonmin_router_output = min_df_new_global_port(r, f, f->intm, grp_ID, int(midRouter/_grp_num_routers), cur, midRouter, _grp_num_routers, nodeGateSize, numHosts);
			if(min_router_output >= gK-1){
				assert(nonmin_router_output >= gK-1);
			}
			int nonmin_queue_size = max(r->GetUsedCredit(nonmin_router_output), 0);
			if(min_queue_size <= nonmin_queue_size * 2){  
				f->min_or_not = 1;
				f->ph = 0;
			}
			else{
				f->min_or_not = 0;
				f->ph = 0;
			}
		}

	}

	// PAR
	if(in_channel < nodeGateSize && f->min_or_not && dest_grp_ID != grp_ID){
		assert(f->ph == 0);
		int min_router_output = min_df_new_global_port(r, f, f->dest, grp_ID, dest_grp_ID, cur, dest_router, _grp_num_routers, nodeGateSize, numHosts);;
		int min_queue_size = max(r->GetUsedCredit(min_router_output), 0);
		midRouter = choose_mid_router_new_df_global(r, f, rr, dest_grp_ID, grp_ID, min_router_output, cur);
		assert(midRouter != -1);
		f->intm = midRouter * numHosts;
		int nonmin_router_output = min_df_new_global_port(r, f, f->intm, grp_ID, int(midRouter/_grp_num_routers), cur, midRouter, _grp_num_routers, nodeGateSize, numHosts);
		assert(nonmin_router_output >= gK-1 && min_router_output >= gK - 1);
		int nonmin_queue_size = max(r->GetUsedCredit(nonmin_router_output), 0);
		if(min_queue_size * 2 <= nonmin_queue_size * 3){
			f->min_or_not = 1;
			f->ph = 2;
		}
		else{
			f->min_or_not = 0;
			f->ph = 0;
		}
	}

	int intm_rID = int(f->intm / numHosts);
	int intm_grp = int(intm_rID / _grp_num_routers);
	if (f->ph <= 1 && !f->min_or_not)
	{
		if (grp_ID == intm_grp && cur != intm_rID)
		{
				f->ph = 1;
		}
		if (cur == intm_rID)
		{
				f->ph = 2;
		}
	}

	if(grp_ID == dest_grp_ID){
		f->ph = 3;
	}

	if (f->min_or_not){
		out_port = min_df_ur_global_port(r, f, f->dest, grp_ID, dest_grp_ID, cur, dest_router, _grp_num_routers, nodeGateSize, numHosts);
	}
	else{
		if (f->ph <= 1){
			out_port = min_df_ur_global_port(r, f, f->intm, grp_ID, intm_grp, cur, intm_rID, _grp_num_routers, nodeGateSize, numHosts);
		}
		else{
			out_port = min_df_ur_global_port(r, f, f->dest, grp_ID, dest_grp_ID, cur, dest_router, _grp_num_routers, nodeGateSize, numHosts);
		}
	}
	assert(f->hops <= 6);
	int vcBegin = f->ph;
	int vcEnd = f->ph;
	if(out_port >= nodeGateSize){
		vcBegin = 0;
		vcEnd = gNumVCs - 1;
	}
	outputs->Clear();
	outputs->AddRange(out_port, vcBegin, vcEnd, 0);
}


//  construct the routing table , please note, these code not belong to routefunc.cpp, these code should be put in the constructor or initial of the input-queue router

//-------------------Dragonfly-----------------------
if (routeFuncType=="min_df_ur_global" || routeFuncType == "ugal_df_ur_global" || routeFuncType == "par_df_ur_global" || routeFuncType == "BIAS_df_ur_global"){
		
	//get the intra-group routing table
		int grouPid = id / gK;
		int router_pos = id % gK;
		for(int i = 0;i<gK;i++){
			if(i < router_pos){
				intra_group_port[i] = i;
			}
			else if(i == router_pos){
				intra_group_port[i] = -1;
			}
			else{
				intra_group_port[i] = i - 1;
			}
		}
		int grpNum = 1 + gK * gK / 4;
		int h = gK / 2;
		
		// get the inter-group routing table
		for (int i = 0;i<grpNum;i++){
			vector<int> temp;
			int shift = (i + grpNum - grouPid) % grpNum;
			if(shift == 0){
				temp.push_back(-1);
			}
			else{
				int cnt = (shift-1) / h;
				int intra_group_router_id = (gK/2) - 1 -  cnt;
				assert(0<=intra_group_router_id && intra_group_router_id <= gK - 1);
				if(intra_group_router_id == router_pos || intra_group_router_id + h == router_pos){
					int direct_port = (shift - 1) % h;
					assert(direct_port >= 0 && direct_port < h);
					temp.push_back(gK - 1 + direct_port);
					inter_group_connect.push_back(i);
				}
				else{
					int port1 = intra_group_port[intra_group_router_id];
					int port2 = intra_group_port[intra_group_router_id + h];
					assert(port1 >= 0 && port1 <= gK - 2);
					temp.push_back(port1);
					temp.push_back(port2);
				}
			}
			inter_group_port.push_back(temp);
		}

	}
}



// --------------------------Cliquefly------------------------
if (routeFuncType=="min_df_new_global" || routeFuncType == "ugal_df_new_global" || routeFuncType == "par_df_new_global"){
	if(gK == 18){  // for cliquefly(9,18,9,85)
		//get the intra-group routinng table
		int grouPid = id / gK;
		int router_pos = id % gK;
		for(int i = 0;i<gK;i++){
			if(i < router_pos){
				intra_group_port[i] = i;
			}
			else if(i == router_pos){
				intra_group_port[i] = -1;
			}
			else{
				intra_group_port[i] = i - 1;
			}
		}
		int h = gK / 2;
		int superGroupSize = 5;
		int grpNum = superGroupSize * (gK-1);

		int nowSuperGroup = grouPid / superGroupSize;
		int superGroupPos = grouPid % superGroupSize;
		for(int i = 0;i<grpNum;i++){
			vector<int> temp;
			int destSuperGroup = i/superGroupSize;
			if(i != grouPid){
				if(router_pos == gK-1 || router_pos == gK - 2){  // handle the shortcut switch
					
					if((id % 90 == 17 || id % 90 == 16) && (grouPid+grpNum-i)%grpNum == 1){  // 补偿交换机向后连接
						for(int iii = 0;iii<gN;iii++) temp.push_back(gK-1+iii);
						inter_group_connect.push_back(i);
					}
					else if((id % 90 == 89 || id % 90 == 88) &&  (i+grpNum-grouPid)%grpNum == 1){
						for(int iii = 0;iii<gN;iii++) temp.push_back(gK-1+iii);
						inter_group_connect.push_back(i);
					}
					else if((id % 90 > 17 && id % 90 < 72) && ((((i+grpNum-grouPid)%grpNum == superGroupSize))||((grouPid+grpNum-i)%grpNum == superGroupSize))){
						if ((i+grpNum-grouPid)%grpNum == superGroupSize){
							for(int iii = 0;iii<superGroupSize-1;iii++) temp.push_back(gK-1+superGroupSize+iii);
							inter_group_connect.push_back(i);
						}
						else if ((grouPid+grpNum-i)%grpNum == superGroupSize){
							for(int iii = 0;iii<superGroupSize-1;iii++) temp.push_back(gK-1+iii);
							inter_group_connect.push_back(i);
						}
					}
					else if(destSuperGroup == nowSuperGroup)
					{
						for(int groupGo = 0;groupGo<gK-2;groupGo++){
							temp.push_back(groupGo);
						}
					}
					else{
						int intraGroupGo = -1;
						if(destSuperGroup > nowSuperGroup){
							intraGroupGo = destSuperGroup - 1;
						}
						else{
							intraGroupGo = destSuperGroup;
						}
						temp.push_back(intra_group_port[intraGroupGo]);
					}

				}
				else{
					if(destSuperGroup==nowSuperGroup){  // 簇内连接
						if(grouPid > i){
							temp.push_back(gK-1+(i%superGroupSize));
							inter_group_connect.push_back(i);
						}
						else{
							temp.push_back(gK-1+(i%superGroupSize)-1);
							inter_group_connect.push_back(i);
						}
					}
					else{  // 簇间连接
						int intraGroupGo = -1;
						if(destSuperGroup > nowSuperGroup){
							intraGroupGo = destSuperGroup - 1;
						}
						else{
							intraGroupGo = destSuperGroup;
						}
						if(intraGroupGo != router_pos){
							temp.push_back(intra_group_port[intraGroupGo]);

							if(id % 90 <= 16 && (grouPid+grpNum-i)%grpNum == 1){
								for(int iii = 0;iii<gN;iii++) {
									temp.push_back(intra_group_port[gK-1]);
									temp.push_back(intra_group_port[gK-2]);
								}
							}
							else if((id % 90) > 17 && (id % 90) < 72){
								if ((i+grpNum-grouPid)%grpNum == superGroupSize){
									for(int iii = 0;iii<superGroupSize-1;iii++) {
										temp.push_back(intra_group_port[gK-1]);
										temp.push_back(intra_group_port[gK-2]);
									}
								}
								else if ((grouPid+grpNum-i)%grpNum == superGroupSize){
									for(int iii = 0;iii<superGroupSize-1;iii++){
										temp.push_back(intra_group_port[gK-1]);
										temp.push_back(intra_group_port[gK-2]);
									}
								}
							}
							else if(id % 90 >= 72 && (i+grpNum-grouPid)%grpNum == 1){
								for(int iii = 0;iii<gN;iii++) {
									temp.push_back(intra_group_port[gK-1]);
									temp.push_back(intra_group_port[gK-2]);
								}
							}
						}
						else{
							temp.push_back(gK-1+superGroupSize-1+(i%superGroupSize));
							inter_group_connect.push_back(i);
						}
					}
				}
			}
			else{
				temp.push_back(-1);
				// assert(0);
			}
			inter_group_port.push_back(temp);
			if(!(temp.size()!=0)){
				cerr<<"temp.size() == 0!!!"<<"id = "<<id<<", superGroupSize = "<<superGroupSize<<", grpNum = "<<grpNum<<", nowSuperGroup = "<<nowSuperGroup<<", superGroupPos = "<<superGroupPos<<", router_pos = "<<router_pos<<endl<<", i = "<<i<<", destSuperGroup = "<<destSuperGroup<<endl;
			}
			assert(temp.size()!=0);
		}
		return;
	}

	// for cliquefly(6,12,6,36)
	int grouPid = id / gK;
	int router_pos = id % gK;
	for(int i = 0;i<gK;i++){
		if(i < router_pos){
			intra_group_port[i] = i;
		}
		else if(i == router_pos){
			intra_group_port[i] = -1;
		}
		else{
			intra_group_port[i] = i - 1;
		}
	}
	int grpNum = gK * gK / 4;
	int h = gK / 2;
	int superGroupSize = h/2;
	int nowSuperGroup = grouPid / superGroupSize;
	int superGroupPos = grouPid % superGroupSize;
	for(int i = 0;i<grpNum;i++){
		vector<int> temp;
		if(i != grouPid){
			int destSuperGroup = i/superGroupSize;
			if(router_pos == gK-1){  
				if(id % 36==11 && (grouPid+grpNum-i)%grpNum == 1){ 
					for(int iii = 0;iii<gN;iii++) temp.push_back(gK-1+iii);
					inter_group_connect.push_back(i);
				}
				else if(id % 36 == 35 &&  (i+grpNum-grouPid)%grpNum == 1){
					for(int iii = 0;iii<gN;iii++) temp.push_back(gK-1+iii);
					inter_group_connect.push_back(i);
				}
				else if(id % 36 == 23 && ((((i+grpNum-grouPid)%grpNum == superGroupSize))||((grouPid+grpNum-i)%grpNum == superGroupSize))){
					if ((i+grpNum-grouPid)%grpNum == superGroupSize){
						for(int iii = 0;iii<superGroupSize;iii++) temp.push_back(gK-1+superGroupSize+iii);
						inter_group_connect.push_back(i);
					}
					else if ((grouPid+grpNum-i)%grpNum == superGroupSize){
						for(int iii = 0;iii<superGroupSize;iii++) temp.push_back(gK-1+iii);
						inter_group_connect.push_back(i);
					}
				}
				else if(destSuperGroup == nowSuperGroup)
				{
					for(int groupGo = 0;groupGo<gK-1;groupGo++){
						temp.push_back(groupGo);
					}
				}
				else{
					int intraGroupGo = -1;
					if(destSuperGroup > nowSuperGroup){
						intraGroupGo = destSuperGroup - 1;
					}
					else{
						intraGroupGo = destSuperGroup;
					}
					temp.push_back(intra_group_port[intraGroupGo]);
				}
			}
			else{
				if(destSuperGroup==nowSuperGroup){
					if(grouPid > i){
						temp.push_back(gK-1+(i%3));
						inter_group_connect.push_back(i);
					}
					else{
						temp.push_back(gK-1+(i%3)-1);
						inter_group_connect.push_back(i);
					}
				}
				else{ 
					int intraGroupGo = -1;
					if(destSuperGroup > nowSuperGroup){
						intraGroupGo = destSuperGroup - 1;
					}
					else{
						intraGroupGo = destSuperGroup;
					}
					if(intraGroupGo != router_pos){
						temp.push_back(intra_group_port[intraGroupGo]);
						if(id % 36 <= 11 && (grouPid+grpNum-i)%grpNum == 1){
							for(int iii = 0;iii<gN;iii++) temp.push_back(intra_group_port[gK-1]);
						}
						else if((id % 36) > 11 && (id % 36) < 24){
							if ((i+grpNum-grouPid)%grpNum == superGroupSize){
								for(int iii = 0;iii<superGroupSize;iii++) temp.push_back(intra_group_port[gK-1]);
							}
							else if ((grouPid+grpNum-i)%grpNum == superGroupSize){
								for(int iii = 0;iii<superGroupSize;iii++) temp.push_back(intra_group_port[gK-1]);
							}
						}
						else if(id % 36 >= 24 && (i+grpNum-grouPid)%grpNum == 1){
							for(int iii = 0;iii<gN;iii++) temp.push_back(intra_group_port[gK-1]);
						}
					}
					else{
						temp.push_back(gK-1+superGroupSize+(i%superGroupSize));
						inter_group_connect.push_back(i);
						if((i%superGroupSize) == (grouPid % superGroupSize)){ 
							temp.push_back(gK-1+superGroupSize-1);
						}
					}
				}
			}
		}
		else{
			temp.push_back(-1);
		}
		inter_group_port.push_back(temp);
		assert(temp.size()!=0);
	}
}
