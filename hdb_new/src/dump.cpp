#include "interface.h"
#include "MaskStrategyImpl.h"
#include "tool.h"

std::vector<IMaskStrategy*> query_pc(/* todo */) {
    MaskStrategyImpl *strategy = new MaskStrategyImpl;
    strategy->_masked_column = u"BANKL";
    strategy->_conditions = {
            {u"ACCNAME", SQLCondition::SQL_EQ, u"TOM"},
            {u"CLIENT", SQLCondition::SQL_LT, u"123"}
    };

    MaskStrategyImpl *strategy1 = new MaskStrategyImpl;
    strategy1->_masked_column = u"BANKN";
    return { strategy, strategy1 };
}

class AstStmtDumpImpl : public IAstStmt {
public:
    virtual bool CheckSupport() override {
        return true;
    }
    /*
     * return : is the ast really modified
     * */
    virtual bool RewriteWithMaskStrategy(void* connection, const std::vector<IMaskStrategy*>& mask_strategys) override {
        return true;
    }

    virtual std::u16string Output() override {
        return _new_sql;
    }
    std::u16string _ori_sql;
    std::u16string _new_sql;
};

IAstStmt *parse_sql(const std::u16string& sql, S4HException& e) {
    AstStmtDumpImpl *stmt = new AstStmtDumpImpl;
    stmt->_ori_sql = sql;

    std::u16string cols = u" MANDT,PARTNER,BKVID,BANKS,'***' AS BANKL,BANKN,BKONT,BKREF,KOINH,BKEXT,XEZER,ACCNAME,MOVE_BKVID,BK_VALID_FROM,BK_VALID_TO,BK_MOVE_DATE,IBAN,BANK_GUID,ACCOUNT_ID,CHECK_DIGIT,ACCOUNT_TYPE,BP_EEW_BUT0BK ";
    size_t pos = sql.rfind(u"*");
    if (pos == std::u16string::npos) {
        stmt->_new_sql = sql;
    } else {
        stmt->_new_sql = sql;
        stmt->_new_sql.replace(pos, 1, cols);

        auto f = fopen("/home/he4adm/github/log_hook.txt", "a");
        std::string str_utf8(stmt->_new_sql.begin(), stmt->_new_sql.end());
        fprintf(f != nullptr ? f : stdout, "NEW SQL: %s\n", str_utf8.c_str());
        if (f != nullptr) fclose(f);
    }
    return stmt;
}

bool is_sql_need_mask(const std::u16string& sql) {
    return sql.find(u"FROM \"BUT0BK\"") != std::u16string::npos;
}