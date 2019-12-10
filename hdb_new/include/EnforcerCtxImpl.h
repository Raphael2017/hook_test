#ifndef ENFORCER_CTX_IMPL_H
#define ENFORCER_CTX_IMPL_H

#include "interface.h"
#include <map>

class EnforcerCtxImpl;

class BindParameters {
public:
    BindParameters(const SQLDBC_UInt4     Index,
                   const SQLDBC_HostType  Type,
                   void                  *paramAddr,
                   SQLDBC_Length         *LengthIndicator,
                   const SQLDBC_Length    Size,
                   const SQLDBC_Bool      Terminate);
    ~BindParameters();
private:
    const SQLDBC_UInt4     _Index;
    const SQLDBC_HostType  _Type;
    unsigned char         *_paramAddr;
    SQLDBC_Length         *_LengthIndicator;
    const SQLDBC_Length    _Size;
    const SQLDBC_Bool      _Terminate;
    friend class EnforcerCtxImpl;
};

class EnforcerCtxImpl : public IEnforcerCtx {
public:
    EnforcerCtxImpl(void *original_stmt, const std::u16string& original_sql);
    virtual void RecordParameter(const SQLDBC_UInt4     Index,
                                 const SQLDBC_HostType  Type,
                                 void                  *paramAddr,
                                 SQLDBC_Length         *LengthIndicator,
                                 const SQLDBC_Length    Size,
                                 const SQLDBC_Bool      Terminate) override;
    virtual bool EnforcedStmtBindParameters(S4HException& e) override;
    virtual void *GetOriginalStmt() override { return _original_stmt; }
    virtual void SetEnforcedStmt(void *s) override { _enforced_stmt = (SQLDBC::SQLDBC_PreparedStatement*)s; }
    virtual void *GetEnforcedStmt() override { return _enforced_stmt; }
    virtual std::u16string GetOriginalSql() override { return _original_sql; }

private:
    void*                                       _original_stmt;
    std::u16string                              _original_sql;
    std::map<SQLDBC_UInt4, BindParameters*>     _bind_parameters;
    SQLDBC::SQLDBC_PreparedStatement*           _enforced_stmt;
};

#endif