#include "MaskStrategyImpl.h"

std::set<std::u16string> MaskStrategyImpl::GetReferencedColumn() {
    std::set<std::u16string> ret;
    ret.insert(_masked_column);
    for (auto it : _conditions) {
        ret.insert(it._column);
    }
    return ret;
}

std::u16string make_column(const std::u16string& scope, const std::u16string& column) {
    if (scope.length() == 0) return column;
    else return scope + u"." + column;
}

std::u16string make_op(SQLCondition::SQL_OP op) {
    switch (op) {
        case SQLCondition::SQL_GTEQ:    return u" >= ";
        case SQLCondition::SQL_GT:      return u" > ";
        case SQLCondition::SQL_LTEQ:    return u" <= ";
        case SQLCondition::SQL_LT:      return u" < ";
        case SQLCondition::SQL_EQ:      return u" = ";
        case SQLCondition::SQL_NEQ:     return u" <> ";
        default: { /* not reached */ }
    }
    return u"";
}

std::u16string make_predicate(const std::u16string& scope, const SQLCondition& condition) {
    return make_column(scope, condition._column) + make_op(condition._op) + u"'" + condition._constant_value + u"'";
}

std::u16string MaskStrategyImpl::MakeExpression(const std::u16string& scope, const std::u16string& alias) {
    std::u16string alias_name = alias;
    if (alias_name.length() == 0) {
        alias_name = _masked_column;
    }
    if (_conditions.size() > 0) {
        // "CASE WHEN %s THEN '***' ELSE %s END AS %s "
        std::u16string conditions;
        for (int i = 0; i < _conditions.size(); ++i) {
            if (i > 0) {
                conditions += u" OR ";
            }
            conditions += make_predicate(scope, _conditions[i]);
        }
        return u" CASE WHEN " + conditions + u" THEN '***' ELSE " + make_column(scope, _masked_column) + u" END AS " + alias_name + u" ";
    } else {
        return u" '***' AS " + alias_name + u" ";
    }
}