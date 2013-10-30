/*
 * rawParser.cpp
 *
 *  Created on: Oct 15, 2013
 *      Author: speters
 */

#include "rawParser.hpp"
/*
 * rawParser.c
 *
 *  Created on: Oct 15, 2013
 *      Author: speters
 */

#include <typeinfo>
#include "enums.hpp"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <complex>
#include <string>
#include "error_code.hpp"
#include "sMDH.hpp"
#include "Data.hpp"
#include <stdlib.h>
#include <stdio.h>
#include "inline.hpp"
using namespace std;



rawParser::rawParser()
{
	m_initialised = false;
	m_size=0;
	m_dataSize=0;
	m_fod=0;
	m_acqend=false;
	m_pdata=0;
	m_mdata=0;
	m_palloc=false;
	m_malloc=false;
	m_meas_col_bytes=0;
	m_pos=0;
	m_verb=true;
}


rawParser::rawParser(const char *fname)  {
				 /*m_cur_sync(0),*/
	m_size=0;
	m_dataSize=0;
	m_fod=0;
	m_acqend=false;
	m_pdata=0;
	m_mdata=0;
	m_palloc=false;
	m_malloc=false;
	m_meas_col_bytes=0;
	m_pos=0;
	m_verb=true;
	//m_ifs();//fname
	//m_ifs.open("/opt/meas.dat",ios::binary|ios::in);
	if(m_ifs.fail())
		cout<<"Lesen ist Fehlgeschlagen"<<endl;
	// TODO Open the file
//	cout<<fname<<endl;
//	if(m_ifs.is_open())
//		cout<<"wurde geoeffnet"<<endl;
	//m_matrix()=new Data();
	//m_meas_dims = std::vector<size_t>(MAX_ICE_DIM,(size_t)1);
	if (m_ifs.fail()) {
		cout << "Error opening file " << fname << endl;
		cerr << "Error opening file " << fname << endl;
		m_status = FILE_OPEN_FAILED;
	}
	m_initialised = true;
#ifdef USE_OMP1
	//cout<<" OMP verbesert "<< endl;
#endif
#ifdef USE_OMP
	NumOfThreas=omp_get_num_procs();
	ThreadPos=new vector<float>(NumOfThreads+1);
#endif

}


rawParser::~rawParser() {

	this->CleanUp();
	m_sync.Clear();
	m_matrix.Clear();
}


error_code rawParser::Read() {

	m_status = OK;
	/* Calculate data size */
	CalcSize();
//	cout<<fixed<<m_ifs.tellg()<<endl;
	/* Read protocol into buffer */
	ReadPFile();
#ifdef USE_OMP1
#pragma omp sections
	{
#pragma omp section
	{
#endif
	/* Exract protocol */
	ExtractProtocol();

	if (m_palloc) {
		delete[] m_pdata;
		m_palloc = false;
	}
#ifdef USE_OMP1
	}
#pragma omp section
	{
#endif
	/* Read messement into buffer */
	ReadMFile();

	/* Access matrix sizes */
//if (amode == SCAN_MEAS)
	preParseMeas(m_verb);
	/*else
	 ParseProtocol(this->m_verb);*/
#ifdef USE_OMP1
	}
	}
#endif
	/*for(int i=0;i<MAX_ICE_DIM;++i)
		cout<<m_matrix.dims[i]<<",";
	cout<<endl;*/

	/* Allocate matrices */
	AllocateMatrices();

	/* Parse data into matrices */
	ParseMeas();
	/* Free buffer */

	CleanUp();

	return m_status;

}


error_code rawParser::CalcSize() {
	unsigned fod;
	char *fode=(char *) &fod;
	m_ifs.read(fode,sizeof(fod));// >> fod;
	m_fod = (size_t) fod;
	//cout<< m_fod <<endl;
	if (m_ifs.fail()) {
		cerr << "Reading error" << endl;
		m_status = FILE_READ_FAILED;
	}

	//cout << "\n Protocol:" << setprecision(1)<<fixed
//			<< ((-16.0 + (float) m_fod) / 1024.0) << "KB\n" << endl;
	m_ifs.seekg(-1, ios::end);
	m_size = m_ifs.tellg();
	m_dataSize = m_size - m_fod;
//	cout << " Measurement: " << setprecision(1)<<fixed
//			<< (((float) m_dataSize) / 1024.0 / 1024.0) << " MB\n" << endl;
	m_ifs.seekg(0);

#ifdef USE_OMP
	ThreadSize=m_dataSize/NumOfThreads;
#endif

	return m_status;
}


error_code rawParser::ReadPFile() {
	//Speicher allokieren
	m_pdata = new char[m_fod];
	//Daten in Speicher laden
	m_ifs.read(m_pdata, m_fod);
	m_palloc=true;
	return m_status = OK;
}


error_code rawParser::ExtractProtocol()/*const bool verbose = true)*/{

	m_status = OK;
	m_xprot.append(&m_pdata[19], m_fod);
	return m_status;

}


error_code rawParser::ParseProtocol(const bool verbose /*= true*/, const bool dry /*= false*/) {

	m_status = OK;

	return m_status;

}


error_code rawParser::ReadMFile() {
	//Speicher allokieren
	m_mdata = new char[m_dataSize];
	m_malloc=true;
	//Daten in Speicher laden
	m_ifs.read(m_mdata, m_dataSize);
	return m_status = OK;

}


error_code rawParser::preParseMeas(const bool verbose /*= true*/) {

	m_status = OK;
	m_acqend = false;
	m_pos = 0;
	//m_cur_sync = 0;

//	cout << " Parsing measurement (dry run!)...\n" << endl;
//	size_t sc = 0;


	while (true) {


#ifdef USE_OMP
	if(m_pos>=ThreadSize*ThreadNum)
	{
		ThreadPos[ThreadNum]=m_pos;
		ThreadNum++;
	}
#endif


		preParseMDH(verbose);
		if (m_acqend)	//Abfrage ob es die letzte mdh war
		{
#ifdef USE_OMP
			ThreadPos[ThreadNum]=m_dataSize;
#endif
				break;
		}

		/*if (bit_set(m_cur_mdh.aulEvalInfoMask[0], SYNCDATA))
			cout<<"Sync Data erreicht"<<endl;
			break;
//ParseSyncData(verbose, dry);
		else*/
			m_pos += m_meas_col_bytes;

//		sc++;

	}

//	cout << " ... done.\n\n" << endl;

	return m_status;

}


error_code rawParser::preParseMDH(const bool verbose /*= true*/) {

	m_status = OK;
	vector<long unsigned int>* matrix_dim;

	memcpy(&m_cur_mdh, &m_mdata[m_pos], sizeof(sMDH));	//copy wuerde es auc tun
	if (bit_set(m_cur_mdh.aulEvalInfoMask[0], ACQEND)) {
		m_acqend = true;
	//	cout<<"Ende erreicht"<<endl;
		return m_status;
	}
	if (bit_set(m_cur_mdh.aulEvalInfoMask[0],SYNCDATA)) {
		matrix_dim=&(m_sync.dims);
		//may jump a bit more for spezial syncdata bits
	}
	else
		matrix_dim=&(m_matrix.dims);

	if ((*matrix_dim)[0] == 1) {

		(*matrix_dim)[COL] = (size_t) m_cur_mdh.ushSamplesInScan;
		(*matrix_dim)[CHA] = (size_t) m_cur_mdh.ushUsedChannels;

	 } else {
		if ((*matrix_dim)[LIN] < (size_t) m_cur_mdh.sLC.ushLine + 1)
			(*matrix_dim)[LIN] = m_cur_mdh.sLC.ushLine + 1;
		if ((*matrix_dim)[SLC] < (size_t) m_cur_mdh.sLC.ushSlice + 1)
			(*matrix_dim)[SLC] = m_cur_mdh.sLC.ushSlice + 1;
		if ((*matrix_dim)[PAR] < (size_t) m_cur_mdh.sLC.ushPartition + 1)
			(*matrix_dim)[PAR] = m_cur_mdh.sLC.ushPartition + 1;
		if ((*matrix_dim)[SEG] < (size_t) m_cur_mdh.sLC.ushSeg + 1){
			(*matrix_dim)[SEG] = m_cur_mdh.sLC.ushSeg + 1;}
		if ((*matrix_dim)[ECO] < (size_t) m_cur_mdh.sLC.ushEcho + 1)
			(*matrix_dim)[ECO] = m_cur_mdh.sLC.ushEcho + 1;
		if ((*matrix_dim)[SET] < (size_t) m_cur_mdh.sLC.ushSet + 1)
			(*matrix_dim)[SET] = m_cur_mdh.sLC.ushSet + 1;
		if ((*matrix_dim)[PHS] < (size_t) m_cur_mdh.sLC.ushPhase + 1)
			(*matrix_dim)[PHS] = m_cur_mdh.sLC.ushPhase + 1;
		if ((*matrix_dim)[REP] < (size_t) m_cur_mdh.sLC.ushRepetition + 1)
			(*matrix_dim)[REP] = m_cur_mdh.sLC.ushRepetition + 1;
		if ((*matrix_dim)[IDA] < (size_t) m_cur_mdh.sLC.ushIda + 1)
			(*matrix_dim)[IDA] = m_cur_mdh.sLC.ushIda + 1;
		if ((*matrix_dim)[IDB] < (size_t) m_cur_mdh.sLC.ushIdb + 1)
			(*matrix_dim)[IDB] = m_cur_mdh.sLC.ushIdb + 1;
		if ((*matrix_dim)[IDC] < (size_t) m_cur_mdh.sLC.ushIdc + 1)
			(*matrix_dim)[IDC] = m_cur_mdh.sLC.ushIdc + 1;
		if ((*matrix_dim)[IDD] < (size_t) m_cur_mdh.sLC.ushIdd + 1)
			(*matrix_dim)[IDD] = m_cur_mdh.sLC.ushIdd + 1;
		if ((*matrix_dim)[IDE] < (size_t) m_cur_mdh.sLC.ushIde + 1)
			(*matrix_dim)[IDE] = m_cur_mdh.sLC.ushIde + 1;
	}
	m_pos += sizeof(sMDH);
	m_meas_col_bytes = sizeof(std::complex<float>) * (*matrix_dim)[0];	//wie gross ist der folgende Header
	return m_status;

}


error_code rawParser::AllocateMatrices(const bool verbose /*= true*/) {

	m_status = OK;

//	cout<< " Allocating data matrices ..."<<endl;

	m_matrix.Allocate();

	m_sync.Allocate();

//	cout<<" ... done."<<endl;

	return m_status;

}


error_code rawParser::ParseMeas(const bool verbose /*= true*/) {

	m_status = OK;
	m_acqend = false;
	m_pos = 0;
	//m_cur_sync = 0;

//	cout << " Parsing measurement ";

//	size_t sc = 0;
#ifdef USE_OMP
#pragma omp parallel for private (m_cur_mdh,matrix,m_pos,m_meas_col_bytes)
	for (int i=0;i<NumOfThreads;i++) {
	m_pos=ThreadPos[i];
	while(true){
		ParseMDH(verbose);
		if (m_acqend||ThreadPos[i+1]>=m_pos)	//Abfrage ob es die letzte mdh war
			break;

		if (bit_set(m_cur_mdh.aulEvalInfoMask[0], SYNCDATA))
			matrix=&m_sync;
		else
			matrix=&m_matrix;
//ParseSyncData(verbose, dry);
			ParseScan(matrix,verbose);

		//sc++;
	}
	}
#else
	while (true) {

		ParseMDH(verbose);
		if (m_acqend)	//Abfrage ob es die letzte mdh war
			break;

		if (bit_set(m_cur_mdh.aulEvalInfoMask[0], SYNCDATA))
			ParseScan(m_sync,verbose);
		else
			ParseScan(m_matrix,verbose);
//ParseSyncData(verbose, dry);

	//	sc++;

	}
#endif
	//cout << " ... done.\n\n" << endl;

	return m_status;

}


error_code rawParser::ParseMDH(const bool verbose /*= true*/) {

	m_status = OK;

	memcpy(&m_cur_mdh, &m_mdata[m_pos], sizeof(sMDH));	//copy wuerde es auc tun

	if (bit_set(m_cur_mdh.aulEvalInfoMask[0], ACQEND)) {
		m_acqend = true;
		return m_status;
	}

	if (bit_set(m_cur_mdh.aulEvalInfoMask[0],SYNCDATA)) {
		//may jump a bit more for spezial syncdata bits
	}
	m_meas_col_bytes = sizeof(std::complex<float>) * m_cur_mdh.ushSamplesInScan;
	m_pos += sizeof(sMDH);
	return m_status;

}


error_code rawParser::ParseScan(Data matrix,const size_t& c) {

	m_status = OK;

	/*cout << "0," << m_cur_mdh.sLC.ushLine << "," << m_cur_mdh.sLC.ushSlice
			<< "," << m_cur_mdh.sLC.ushPartition << "," << m_cur_mdh.sLC.ushSeg
			<< "," << m_cur_mdh.sLC.ushEcho << "," << m_cur_mdh.ushChannelId
			<< "," << m_cur_mdh.sLC.ushSet << "," << m_cur_mdh.sLC.ushPhase
			<< "," << m_cur_mdh.sLC.ushRepetition << "," << m_cur_mdh.sLC.ushIda
			<< "," << m_cur_mdh.sLC.ushIdb << "," << m_cur_mdh.sLC.ushIdc << ","
			<< m_cur_mdh.sLC.ushIdd << "," << m_cur_mdh.sLC.ushIde << endl;*/

	matrix.insert( &(m_mdata[m_pos]), m_meas_col_bytes,&m_cur_mdh);
	m_pos += m_meas_col_bytes;
	return m_status;

}


error_code rawParser::CleanUp(const bool verbose /*= true*/) {

	m_status = OK;

	if (m_initialised) {

		if (m_malloc) {
			delete (m_mdata);
			m_malloc = false;
		}

		if (m_palloc) {
			delete (m_pdata);
			m_palloc = false;
		}
		m_ifs.close();

//m_sync.Clear();
//m_noise.Clear();
//m_acs.Clear();

		m_initialised = false;
	}
	return m_status;

}

