#include "stdio.h"
#include <dlfcn.h>
#include "SQLDBC.h"
#include <string>
#include "hook.h"
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>
#include "PreparedStatementInfoMgr.h"
#include <iostream>

#define LOCAL_OUT_PUT       "/home/qwerty/hook_log.txt"
#define REMOTE_OUT_PUT      "/home/he4adm/hook_log.txt"
#define TERMINAL_OUT_PUT    ""

#define SQLDBC_SQLDBC_PreparedStatement_prepare_char_const_SQLDBC_StringEncodingType_Encoding   "_ZN6SQLDBC24SQLDBC_PreparedStatement7prepareEPKcN25SQLDBC_StringEncodingType8EncodingE"
#define SQLDBC_SQLDBC_PreparedStatement_executeItab_void_bool                                   "_ZN6SQLDBC24SQLDBC_PreparedStatement11executeItabEPvb"
#define SQLDBC_SQLDBC_PreparedStatement_getItabReader                                           "_ZN6SQLDBC24SQLDBC_PreparedStatement13getItabReaderEv"
#define SQLDBC_SQLDBC_PreparedStatement_bindParameter_unsigned_int_SQLDBC_HostType_void_long_long_long_long_bool "_ZN6SQLDBC24SQLDBC_PreparedStatement13bindParameterEj15SQLDBC_HostTypePvPxxb"
#define SQLDBC_SQLDBC_PreparedStatement_execute                                                 "_ZN6SQLDBC24SQLDBC_PreparedStatement7executeEv"
#define SQLDBC_SQLDBC_Statement_getResultSet                                                    "_ZN6SQLDBC16SQLDBC_Statement12getResultSetEv"
#define ThRqSetCurrentRequest                                                                   (0x540b71)

void test();

extern "C" {
////////////////////////////////////////////////////////////////////
typedef
SQLDBC_Retcode (*PrepareFun)(SQLDBC::SQLDBC_PreparedStatement *self,
                             const char *sql,
                             const SQLDBC_StringEncoding encoding);

PrepareFun prepare_old = nullptr;
SQLDBC_Retcode prepare_new(SQLDBC::SQLDBC_PreparedStatement *self,
                           const char *sql,
                           const SQLDBC_StringEncoding encoding) {
    SQLDBC_Retcode ret = SQLDBC_NOT_OK;

    wprintf(L"Call prepare_new: %S\n", (wchar_t*)sql);

    g_thePreparedStatementMgr->AddedStatementSQL(self, sql, encoding);


    ret = (prepare_old)(self,  sql, encoding);

    return ret;
}
////////////////////////////////////////////////////////////////////
typedef
SQLDBC_Retcode (*ExecuteItabFun)(SQLDBC::SQLDBC_PreparedStatement *,
                              void*, bool);

ExecuteItabFun execute_itab_old = nullptr;
SQLDBC_Retcode execute_itab_new(SQLDBC::SQLDBC_PreparedStatement *self,
                                void *p, bool b);
////////////////////////////////////////////////////////////////////
typedef
void* (*GetItabReaderFun)(SQLDBC::SQLDBC_PreparedStatement *);

GetItabReaderFun get_itab_reader_old = nullptr;
void* get_itab_reader_new(SQLDBC::SQLDBC_PreparedStatement *self) {
    void* pEnforceStatement = g_thePreparedStatementMgr->FindEnforcerStatement(self);
    if (pEnforceStatement)
    {
        void* pRet =  get_itab_reader_old((SQLDBC::SQLDBC_PreparedStatement *)pEnforceStatement);
        //g_theLog->WriteLog(0, "My_SQLDBC_PreparedStatement_getItabReader pthis=%p, return enforcre void*=%p", pThis, pRet);
        return pRet;
    }
    else
    {
        return get_itab_reader_old(self);
    }

    return NULL;
}
////////////////////////////////////////////////////////////////////
typedef
SQLDBC_Retcode (*BindParameterFun) (SQLDBC::SQLDBC_PreparedStatement *,
                              const SQLDBC_UInt2     Index,
                              const SQLDBC_HostType  Type,
                              void                  *paramAddr,
                              SQLDBC_Length         *LengthIndicator,
                              const SQLDBC_Length    Size,
                              const SQLDBC_Bool      Terminate);
BindParameterFun bind_parameter_old = nullptr;
SQLDBC_Retcode bind_parameter_new(SQLDBC::SQLDBC_PreparedStatement *self,
                                    const SQLDBC_UInt4     Index,
                                    const SQLDBC_HostType  Type,
                                    void                  *paramAddr,
                                    SQLDBC_Length         *LengthIndicator,
                                    const SQLDBC_Length    Size,
                                    const SQLDBC_Bool      Terminate) {
    wprintf(L"Call bind_parameter_new\n");
    //g_thePreparedStatementMgr->AddedStatementParam(self, Index, Type, paramAddr, LengthIndicator, Size, Terminate);
    return bind_parameter_old(self, Index, Type, paramAddr, LengthIndicator, Size, Terminate);
}
///////////////////////////////////////////////////////////////////
typedef
SQLDBC_Retcode (*ExecuteFun)(SQLDBC::SQLDBC_PreparedStatement *);
ExecuteFun execute_old = nullptr;
SQLDBC_Retcode execute_new(SQLDBC::SQLDBC_PreparedStatement *self) {
    wprintf(L"Call execute_new\n");
    return execute_old(self);
}
/////////////////////////////////////////////////////////////////////////////

typedef
SQLDBC::SQLDBC_ResultSet *(*GetResultSetFun)(SQLDBC::SQLDBC_PreparedStatement *);
GetResultSetFun get_result_set_old = nullptr;
SQLDBC::SQLDBC_ResultSet *get_result_set_new(SQLDBC::SQLDBC_PreparedStatement *self) {
    wprintf(L"getpid:%d, Call get_result_set_new\n", getpid());
    return get_result_set_old(self);
}
/////////////////////////////////////////////////////////////////////////////
typedef
void (*ThRqSetCurrentRequestFun)(struct REQUEST_BUF *, unsigned int);
ThRqSetCurrentRequestFun thrq_set_current_request_old = nullptr;
void thrq_set_current_request_new(struct REQUEST_BUF *req, unsigned int d) {
    wprintf(L"getpid:%d, Call thrq_set_current_request_new\n", getpid());
    return thrq_set_current_request_old(req, d);
}


void init_log(const std::string& file) {
    if (file.length() > 0) freopen(file.c_str(), "w", stdout);
}

__attribute__((constructor))
void loadMsg() {
    test();
    init_log(TERMINAL_OUT_PUT);
    wprintf(L"So file inject success\n");
    wprintf(L"Hook begin\n");

    g_thePreparedStatementMgr = new CPreparedStatementInfoMgr;
    prepare_old = (PrepareFun)install_hook(dlsym(RTLD_DEFAULT, SQLDBC_SQLDBC_PreparedStatement_prepare_char_const_SQLDBC_StringEncodingType_Encoding),
                               (void*)prepare_new, HOOK_BY_SUBHOOK);
    if (prepare_old == nullptr) {
        wprintf(L"Hook fail with prepare = NIL\n");
        return;
    }

    execute_itab_old = (ExecuteItabFun)install_hook(dlsym(RTLD_DEFAULT, SQLDBC_SQLDBC_PreparedStatement_executeItab_void_bool),
                                                    (void*)execute_itab_new, HOOK_BY_FUNCHOOK);
    if (execute_itab_old == nullptr) {
        wprintf(L"Hook fail with execute_itab = NIL\n");
        return;
    }

    get_itab_reader_old = (GetItabReaderFun)install_hook(dlsym(RTLD_DEFAULT, SQLDBC_SQLDBC_PreparedStatement_getItabReader),
                                                         (void*)get_itab_reader_new, HOOK_BY_FUNCHOOK);
    if (get_itab_reader_old == nullptr) {
        wprintf(L"Hook fail with get_itab_reader = NIL\n");
        return;
    }

    bind_parameter_old = (BindParameterFun)install_hook(dlsym(RTLD_DEFAULT, SQLDBC_SQLDBC_PreparedStatement_bindParameter_unsigned_int_SQLDBC_HostType_void_long_long_long_long_bool),
                                                        (void*)bind_parameter_new, HOOK_BY_FUNCHOOK);
    if (bind_parameter_old == nullptr) {
        wprintf(L"Hook fail with bind_parameter = NIL\n");
        return;
    }

    execute_old = (ExecuteFun)install_hook(dlsym(RTLD_DEFAULT, SQLDBC_SQLDBC_PreparedStatement_execute),
                                           (void*)execute_new, HOOK_BY_FUNCHOOK);
    if (execute_old == nullptr) {
        wprintf(L"Hook fail with execute = NIL\n");
        return;
    }

    get_result_set_old = (GetResultSetFun)install_hook(dlsym(RTLD_DEFAULT, SQLDBC_SQLDBC_Statement_getResultSet),
                                                        (void*)get_result_set_new, HOOK_BY_FUNCHOOK);
    if (get_result_set_old == nullptr) {
        wprintf(L"Hook fail with get_result_set = NIL\n");
        return;
    }

    /*
    void *tmp = nullptr;
    if ((tmp = dlopen(nullptr, RTLD_NOW)) != nullptr) {
        printf("disp+work: %p\n", tmp);
        thrq_set_current_request_old = (ThRqSetCurrentRequestFun)install_hook((unsigned char*)tmp+ThRqSetCurrentRequest,
                                                                              (void*)thrq_set_current_request_new, HOOK_BY_FUNCHOOK);
    }
    else {
        printf("Hook fail with dlopen(disp+work, RTLD_NOW) = NIL\n");
        return;
    }
*/
    wprintf(L"Hook success\n");
}



}

std::wstring DoenforcerForStatementOnExecute(void* pStatement, const StatementSQL* pStateSQL);

SQLDBC_Retcode execute_itab_new(SQLDBC::SQLDBC_PreparedStatement *self,
                                void *p, bool b) {
    wprintf(L"My_SQLDBC_PreparedStatement_executeItab_3,pState:%p, 2p:%p, 3p:%d\n", self, p, b);

    //enforcer, find sql and param
    const StatementSQL* pStateSQL = g_thePreparedStatementMgr->FindStatementSQL(self);
    if (NULL != pStateSQL)
    {
        std::wstring strNewSql = DoenforcerForStatementOnExecute(self, pStateSQL);

        //do enforcer
        if (strNewSql.compare(pStateSQL->_wstrSQL) != 0)
        {
//            theLog->WriteLog(0, "My_SQLDBC_PreparedStatement_executeItab_3 do enforcer.");

            //create new object of PreparedStatement
            //get connection
            auto pConn = self->getConnection();
            auto pEnforceStatement = pConn->createPreparedStatement();
            if (pEnforceStatement)
            {
 //               theLog->WriteLog(0, "My_SQLDBC_PreparedStatement_executeItab_3 create enforce statement success");

                //prepare sql
                SQLDBC_Retcode enforceRet = prepare_old(pEnforceStatement, (const char*)strNewSql.c_str(), pStateSQL->_encoding);
 //               theLog->WriteLog(0, "My_SQLDBC_PreparedStatement_executeItab_3 prepare new sql result:%d", enforceRet);
                if (enforceRet == SQLDBC_OK)
                {
                    //bind parameters
                    const LIST_STATEMENT_PARAM* pStateParams = g_thePreparedStatementMgr->FindStatementParam(self);
                    if (pStateParams != NULL)
                    {
                        auto itParam = pStateParams->begin();
                        while (itParam != pStateParams->end())
                        {
                            auto pParam = *itParam;
                            enforceRet = bind_parameter_old(pEnforceStatement, pParam->_Index,
                                                            pParam->_Type, pParam->_paramAddr, pParam->_LengthIndicator, pParam->_Size, pParam->_Terminate);

 //                           theLog->WriteLog(0, "My_SQLDBC_PreparedStatement_executeItab_3 bind param, index:%d, result:%d", pParam->_Index, enforceRet);
                            itParam++;
                        }
                    }

                    //execute
                    enforceRet = pEnforceStatement->executeItab(p, b);
 //                   theLog->WriteLog(0, "My_SQLDBC_PreparedStatement_executeItab_3 bind param, execute enforce sql result:%d", enforceRet);
                    if (enforceRet == SQLDBC_OK)
                    {
                        //
                        g_thePreparedStatementMgr->AddedEnforceStatement(self, pEnforceStatement);
 //                       theLog->WriteLog(0, "My_SQLDBC_PreparedStatement_executeItab_3 added enforcer statement");
                    }

                }
            }
        }
        else
        {
            //remove enforcer statement
//            theLog->WriteLog(0, "My_SQLDBC_PreparedStatement_executeItab_3 no need do enforcer.");

            g_thePreparedStatementMgr->RemoveEnforceStatement(self);
        }

    }
    //call next
    return  execute_itab_old(self, p, b);
}

std::wstring GetTable(const std::wstring src)
{
    if (src.find(L"SELECT") == std::string::npos && src.find(L"select") == std::string::npos) {
        return L"";
    }

    const std::wstring BUT0BK = L"BUT0BK";
    const std::wstring BUT0BK_FIND = L"FROM \"BUT0BK\"";

    const std::wstring BNKA = L"BNKA";
    const std::wstring BNKA_FIND = L"FROM \"BNKA\"";

    const std::wstring T012 = L"T012";
    const std::wstring T012_FIND = L"FROM \"T012\"";

    const std::wstring BUT000 = L"BUT000";
    const std::wstring BUT000_FIND = L"FROM \"BUT000\"";


    std::wstring table_name;
    if (src.find(BUT0BK_FIND) != std::string::npos) {
        table_name = BUT0BK;
    }
    else if (src.find(BNKA_FIND) != std::string::npos) {
        table_name = BNKA;
    }
    else if (src.find(T012_FIND) != std::string::npos) {
        table_name = T012;
    }
    else if (src.find(BUT000_FIND) != std::string::npos) {
        table_name = BUT000;
    }


    return table_name;
}

struct MaskConditions
{
    std::wstring _field;
    std::wstring _op;
    std::wstring _value;
};


struct MaskFields {
    std::wstring _c2character;
    std::vector<std::wstring> _fields;
    std::list<MaskConditions>  _condi;
};

std::wstring MaskSQLFieldWithCondition(const std::wstring& src, const MaskFields& mf)
{
    std::wstring r = src;
    size_t find_wild;

    std::wstring strMaksValueFmt = L"CASE WHEN %S THEN '***' ELSE %S END AS %S ";// L"'" + mf._c2character + L"' ";

    auto fields = mf._fields;
    auto itField = fields.begin();
    while (itField != fields.end() )
    {
        std::wstring fname = L"\"" + *itField + L"\"";

        if ((find_wild = r.find(fname)) != std::string::npos) {

            wchar_t* pszFnameMask = new wchar_t[src.length() + 1024];

            //construct conditions
            std::wstring strConditions;
            if (mf._condi.empty())
            {
                strConditions = L"1=1";
            }
            else
            {
                auto itCon = mf._condi.begin();
                while (itCon != mf._condi.end())
                {
                    if (!strConditions.empty())
                    {
                        strConditions += L" OR ";
                    }
                    strConditions += itCon->_field + itCon->_op + L"'" + itCon->_value + L"'";
                    itCon++;
                }
            }


            swprintf(pszFnameMask, src.length() + 1024, strMaksValueFmt.c_str(), strConditions.c_str(), fname.c_str(), fname.c_str());
            std::wstring fnameMask = pszFnameMask;
            delete[] pszFnameMask;
            pszFnameMask = NULL;

            r.replace(find_wild, fname.length() , fnameMask.c_str() );
        }


        itField++;
    }
    wprintf(r.c_str()); wprintf(L"\n");
    return r;
}

std::wstring DoenforcerForStatementOnExecute(void* pStatement, const StatementSQL* pStateSQL)
{
    std::wstring strNewSql;

    if (pStateSQL != NULL)
    {
        //get from table
        const std::wstring& strSql = pStateSQL->_wstrSQL;
        std::wstring strTableName = GetTable(strSql);


        if (!strTableName.empty())
        {
            MaskFields mf;

            if (strTableName == L"BUT0BK") {
                mf._c2character = L"***";
                mf._fields = {L"BANKL"};
            }

            //do mask
            strNewSql = MaskSQLFieldWithCondition(strSql, mf);

            std::wstring wstrField;
            std::vector<std::wstring>::iterator itField = mf._fields.begin();
            while (itField != mf._fields.end())
            {
                wstrField += itField->c_str();
                wstrField += L",";
                itField++;
            }

            std::wstring wstrCondi;
            auto itCondi = mf._condi.begin();
            while (itCondi != mf._condi.end())
            {
                wstrCondi += itCondi->_field + itCondi->_op + itCondi->_value + L",";
                itCondi++;
            }
//            theLog->WriteLog(0, L"DoenforcerForStatementOnExecute mask field:%s, condi:%s, new sql:%s", wstrField.c_str(), wstrCondi.c_str(), strNewSql.c_str());
        }
    }

    return strNewSql;
}

void test() {
    MaskFields mf;
    mf._c2character = L"***";
    mf._fields = {L"BANKL"};

    //do mask
    auto strNewSql = MaskSQLFieldWithCondition(L"SELECT \"BANKL\" FROM BUT0BK", mf);
}