%module pyRawRead
%include "std_string.i"

%{
#include "pyRawRead.h"
%}

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
