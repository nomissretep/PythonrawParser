/*
 * PyRawRead.hpp
 *
 *  Created on: Oct 15, 2013
 *      Author: speters
 */

#ifndef PYRAWREAD_HPP_
#define PYRAWREAD_HPP_

#include <string>

class PyRawRead {
public:
	PyRawRead();
	PyRawRead(const char *file);
	void Read();
	virtual ~PyRawRead();
private:
	char * file;
	void* matrix;
	void* sync;
	int matrix_dims[];
	int sync_dims[];
};

#endif /* PYRAWREAD_HPP_ */
