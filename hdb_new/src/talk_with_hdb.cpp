#include "talk_with_hdb.h"
#include "interface.h"
#include <string>

SQLDBC_Retcode prepare_new(SQLDBC::SQLDBC_PreparedStatement *self,
                           const char *sql /* this is char16_t */,
                           const SQLDBC_StringEncoding encoding)
{
    std::u16string sql_str = (char16_t*)sql;
    if (is_sql_need_mask(sql_str)) {
        register_enforcer_ctx(self, sql_str);
    }
    return prepare_old(self, sql, encoding);
}

SQLDBC_Retcode execute_itab_new(SQLDBC::SQLDBC_PreparedStatement *self,
                                void *p,
                                bool b)
{
    /* todo */
    return execute_itab_old(self, p, b);
}

void* get_itab_reader_new(SQLDBC::SQLDBC_PreparedStatement *self) {
    /* todo */
    return get_itab_reader_old(self);
}

SQLDBC_Retcode bind_parameter_new(SQLDBC::SQLDBC_PreparedStatement *self,
                                  const SQLDBC_UInt4     Index,
                                  const SQLDBC_HostType  Type,
                                  void                  *paramAddr,
                                  SQLDBC_Length         *LengthIndicator,
                                  const SQLDBC_Length    Size,
                                  const SQLDBC_Bool      Terminate)
{
    IEnforcerCtx *ctx = get_enforcer_ctx(self);
    if (ctx != nullptr) {
        ctx->RecordParameter( /* todo */ );
    }
    bind_parameter_old(self, Index, Type, paramAddr, LengthIndicator, Size, Terminate);
}

SQLDBC_Retcode execute_new(SQLDBC::SQLDBC_PreparedStatement *self) {
    IEnforcerCtx *ctx = get_enforcer_ctx(self);
    auto connection = self->getConnection();
    if (ctx != nullptr && ctx->GetEnforcedStmt() != nullptr) {
        connection->releaseStatement((SQLDBC::SQLDBC_PreparedStatement*)ctx->GetEnforcedStmt());
        ctx->SetEnforcedStmt(nullptr);
    }
    do {
        if (ctx == nullptr) break;
        IAstStmt *ast = parse_sql(ctx->GetOriginalSql());
        if (ast == nullptr || !ast->CheckSupport()) break;
        std::vector<IMaskStrategy*> mask_strategys = query_pc( /* todo */ );
        bool r = ast->RewriteWithMaskStrategy(connection, mask_strategys);
        if (!r) break;
        std::u16string new_sql = ast->Output();
        auto enforced_stmt = connection->createPreparedStatement();
        ctx->SetEnforcedStmt(enforced_stmt);
        enforced_stmt->prepare((char*)new_sql.c_str());
        for (int i = 0; i < ctx->GetParameterCount(); ++i) {
            //enforced_stmt->bindParameter(/* todo */)
        }
        execute_old(enforced_stmt);
    } while (false);
    return execute_old(self);
}

SQLDBC::SQLDBC_ResultSet *get_result_set_new(SQLDBC::SQLDBC_PreparedStatement *self) {
    IEnforcerCtx *ctx = get_enforcer_ctx(self);
    if (ctx != nullptr && ctx->GetEnforcedStmt() != nullptr) {
        return get_result_set_old((SQLDBC::SQLDBC_PreparedStatement*)ctx->GetEnforcedStmt());
    }
    return get_result_set_old(self);
}