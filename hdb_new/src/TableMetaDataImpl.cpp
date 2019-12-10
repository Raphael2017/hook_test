#include "TableMetaDataImpl.h"
#include "tool.h"
#include "talk_with_hdb.h"

std::u16string TableMetaDataImpl::GetTableName() {
    return _table_name;
}

bool TableMetaDataImpl::HasColumn(const std::u16string& column_name) {
    for (auto& it : _meta_data) {
        if (CommonFun::StrCaseCmp(it._column_name, column_name) == 0) return true;
    }
    return false;
}

bool TableMetaDataImpl::IsColumnCompatibleWithString(const std::u16string& column_name) {
    for (auto& it : _meta_data) {
        if (CommonFun::StrCaseCmp(it._column_name, column_name) == 0) return it._column_type == MetaDataItem::COLUMN_TYPE_STRING;
    }
    return false;
}

std::vector<std::u16string> TableMetaDataImpl::GetAllColumns() {
    std::vector<std::u16string> ret;
    for (auto& it : _meta_data) ret.push_back(it._column_name);
    return ret;
}

MetaDataItem::COLUMN_TYPE get_type(const std::u16string& data_type_name) {
    /* todo */
    if (0 == CommonFun::StrCaseCmp(data_type_name, u"VARCHAR")
        || 0 == CommonFun::StrCaseCmp(data_type_name, u"NVARCHAR")) {
        return  MetaDataItem::COLUMN_TYPE_STRING;
    } else if (0 == CommonFun::StrCaseCmp(data_type_name, u"TINYINT")
        || 0 == CommonFun::StrCaseCmp(data_type_name, u"SMALLINT")
        || 0 == CommonFun::StrCaseCmp(data_type_name, u"BIGINT")
        || 0 == CommonFun::StrCaseCmp(data_type_name, u"INTEGER")) {
        return MetaDataItem::COLUMN_TYPE_NUMBER;
    } else {
        return MetaDataItem::COLUMN_TYPE_OTHER;
    }
}

ITableMetaData *create_table_metadata(void *connection, const std::u16string& table_name, S4HException& e) {
    if (nullptr == connection) {
        e._code = S4H_INVALID_CONNECTION;
        e._detail = u"INVALID CONNECTION!";
        return nullptr;
    }
    if (table_name.length() == 0) {
        e._code = S4H_INVALID_TABLE_NAME;
        e._detail = u"INVALID TABLE NAME!";
        return nullptr;
    }
    auto hdb_connection = (SQLDBC::SQLDBC_Connection*)connection;
    auto hdb_stmt = hdb_connection->createPreparedStatement();

    std::u16string query = u"SELECT COLUMN_NAME, DATA_TYPE_NAME\n"
                           "    FROM (     SELECT COLUMN_NAME, POSITION, DATA_TYPE_NAME FROM SYS.TABLE_COLUMNS WHERE SCHEMA_NAME = CURRENT_USER AND TABLE_NAME =  ?\n"
                           "        UNION  SELECT COLUMN_NAME, POSITION, DATA_TYPE_NAME FROM SYS.VIEW_COLUMNS  WHERE SCHEMA_NAME = CURRENT_USER AND VIEW_NAME =  ?  )\n"
                           "            ORDER BY POSITION;";
    SQLDBC_Retcode rc = prepare_old(hdb_stmt, (char*)query.c_str(), SQLDBC_StringEncodingType::UCS2Swapped);
    if (SQLDBC_OK != rc) {
        hdb_connection->releaseStatement(hdb_stmt);
        e._code = S4H_INVALID_CONNECTION;
        CommonFun::FromUTF8(hdb_stmt->error().getErrorText(), e._detail);
        return nullptr;
    }

    rc = bind_parameter_old(hdb_stmt, 1, SQLDBC_HOSTTYPE_UCS2_SWAPPED, (void*)table_name.c_str(), nullptr, 0, SQLDBC_TRUE);
    if (SQLDBC_OK != rc) {
        e._code = S4H_INVALID_CONNECTION;
        CommonFun::FromUTF8(hdb_stmt->error().getErrorText(), e._detail);
        hdb_connection->releaseStatement(hdb_stmt);
        return nullptr;
    }
    rc = bind_parameter_old(hdb_stmt, 2, SQLDBC_HOSTTYPE_UCS2_SWAPPED, (void*)table_name.c_str(), nullptr, 0, SQLDBC_TRUE);
    if (SQLDBC_OK != rc) {
        e._code = S4H_INVALID_CONNECTION;
        CommonFun::FromUTF8(hdb_stmt->error().getErrorText(), e._detail);
        hdb_connection->releaseStatement(hdb_stmt);
        return nullptr;
    }
    rc = execute_old(hdb_stmt);
    if (SQLDBC_OK != rc) {
        e._code = S4H_INVALID_CONNECTION;
        CommonFun::FromUTF8(hdb_stmt->error().getErrorText(), e._detail);
        hdb_connection->releaseStatement(hdb_stmt);
        return nullptr;
    }
    auto result_set = get_result_set_old(hdb_stmt);
    if (result_set == nullptr) {
        e._code = S4H_INVALID_CONNECTION;
        CommonFun::FromUTF8(hdb_stmt->error().getErrorText(), e._detail);
        hdb_connection->releaseStatement(hdb_stmt);
        return nullptr;
    }
    rc = result_set->next();
    if(SQLDBC_OK != rc) {
        e._code = S4H_INVALID_TABLE_NAME;
        CommonFun::FromUTF8(hdb_stmt->error().getErrorText(), e._detail);
        hdb_connection->releaseStatement(hdb_stmt);
        return nullptr;
    }
    TableMetaDataImpl *impl = new TableMetaDataImpl;
    impl->_table_name = table_name;
    do {
        char16_t column_name[64] = { 0 };
        char16_t column_type_name[64] = { 0 };
        result_set->getObject(1, SQLDBC_HOSTTYPE_UCS2_SWAPPED, column_name, nullptr, sizeof(column_name));
        result_set->getObject(2, SQLDBC_HOSTTYPE_UCS2_SWAPPED, column_type_name, nullptr, sizeof(column_type_name));
        MetaDataItem item;
        item._column_name = column_name;
        item._column_type = get_type(column_type_name);
        impl->_meta_data.push_back(item);
    } while (result_set->next() == SQLDBC_OK);
    e._code == S4H_SUCCESS;
    hdb_connection->releaseStatement(hdb_stmt);
    return impl;
}





