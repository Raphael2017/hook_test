#include "EnforcerCtxImpl.h"
#include "string.h"
#include "tool.h"
#include <mutex>

BindParameters::BindParameters(const SQLDBC_UInt4 Index, const SQLDBC_HostType Type, void *paramAddr,
        SQLDBC_Length *LengthIndicator, const SQLDBC_Length Size, const SQLDBC_Bool Terminate) :
        _Index(Index), _Type(Type), _paramAddr(nullptr), _LengthIndicator(LengthIndicator), _Size(Size), _Terminate(Terminate) {
    if (_Size > 0) {
        _paramAddr = new unsigned char[_Size];
        memcpy(_paramAddr, paramAddr, Size);
    } else {
        /* todo http://maxdb.sap.com/documentation/sqldbc/SQLDBC_API/index.html */
    }
}

BindParameters::~BindParameters() {
    if (_paramAddr != nullptr) {
        delete[] (_paramAddr);
        _paramAddr = nullptr;
    }
}

EnforcerCtxImpl::EnforcerCtxImpl(void *original_stmt, const std::u16string& original_sql) :
    _original_stmt(original_stmt), _original_sql(original_sql), _enforced_stmt(nullptr) {}

void EnforcerCtxImpl::RecordParameter(const SQLDBC_UInt4     Index,
                     const SQLDBC_HostType  Type,
                     void                  *paramAddr,
                     SQLDBC_Length         *LengthIndicator,
                     const SQLDBC_Length    Size,
                     const SQLDBC_Bool      Terminate) {
    auto fd = _bind_parameters.find(Index);
    if (fd == _bind_parameters.end()) {
        _bind_parameters.insert(std::make_pair(Index, new BindParameters(Index, Type, paramAddr, LengthIndicator, Size, Terminate)));
    } else {
        delete (fd->second);
        fd->second = new BindParameters(Index, Type, paramAddr, LengthIndicator, Size, Terminate);
    }
}

bool EnforcerCtxImpl::EnforcedStmtBindParameters(S4HException& e) {
    if (_enforced_stmt == nullptr) {
        e._code = S4H_BIND_PARAM_ERROR;
        e._detail = u"_enforced_stmt is NIL!";
        return false;
    }
    SQLDBC_Retcode rc = SQLDBC_OK;
    for (auto it : _bind_parameters) {
        rc = bind_parameter_old(_enforced_stmt, it.second->_Index, it.second->_Type, it.second->_paramAddr, it.second->_LengthIndicator, it.second->_Size, it.second->_Terminate);
        if (rc != SQLDBC_OK) {
            e._code = S4H_BIND_PARAM_ERROR;
            CommonFun::FromUTF8(_enforced_stmt->error().getErrorText(), e._detail);
            return false;
        }
    }

    e._code = S4H_SUCCESS;
    return true;
}

std::map<void*, IEnforcerCtx*> BINDINGS;
std::mutex mtx;

IEnforcerCtx *register_enforcer_ctx(void *original_stmt, const std::u16string& sql) {
    IEnforcerCtx *ret = nullptr;
    mtx.lock();
    auto fd = BINDINGS.find(original_stmt);
    if (fd == BINDINGS.end()) {
        ret = new EnforcerCtxImpl(original_stmt, sql);
        BINDINGS.insert(std::make_pair(original_stmt, ret));
    } else {
        ret = fd->second;
    }
    mtx.unlock();
    return ret;
}

IEnforcerCtx *get_enforcer_ctx(void *original_stmt) {
    IEnforcerCtx *ret = nullptr;
    mtx.lock();
    auto fd = BINDINGS.find(original_stmt);
    if (fd != BINDINGS.end()) ret = fd->second;
    mtx.unlock();
    return ret;
}

