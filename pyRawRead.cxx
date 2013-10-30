/*
 * PyRawRead.cpp
 *
 *  Created on: Oct 15, 2013
 *      Author: speters
 */

#include "pyRawRead.h"
#include "rawParser.hpp"


PyRawRead::PyRawRead() {
	rp=NULL;
}

PyRawRead::PyRawRead(const char *file) {
	this->file=file;
}
void PyRawRead::Read() {
	//rawParser* rr=(rawParser*)rp;
	//rr->Read();
	/*struct PyStruct a;
	a.matarix=rr->m_matrix.data;
	a.sync=rr.m_sync.data;
	for(int i=0;i<16;i++){
		a.m_dims[i]=rr->m_matrix.dims[i];
		a.s_dims[i]=rr->m_sync.dims[i];
	}*/
	//return a;
}
PyRawRead::~PyRawRead() {
	free(matrix);
	free(sync);

}

