#ifndef INTERFACE_H
#define INTERFACE_H

#include <string>
#include <vector>
#include <set>
#include "talk_with_hdb.h"

#define S4H_SUCCESS 0
#define S4H_INVALID_CONNECTION -1
#define S4H_INVALID_TABLE_NAME -2
#define S4H_BIND_PARAM_ERROR -3
#define S4H_SQL_PARSE_FAIL -4

struct S4HException {
    int _code;
    std::u16string _detail;
};

/*
 * x86_64  SUSE  OK (todo 0.5days to tidy up the poc code)
 * ppc todo (expect 10 days can solve this problem, need more investigation)
 * Currently, we can hook the 6 api of libSQLDBC on cpu x86_64 and os SUSE
 * */
class IHook {
public:
    enum HOOK_TYPE { HOOK_BY_SUBHOOK, HOOK_BY_FUNCHOOK /* todo extension */ };
    virtual ~IHook() { }
    virtual void *Install(void *old_addr, void *new_addr) = 0;
};

IHook *create_hook(IHook::HOOK_TYPE);

/*
 * map the terminal user
 * Currently, we can't get the terminal user
 * todo (expect 4 days can solve this problem, need more investigation)
 * */

/*
 * this module need to use the current connection and hana db sql to fetch the table or view's column name and type
 * need 1 days
 * */
class ITableMetaData {
public:
    virtual ~ITableMetaData() { }
    virtual std::u16string GetTableName() = 0;
    virtual bool HasColumn(const std::u16string& column_name) = 0;
    virtual bool IsColumnCompatibleWithString(const std::u16string& column_name) = 0;
    virtual std::vector<std::u16string> GetAllColumns() = 0;
};

ITableMetaData *create_table_metadata(void *connection, const std::u16string& table_name, S4HException& e);

/*
 * this module need to run queryPC cpp sdk on linux
 * so need 2 days
 * */
class IMaskStrategy {
public:
    virtual ~IMaskStrategy() {}
    virtual std::u16string GetMaskedColumn() = 0;
    // include the condition's column ref and the masked column
    virtual std::set<std::u16string> GetReferencedColumn() = 0;
    virtual std::u16string MakeExpression(const std::u16string& scope, const std::u16string& alias) = 0;
    /* todo */
};

std::vector<IMaskStrategy*> query_pc(/* todo */);

/*
 * this module is the rewrite sql module
 * contains a simple hana sql lexer conponent, parser component, and apply mask strategy component
 * need 8 days
 * */
class IAstStmt {
public:
    virtual ~IAstStmt() { }
    /*
     *
     * */
    virtual bool CheckSupport() = 0;
    /*
     * return : is the ast really modified
     * */
    virtual bool RewriteWithMaskStrategy(void* connection, const std::vector<IMaskStrategy*>& mask_strategys) = 0;
    virtual std::u16string Output() = 0;
};

IAstStmt *parse_sql(const std::u16string& sql, S4HException& e);

/*
 * this module maintain the enforcer context
 * need 1 days
 * */
class IEnforcerCtx {
public:
    virtual ~IEnforcerCtx() {  }
    virtual void RecordParameter(const SQLDBC_UInt4     Index,
                                 const SQLDBC_HostType  Type,
                                 void                  *paramAddr,
                                 SQLDBC_Length         *LengthIndicator,
                                 const SQLDBC_Length    Size,
                                 const SQLDBC_Bool      Terminate) = 0;
    virtual bool EnforcedStmtBindParameters(S4HException& e) = 0;
    virtual void *GetOriginalStmt() = 0;
    virtual void SetEnforcedStmt(void *s) = 0;
    virtual void *GetEnforcedStmt() = 0;
    virtual std::u16string GetOriginalSql() = 0;
};

IEnforcerCtx *register_enforcer_ctx(void *original_stmt, const std::u16string& sql);
IEnforcerCtx *get_enforcer_ctx(void *original_stmt);



/*
 * todo
 * We need a sql prefilter
 * use hana sql lexer analyze and blacklist, white list inplement it
 * need 0.5~1 days
 * */
bool is_sql_need_mask(const std::u16string& sql);



#endif