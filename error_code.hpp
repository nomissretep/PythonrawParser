/*
 * error_code.hpp
 *
 *  Created on: Oct 15, 2013
 *      Author: speters
 */

#ifndef ERROR_CODE_HPP_
#define ERROR_CODE_HPP_

enum error_code {

OK,
ERROR_GENERAL,
ERROR_UNKNOWN_METHOD,
FATAL_SYSTEM_CALL,
LAPACK_WORKSPACE_QUERY_FAILED,
CG_DID_NOT_CONVERGE,
CANNOT_LOAD_LIBRARY,
UNSUPPORTED_DIMENSION,
UNSUPPORTED_IMAGE_MATRIX,
ZERO_NODES,
CGSENSE_ZERO_CHANNELS,
FILE_ACCESS_FAILED,
CONTEXT_CONFIGURATION_FAILED,
CONTEXT_NOT_FOUND,
MEM_ALLOC_FAILED,
FILE_OPEN_FAILED,
FILE_READ_FAILED,
FILE_WRITE_FAILED,
NULL_FILE_HANDLE,
UNIMPLEMENTED_METHOD,
HDF5_ERROR_FOPEN,
HDF5_ERROR_FCLOSE,
HDF5_ERROR_FFLUSH,
GENERAL_IO_ERROR

};



#endif /* ERROR_CODE_HPP_ */
