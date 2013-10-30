/*
 * Data.hpp
 *
 *  Created on: Oct 15, 2013
 *      Author: speters
 */

#ifndef DATA_HPP_
#define DATA_HPP_

#include "enums.hpp"
#include <vector>
#include <valarray>
#include <cstring>
#include "sMDH.hpp"
#include <iostream>
#include <iomanip>

using namespace std;

class Data {
private:
	bool Allocated;
	int l;

	public:
	vector<size_t> dims;
	vector<size_t> idx;
	vector<float> ress;
	char* data;
	size_t d_size;

	/**
	 * @brief Constructor
	 */
	Data() {
		dims.resize(MAX_ICE_DIM, (size_t) 1);
		ress.resize(MAX_ICE_DIM, (float) 1);
		idx.resize(MAX_ICE_DIM, (size_t) 1);
		Allocated=false;
		l=0;
		d_size=sizeof(std::complex<float>);
	}

	/**
	 * @brief Allocate RAM
	 */
	inline void Allocate(const bool verbose = false) {
		idx[0] = 1;
		for (size_t i = 1; i < MAX_ICE_DIM; i++)
			idx[i] = idx[i - 1] * dims[i - 1];
		cout<< "Data (dims:" ;
		for (size_t i = 0; i < MAX_ICE_DIM; i++) {
			cout << dims[i];
			if (i < AVE)
				cout << " ";
			else
				cout<<") (memory:"<<fixed <<setprecision(1) <<8.0 * idx[AVE] / 1024.0 / 1024.0<<" MB)"<<endl;

		}
		data=new char[idx[AVE]*d_size];
		Allocated=true;
	}

	/**
	 * @brief Clear RAM
	 */
	inline void Clear() {

		dims.resize(MAX_ICE_DIM, (size_t) 1);
		ress.resize(MAX_ICE_DIM, (float) 1);
		idx.resize(MAX_ICE_DIM, (size_t) 1);
		free(data);
		Allocated=false;

	}

	/**
	 * @brief insert data
	 * wo hin, wie viel,was
	 */

	inline void insert(char* pt,size_t size,sMDH* smdh) {
		if(Allocated){
		memcpy( &data[(smdh->sLC.ushLine * idx[LIN-1]+
		smdh->sLC.ushSlice * idx[SLC-1]+
		smdh->sLC.ushPartition * idx[PAR-1]+
		smdh->sLC.ushSeg * idx[SEG-1]+
		smdh->sLC.ushEcho * idx[ECO-1]+
		smdh->ushChannelId * idx[CHA-1]+
		smdh->sLC.ushSet * idx[SET-1]+
		smdh->sLC.ushPhase * idx[PHS-1]+
		smdh->sLC.ushRepetition * idx[REP-1]+
		smdh->sLC.ushIda * idx[IDA-1]+
		smdh->sLC.ushIdb * idx[IDB-1]+
		smdh->sLC.ushIdc * idx[IDC-1]+
		smdh->sLC.ushIdd * idx[IDD-1]+
		smdh->sLC.ushIde * idx[IDE-1])*d_size],pt,size);
		}
		else
			cout<<"Not Allocated.Please allocate first"<< endl;
	}



	/*inline void insert(char* pt,size_t size,const size_t& col,
	                          const size_t& lin,
	                          const size_t& cha,
	                          const size_t& set,
	                          const size_t& eco,
	                          const size_t& phs = 0,
	                          const size_t& rep = 0,
	                          const size_t& seg = 0,
	                          const size_t& par = 0,
	                          const size_t& slc = 0,
	                          const size_t& ida = 0,
	                          const size_t& idb = 0,
	                          const size_t& idc = 0,
	                          const size_t& idd = 0,
	                          const size_t& ide = 0,
	                          const size_t& ave = 0) const {

		memcpy( &data[col+
		              lin* idx[0]+
		              cha * idx[1]+
		              set * idx[2]+
		              eco * idx[3]+
		              phs * idx[4]+
		              rep * idx[5]+
		              seg * idx[6]+
		              par * idx[7]+
		              slc * idx[8]+
		              ida * idx[9]+
		              idb * idx[10]+
		              idc * idx[11]+
		              idd * idx[12]+
		              ide * idx[13]+
		              ave * idx[14]],pt,size);



	}*/
};


#endif /* DATA_HPP_ */
