// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the VWCXP_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// VWCXP_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
// #ifdef VWCXP_EXPORTS
// #define VWCXP_API __declspec(dllexport)
// #else
// #define VWCXP_API __declspec(dllimport)
// #endif
// 
// // This class is exported from the VwCXP.dll
// class VWCXP_API CVwCXP {
// public:
// 	CVwCXP(void);
// 	// TODO: add your methods here.
// };
// 
// extern VWCXP_API int nVwCXP;
// 
// VWCXP_API int fnVwCXP(void);





#pragma once
#include "VwCXP.Global.h"



#include <list>



#ifdef _USRDLL
#define CLASS_IMPORTEXPORT __declspec(dllexport)
#else if
#define CLASS_IMPORTEXPORT __declspec(dllimport)
#endif

using namespace std;
using namespace GenICam;
using namespace GenICam::Client;


namespace VWSDK
{

	namespace CXP
	{


		class VwCXPGrabberBoard;

#define CXP_IF_MAX		4	

		class CLASS_IMPORTEXPORT VwCXP
		{
		private:
			TL_HANDLE					m_hTL;
			IF_HANDLE					m_hInterface[CXP_IF_MAX];

			char*						m_pszInterfaceID;
			char*						m_pszDisplayName;

			list<VwCXPGrabberBoard*>	m_listGrabberBoard;

		public:
			VwCXP(void);
			virtual ~VwCXP(void);

			RESULT Discovery();

			RESULT OpenCamera(VwCXPGrabberBoard* pGrabberBoard);

			RESULT GetGrabberBoardCount(UINT& _grabberCount);
			RESULT GetGrabberBoard(VwCXPGrabberBoard** _pGrabberBoard, UINT _grabberIndex);

			RESULT UserLogging(IN TCHAR* pModuleName, IN TCHAR* pFileName, IN TCHAR* pFuncName, IN UINT unLine, IN TCHAR* pLogMsg);


		private:
			RESULT _DiscoveryGrabberBoard(UINT _interfaceIndex);

			RESULT _ResetGrabberBoard();
		};
	}
}//namespace VWSDK

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)	if(p){ delete (p); (p) = NULL;} 
#endif //SAFE_DELETE

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p)	if(p){ delete[] (p); (p) = NULL;}
#endif //SAFE_DELETE_ARRAY

