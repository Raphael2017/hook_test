#ifndef TALK_WITH_HDB_H
#define TALK_WITH_HDB_H

#include "SQLDBC.h"

/*
 * these function is hooked, call xxxx_old to the original function
 * */
extern "C" {
////////////////////////////////////////////////////////////////////
typedef
SQLDBC_Retcode (*PrepareFun)(SQLDBC::SQLDBC_PreparedStatement *self,
                             const char *sql,
                             const SQLDBC_StringEncoding encoding);

extern PrepareFun prepare_old;
SQLDBC_Retcode prepare_new(SQLDBC::SQLDBC_PreparedStatement *self,
                           const char *sql,
                           const SQLDBC_StringEncoding encoding);
////////////////////////////////////////////////////////////////////
typedef
SQLDBC_Retcode (*ExecuteItabFun)(SQLDBC::SQLDBC_PreparedStatement *self,
                                 void*,
                                 bool);

extern ExecuteItabFun execute_itab_old;
SQLDBC_Retcode execute_itab_new(SQLDBC::SQLDBC_PreparedStatement *self,
                                void *p,
                                bool b);
////////////////////////////////////////////////////////////////////
typedef
void* (*GetItabReaderFun)(SQLDBC::SQLDBC_PreparedStatement *self);

extern GetItabReaderFun get_itab_reader_old;
void* get_itab_reader_new(SQLDBC::SQLDBC_PreparedStatement *self);
////////////////////////////////////////////////////////////////////
typedef
SQLDBC_Retcode (*BindParameterFun) (SQLDBC::SQLDBC_PreparedStatement *self,
                                    const SQLDBC_UInt2     Index,
                                    const SQLDBC_HostType  Type,
                                    void                  *paramAddr,
                                    SQLDBC_Length         *LengthIndicator,
                                    const SQLDBC_Length    Size,
                                    const SQLDBC_Bool      Terminate);
extern BindParameterFun bind_parameter_old;
SQLDBC_Retcode bind_parameter_new(SQLDBC::SQLDBC_PreparedStatement *self,
                                  const SQLDBC_UInt4     Index,
                                  const SQLDBC_HostType  Type,
                                  void                  *paramAddr,
                                  SQLDBC_Length         *LengthIndicator,
                                  const SQLDBC_Length    Size,
                                  const SQLDBC_Bool      Terminate);
///////////////////////////////////////////////////////////////////
typedef SQLDBC_Retcode (*ExecuteFun)(SQLDBC::SQLDBC_PreparedStatement *self);
extern ExecuteFun execute_old;
SQLDBC_Retcode execute_new(SQLDBC::SQLDBC_PreparedStatement *self);
/////////////////////////////////////////////////////////////////////////////

typedef SQLDBC::SQLDBC_ResultSet *(*GetResultSetFun)(SQLDBC::SQLDBC_PreparedStatement *self);
extern GetResultSetFun get_result_set_old;
SQLDBC::SQLDBC_ResultSet *get_result_set_new(SQLDBC::SQLDBC_PreparedStatement *self);
/////////////////////////////////////////////////////////////////////////////
}

#endif