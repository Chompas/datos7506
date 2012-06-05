/*
 * Utils.h
 *
 *  Created on: Jun 2, 2012
 *      Author: vdiego
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <string>
#include <sstream>


namespace Utils
{
	extern std::stringstream errSS;

	extern bool debugMode;
	extern std::stringstream dbgSS;

	void LogError(const std::ostream& mensaje);

	void LogDebug(const std::ostream& mensaje);

	std::string TimeStampToString(const time_t timestamp);



}



#endif /* UTILS_H_ */
