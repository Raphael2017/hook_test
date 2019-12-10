#include "microtest.h"
#include "interface.h"

#include <unordered_map>

TEST(MASK_STRATEGY) {
    std::vector<IMaskStrategy*> strategys = query_pc();
    std::set<std::u16string> r{ u"BANKL", u"ACCNAME", u"CLIENT" };
    ASSERT_TRUE(strategys[0]->GetMaskedColumn() == u"BANKL");
    ASSERT_TRUE(strategys[0]->GetReferencedColumn() == r);
    std::u16string expression = strategys[0]->MakeExpression(u"BUT0BK", u"");
    ASSERT_TRUE(expression.length() > 0);

    ASSERT_TRUE(strategys[1]->GetMaskedColumn() == u"BANKN");
    ASSERT_TRUE(strategys[1]->GetReferencedColumn() == std::set<std::u16string>{u"BANKN"});
    expression = strategys[1]->MakeExpression(u"", u"");
    ASSERT_TRUE(expression.length() > 0);


}