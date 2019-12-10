#include "interface.h"
#include "MaskStrategyImpl.h"

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
    stmt->_new_sql = u"SELECT 'TEST' FROM DUMMY";
    return stmt;
}

bool is_sql_need_mask(const std::u16string& sql) {
    //return sql.find(u"FROM \"BUT0BK\"") != std::u16string::npos;
    bool r = sql.find(u"FROM DUMMY") != std::u16string::npos;
    return r;
}