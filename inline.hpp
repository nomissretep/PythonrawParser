/*
 * inline.hpp
 *
 *  Created on: Oct 15, 2013
 *      Author: speters
 */

#ifndef INLINE_HPP_
#define INLINE_HPP_

inline static bool bit_set(long eim, const EIM_BIT eb) {
	return (eim & (1l << eb));
}


#endif /* INLINE_HPP_ */
