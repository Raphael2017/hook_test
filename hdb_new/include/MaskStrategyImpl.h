#ifndef MASK_STRATEGY_IMPL_H
#define MASK_STRATEGY_IMPL_H

#include "interface.h"

struct SQLCondition {
    enum SQL_OP {SQL_GT, SQL_GTEQ, SQL_LT, SQL_LTEQ, SQL_EQ, SQL_NEQ} ;
    std::u16string                      _column;
    SQL_OP                              _op;
    std::u16string                      _constant_value;
};

class MaskStrategyImpl : public IMaskStrategy {
public:
    virtual std::u16string GetMaskedColumn() override { return _masked_column; }
    virtual std::set<std::u16string> GetReferencedColumn() override;
    virtual std::u16string MakeExpression(const std::u16string& scope, const std::u16string& alias) override;
private:
    std::u16string                  _masked_column;
    //std::u16string              _star;
    std::vector<SQLCondition>       _conditions;
    friend std::vector<IMaskStrategy*> query_pc(/* todo */);
};

#endif