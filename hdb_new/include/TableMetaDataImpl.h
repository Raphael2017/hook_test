#ifndef TABLE_META_DATA_IMPL_H
#define TABLE_META_DATA_IMPL_H

#include <vector>
#include <string>
#include "interface.h"

struct MetaDataItem {
    enum COLUMN_TYPE { COLUMN_TYPE_STRING, COLUMN_TYPE_NUMBER, COLUMN_TYPE_OTHER };
    COLUMN_TYPE _column_type;
    std::u16string _column_name;
};

class TableMetaDataImpl : public ITableMetaData {
public:
    virtual std::u16string GetTableName() override;
    virtual bool HasColumn(const std::u16string& column_name) override;
    virtual bool IsColumnCompatibleWithString(const std::u16string& column_name) override;
    virtual std::vector<std::u16string> GetAllColumns() override;
private:
    std::vector<MetaDataItem>   _meta_data;
    std::u16string              _table_name;
    friend ITableMetaData *create_table_metadata(void *connection, const std::u16string& table_name, S4HException& e);
};

#endif