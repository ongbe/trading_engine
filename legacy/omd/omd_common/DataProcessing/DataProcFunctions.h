//**************************************************
//  Author:      Sunny Yan
//  Created On:  Wed Apr 30 09:57:45 HKT 2014
//  Description: Centralize a place for common data processing functions
//
//**************************************************

#ifndef DATAPROCFUNC_H_
#define DATAPROCFUNC_H_

#include "../Container/OrderBook.h"
#include "../OMD_Global.h"
#include "../OMD_Message_Headers.h"
#include "../Logger/Logger.h"
#include "../SystemConfig.h"
#include <boost/shared_ptr.hpp>
#include <vector>
#include <sstream>
#include <boost/lexical_cast.hpp>
#include "../Util.h"


class DataProcFunctions {
  public:
    DataProcFunctions() {}
    virtual ~DataProcFunctions() {}
    virtual void OutputJsonToLog(const char *, const unsigned short, shared_ptr<Logger>, const BYTE*, char[]) = 0;
    virtual void OutputJsonToLog(const char *, const unsigned short, shared_ptr<Logger>, const BYTE*, unsigned short, char[], unsigned long) = 0;
    virtual void ProcessOrderBookInstruction(const char *,shared_ptr<Logger>,const BYTE*,shared_ptr<SharedObjects>,bool) = 0;
    virtual void ProcessMessageForMDI(shared_ptr<SharedObjects> shrobj, BYTE*,unsigned short);

  private:
    shared_ptr<SystemConfig>  m_SysCfg;
};

class DataProcFunctionsOMDC : public DataProcFunctions {
  public:
    DataProcFunctionsOMDC() : DataProcFunctions() {}
    virtual ~DataProcFunctionsOMDC() {}
    void OutputJsonToLog(const char *, const unsigned short, shared_ptr<Logger>, const BYTE*, char[]);
    void OutputJsonToLog(const char *, const unsigned short, shared_ptr<Logger>, const BYTE*, unsigned short, char[], unsigned long);
    void ProcessOrderBookInstruction(const char *,shared_ptr<Logger>,const BYTE*,shared_ptr<SharedObjects>,bool);
};

class DataProcFunctionsOMDD : public DataProcFunctions {
  public:
    DataProcFunctionsOMDD() : DataProcFunctions() {}
    virtual ~DataProcFunctionsOMDD() {}
    void OutputJsonToLog(const char *, const unsigned short, shared_ptr<Logger>, const BYTE*, char[]);
    void OutputJsonToLog(const char *, const unsigned short, shared_ptr<Logger>, const BYTE*, unsigned short, char[], unsigned long);
    void ProcessOrderBookInstruction(const char *,shared_ptr<Logger>,const BYTE*,shared_ptr<SharedObjects>,bool);
};

class DataProcFuncFactory {
public:
	DataProcFuncFactory();
	virtual ~DataProcFuncFactory();
  static DataProcFunctions* GetDataProcFunc(SystemConfig::Identity);
};

#endif /* DATAPROCFUNC_H_ */