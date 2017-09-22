#pragma once


//#ifndef FIASCO_H
//#define FIASCO_H
//
///////////////////////////////////////////////////////////////////////////////////////////////////////
//// [WS 2010-07-30] Detect the infamous "Static initialization order fiasco"
//// email warrenstevens --> [initials]@[firstnamelastname].com 
//// read --> http://www.parashift.com/c++-faq-lite/ctors.html#faq-10.12 if you haven't suffered
//// To enable this feature --> define E-N-A-B-L-E-_-F-I-A-S-C-O-_-F-I-N-D-E-R, rebuild, and run
////#define ENABLE_FIASCO_FINDER
///////////////////////////////////////////////////////////////////////////////////////////////////////
//
//#ifdef ENABLE_FIASCO_FINDER
//
//#ifdef ANDROID
//#include <android/log.h>
//inline bool WriteFiasco(const char * psz)
//{
//	static int counter = 0;
//	++counter;
//
//	/*std::ofstream file;
//	file.open("FiascoFinder.txt", std::ios::out | std::ios::app);
//	file << "Starting to initialize file - number: [" << counter << "] filename: [" << fileName.c_str() << "]" << std::endl;
//	file.flush();
//	file.close();*/
//	__android_log_print(ANDROID_LOG_INFO, "fiasco_finder", psz);
//	//__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
//	return true;
//}
//#else
//inline bool WriteFiasco(const char * psz)
//{
//	static int counter = 0;
//	++counter;
//
//	/*std::ofstream file;
//	file.open("FiascoFinder.txt", std::ios::out | std::ios::app);
//	file << "Starting to initialize file - number: [" << counter << "] filename: [" << fileName.c_str() << "]" << std::endl;
//	file.flush();
//	file.close();*/
//	__android_log_print(ANDROID_LOG_INFO, "fiasco_finder", psz);
//	//__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
//	return true;
//}
//#endif
//
//// [WS 2010-07-30] If you get a name collision on the following line, your usage is likely incorrect
//#define FIASCO_FINDER static const bool g_psuedoUniqueName = WriteFiasco(__FILE__);
//
//#else // ENABLE_FIASCO_FINDER
//// do nothing
//#define FIASCO_FINDER
//
//#endif // ENABLE_FIASCO_FINDER
//
//#endif //FIASCO_H
//
//
//FIASCO_FINDER
//
