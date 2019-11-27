//#line 1 "C:\\SAPDevelop\\DEV\\develop\\sys\\src\\SAPDB\\Interfaces\\SQLDBC\\SQLDBC.h"

#ifndef SQLDBC_H
#define SQLDBC_H

#ifdef  SQLDBC_FEATURE_TRACEOPTION_OFF
#undef  SQLDBC_FEATURE_TRACEOPTION
#else
#define SQLDBC_FEATURE_TRACEOPTION
#endif

#ifdef  SQLDBC_FEATURE_PROFILE_OFF
#undef  SQLDBC_FEATURE_PROFILE
#else
#define SQLDBC_FEATURE_PROFILE
#endif

#ifdef  SQLDBC_FEATURE_DECIMAL_OFF
#undef  SQLDBC_FEATURE_DECIMAL
#else
#ifndef SQLDBC_FEATURE_DECIMAL
#define SQLDBC_FEATURE_DECIMAL
#endif
#endif

#ifdef  SQLDBC_FEATURE_OMS_ASCII_OFF
#undef  SQLDBC_FEATURE_OMS_ASCII
#else
#ifndef SQLDBC_FEATURE_OMS_ASCII
#define SQLDBC_FEATURE_OMS_ASCII
#endif
#endif

#ifdef  SQLDBC_RESULTSET_ROWWISE_OFF
#undef  SQLDBC_FEATURE_RESULTSET_ROWWISE
#else
#define SQLDBC_FEATURE_RESULTSET_ROWWISE
#endif

#ifdef SQLDBC_CONNECTION_FEATURES_OFF
#undef SQLDBC_FEATURE_CONNECTION_FEATURES
#else
#define SQLDBC_FEATURE_CONNECTION_FEATURES
#endif
/*
 * If this macro is set it is possible to retrieve the connection object
 * to a statement.
 */
#ifdef  SQLDBC_FEATURE_STATEMENT_GETCONNECTION_OFF
#undef  SQLDBC_FEATURE_STATEMENT_GETCONNECTION
#else
#define SQLDBC_FEATURE_STATEMENT_GETCONNECTION
#endif

#ifdef  SQLDBC_FEATURE_METADATA_SCHEMATABLE_OFF
#undef  SQLDBC_FEATURE_METADATA_SCHEMATABLE
#else
#define SQLDBC_FEATURE_METADATA_SCHEMATABLE
#endif

#ifdef  SQLDBC_FEATURE_METADATA_COLUMNLABEL_OFF
#undef  SQLDBC_FEATURE_METADATA_COLUMNLABEL
#else
#define SQLDBC_FEATURE_METADATA_COLUMNLABEL
#endif

#ifdef  SQLDBC_FEATURE_LOB_KEEPALIVE_OFF
#undef  SQLDBC_FEATURE_LOB_KEEPALIVE
#else
#define SQLDBC_FEATURE_LOB_KEEPALIVE
#endif

#include "SQLDBC_Types.h" /* no check */

#ifdef IFR_NAMESPACE
namespace IFR_NAMESPACE  {
#endif
class IFR_Environment;
class IFR_Connection;
class IFR_ConnectProperties;
class IFR_ConnectionItem;
class IFR_ErrorHndl;
class IFR_Statement;
class IFR_PreparedStmt;
class IFR_SQLWarning;
class IFR_ResultSet;
class IFR_RowSet;
class IFR_ParameterMetaData;
class IFR_ResultSetMetaData;
class IFR_LOB;
class IFR_Profile;
#ifdef IFR_NAMESPACE
};
using namespace IFR_NAMESPACE;
#endif
class SAPDBMem_IRawAllocator;

/*
   HP compiler complains about constant primitives as return values.
   Removing the 'const' would break binary compatibility, so it's better
   to disable just the warning (using an undocumented #pragma ...).
*/
#ifdef __hpux
#ifdef __ia64
#pragma diag_suppress 2815
#endif
#endif

static const char* const SQLDBCHeaderVersion = "SQLDBC.H  7.7.4    BUILD 008-000-000-000";
/*+
   type maps used by the script genSQLDBC_C.pl for generating a SQLDBC C-wrapper.
   #typemap  SQLDBC_StringEncodingType_Encoding SQLDBC_StringEncoding
   #typemap  ParameterMode SQLDBC_ParameterMetaData::ParameterMode
   #typemap  ParameterNullBehavior SQLDBC_ParameterMetaData::ParameterNullBehavior
   #typemap  ColumnNullBehavior SQLDBC_ResultSetMetaData::ColumnNullBehavior
   #typemap  SQLDBC_Statement_ResultSetType SQLDBC_Statement::ResultSetType
   #typemap  SQLDBC_Statement_ConcurrencyType SQLDBC_Statement::ConcurrencyType
*/

#ifdef SQLDBC_RUNTIME_IN_NAMESPACE
namespace SQLDBC {
    class SQLDBC_IRuntime;
};
#else
class SQLDBC_IRuntime;
#endif

#ifdef WIN32
#ifndef SQLDBC_FOR_KERNEL
#define SQLDBC_DLLEXPORT __declspec(dllexport)
#else
#define SQLDBC_DLLEXPORT
#endif
#else
#define SQLDBC_DLLEXPORT
#endif

namespace SQLDBC {
class SQLDBC_Statement;
class SQLDBC_ConnectProperties;
class SQLDBC_Connection;

class SQLDBC_ErrorHndl
{
public:
  SQLDBC_DLLEXPORT
  SQLDBC_ErrorHndl();

  SQLDBC_DLLEXPORT
  const SQLDBC_Int4 getErrorCode() const;

  SQLDBC_DLLEXPORT
  const char* getSQLState() const;

  SQLDBC_DLLEXPORT
  const char *getErrorText() const;

  SQLDBC_DLLEXPORT
  operator SQLDBC_Bool () const;

private:
  friend class SQLDBC_ConnectionItem;
  friend struct SQLDBC_ConnectionItemStorage;
  friend class SQLDBC_Connection;

  SQLDBC_ErrorHndl(IFR_ErrorHndl* error);

  void setMemoryAllocationFailed();


  IFR_ErrorHndl *m_error;
};

class SQLDBC_SQLWarning
{
public:
  SQLDBC_DLLEXPORT
  SQLDBC_SQLWarning* getNextWarning() const;

  SQLDBC_DLLEXPORT
  SQLDBC_SQLWarningCode getWarningCode() const;

private:
  friend class SQLDBC_ConnectionItem;
  friend struct SQLDBC_ConnectionItemStorage;
  SQLDBC_SQLWarning(IFR_SQLWarning* warning);
  IFR_SQLWarning    *m_warning;
};

struct SQLDBC_ConnectionItemStorage;

class SQLDBC_ConnectionItem
{
public:

    SQLDBC_DLLEXPORT
    SQLDBC_ErrorHndl &error();

    SQLDBC_DLLEXPORT
    void clearError();

    SQLDBC_DLLEXPORT
    SQLDBC_SQLWarning* warning();

    SQLDBC_DLLEXPORT
    void clearWarnings();

private:
    friend class SQLDBC_Environment;
    friend class SQLDBC_Connection;
    friend class SQLDBC_Statement;
    friend class SQLDBC_PreparedStatement;
    friend class SQLDBC_ResultSet;
    friend class SQLDBC_RowSet;
    friend class SQLDBC_UpdatableRowSet;
    friend struct SQLDBC_StatementStorage;
    friend struct SQLDBC_EnvironmentItemStorage;

    SQLDBC_ConnectionItem(IFR_ConnectionItem *item, SQLDBC_Bool isCreatedForSQLDBC_Connection, SQLDBC_Connection* self);

    ~SQLDBC_ConnectionItem();

    SQLDBC_ConnectionItemStorage *m_citem;
};

class SQLDBC_LOB
{
public:
    SQLDBC_DLLEXPORT SQLDBC_Retcode putData(void *paramAddr, SQLDBC_Length  *paramLengthIndicator);

    SQLDBC_DLLEXPORT SQLDBC_Retcode close();


    SQLDBC_DLLEXPORT SQLDBC_Retcode getData(void                 *paramAddr,
                                            SQLDBC_Length        *LengthIndicator,
                                            const SQLDBC_Length   Size,
                                            const SQLDBC_Bool     Terminate=SQLDBC_TRUE);

    /* #methodmap SQLDBC_LOB_getDataByPos getData */
    SQLDBC_DLLEXPORT SQLDBC_Retcode getData(void                 *paramAddr,
                                            SQLDBC_Length        *LengthIndicator,
                                            const SQLDBC_Length   Size,
                                            const SQLDBC_Length   position,
                                            const SQLDBC_Bool     Terminate=SQLDBC_TRUE);

    SQLDBC_DLLEXPORT SQLDBC_Length getLength();

    SQLDBC_DLLEXPORT SQLDBC_Length getPosition();


    SQLDBC_DLLEXPORT SQLDBC_Length getPreferredDataSize();

#ifdef SQLDBC_FEATURE_LOB_KEEPALIVE

    SQLDBC_DLLEXPORT SQLDBC_Retcode setKeepAlive(SQLDBC_Bool keepalive);
#endif
private:
    friend class SQLDBC77_Extension;
    SQLDBC_Retcode _setKeepAlive(SQLDBC_Bool keepalive);
    IFR_LOB                  *m_lobdata;
    IFR_ConnectionItem       *m_citem;
};

class SQLDBC_ParameterMetaData
{
public:

#ifndef __cplusplus
typedef
#endif
  enum ParameterNullBehavior {
    parameterNoNulls = 0,

    parameterNullable = 1,

    parameterNullableUnknown = 2
  }
#ifndef __cplusplus
ParameterNullBehavior
#endif
;

#ifndef __cplusplus
typedef
#endif
  enum ParameterMode {

    parameterModeUnknown = 0,

    parameterModeIn = 1,

    parameterModeInOut = 2,

    parameterModeOut = 4
  }
#ifndef __cplusplus
ParameterMode
#endif
;

  SQLDBC_DLLEXPORT
  SQLDBC_Int2 getParameterCount();

  SQLDBC_DLLEXPORT
  SQLDBC_Retcode getParameterName(SQLDBC_Int2    param,
                                  char *buffer,
                                  const SQLDBC_StringEncoding encoding,
                                  const SQLDBC_Length  bufferSize,
                                  SQLDBC_Length *bufferLength) const;

  SQLDBC_DLLEXPORT
  SQLDBC_SQLType getParameterType(SQLDBC_Int2 param);

  SQLDBC_DLLEXPORT
  ParameterMode getParameterMode(SQLDBC_Int2 param);

  SQLDBC_DLLEXPORT
  SQLDBC_Int4 getParameterLength(SQLDBC_Int2 param);

  SQLDBC_DLLEXPORT
  SQLDBC_Int4 getPrecision(SQLDBC_Int2 param);

  SQLDBC_DLLEXPORT
  SQLDBC_Int4 getScale(SQLDBC_Int2 param);

  SQLDBC_DLLEXPORT
  SQLDBC_Int4 getPhysicalLength(SQLDBC_Int2 param);

  SQLDBC_DLLEXPORT
  ParameterNullBehavior isNullable(SQLDBC_Int2 param);

private:
  friend class SQLDBC_PreparedStatement;
  friend struct SQLDBC_PreparedStatementStorage;
  SQLDBC_ParameterMetaData(IFR_ParameterMetaData *metadata);
  IFR_ParameterMetaData *m_metadata;
};

class SQLDBC_ResultSetMetaData
{
public:
#ifndef __cplusplus
typedef
#endif
  enum ColumnNullBehavior {
    columnNoNulls = 0,

    columnNullable = 1,

    columnNullableUnknown = 2
  }
#ifndef __cplusplus
ColumnNullBehavior
#endif
;
  SQLDBC_DLLEXPORT
  SQLDBC_Int2 getColumnCount();


#ifdef SQLDBC_FEATURE_METADATA_SCHEMATABLE

  SQLDBC_DLLEXPORT
  SQLDBC_Retcode getSchemaName(SQLDBC_Int2 column,
                               char *buffer,
                               const SQLDBC_StringEncoding encoding,
                               const SQLDBC_Length  bufferSize,
                               SQLDBC_Length *bufferLength) const;


  SQLDBC_DLLEXPORT
  SQLDBC_Retcode getTableName(SQLDBC_Int2 column,
                              char *buffer,
                              const SQLDBC_StringEncoding encoding,
                              const SQLDBC_Length  bufferSize,
                              SQLDBC_Length *bufferLength) const;


#endif

  SQLDBC_DLLEXPORT
  SQLDBC_Retcode getColumnName(SQLDBC_Int2 column,
                                 char *buffer,
                                 const SQLDBC_StringEncoding encoding,
                                 const SQLDBC_Length  bufferSize,
                                 SQLDBC_Length *bufferLength) const;

#ifdef SQLDBC_FEATURE_METADATA_COLUMNLABEL

  SQLDBC_DLLEXPORT
  SQLDBC_Retcode getColumnLabel(SQLDBC_Int2 column,
                                 char *buffer,
                                 const SQLDBC_StringEncoding encoding,
                                 const SQLDBC_Length  bufferSize,
                                 SQLDBC_Length *bufferLength) const;

#endif

  SQLDBC_DLLEXPORT
  SQLDBC_SQLType getColumnType(SQLDBC_Int2 column);

  SQLDBC_DLLEXPORT
  SQLDBC_Int4 getColumnLength(SQLDBC_Int2 column);

  SQLDBC_DLLEXPORT
  SQLDBC_Int4 getPrecision(SQLDBC_Int2 column);

  SQLDBC_DLLEXPORT
  SQLDBC_Int4 getScale(SQLDBC_Int2 column);

  SQLDBC_DLLEXPORT
  SQLDBC_Int4 getPhysicalLength(SQLDBC_Int2 column);

  SQLDBC_DLLEXPORT
  ColumnNullBehavior isNullable(SQLDBC_Int2 column);

  SQLDBC_DLLEXPORT
  SQLDBC_Bool isWritable(SQLDBC_Int2 column);

private:
  friend class SQLDBC_PreparedStatement;
  friend class SQLDBC_ResultSet;
  friend struct SQLDBC_ResultSetStorage;
  friend struct SQLDBC_PreparedStatementStorage;
  friend class SQLDBC77_Extension;
  SQLDBC_Retcode _getColumnLabel(SQLDBC_Int2 column,
                                 char *buffer,
                                 const SQLDBC_StringEncoding encoding,
                                 const SQLDBC_Length  bufferSize,
                                 SQLDBC_Length *bufferLength) const;
  SQLDBC_Retcode _getTableName(SQLDBC_Int2 column,
                              char *buffer,
                              const SQLDBC_StringEncoding encoding,
                              const SQLDBC_Length  bufferSize,
                              SQLDBC_Length *bufferLength) const;
  SQLDBC_Retcode _getSchemaName(SQLDBC_Int2 column,
                               char *buffer,
                               const SQLDBC_StringEncoding encoding,
                               const SQLDBC_Length  bufferSize,
                               SQLDBC_Length *bufferLength) const;
  SQLDBC_ResultSetMetaData(IFR_ResultSetMetaData *metadata);
  IFR_ResultSetMetaData *m_metadata;
};

class SQLDBC_RowSet
  : public SQLDBC_ConnectionItem
{
public:

  SQLDBC_DLLEXPORT
  SQLDBC_Retcode setPos(SQLDBC_UInt4 pos);

  SQLDBC_DLLEXPORT
  SQLDBC_Retcode fetch();

  SQLDBC_DLLEXPORT
  const SQLDBC_Int4 getRowsAffected() const;

  SQLDBC_DLLEXPORT
  const SQLDBC_Int4 *getRowStatus() const;

  SQLDBC_DLLEXPORT
  SQLDBC_Retcode getObject(const SQLDBC_Int4     Index,
                           const SQLDBC_HostType Type,
                           void                 *paramAddr,
                           SQLDBC_Length        *LengthIndicator,
                           const SQLDBC_Length   Size,
                           const SQLDBC_Bool     Terminate=SQLDBC_TRUE);

  /* #methodmap SQLDBC_RowSet_getObjectByPos getObject */
  SQLDBC_DLLEXPORT
  SQLDBC_Retcode getObject(const SQLDBC_Int4     Index,
                           const SQLDBC_HostType Type,
                           void                 *paramAddr,
                           SQLDBC_Length        *LengthIndicator,
                           const SQLDBC_Length   Size,
                           SQLDBC_Length         StartPos,
                           const SQLDBC_Bool     Terminate=SQLDBC_TRUE);

protected:
  friend class SQLDBC_ResultSet;
  friend struct SQLDBC_ResultSetStorage;
  SQLDBC_RowSet(IFR_ResultSet *resultset);
};

class SQLDBC_UpdatableRowSet
  : public SQLDBC_RowSet
{
public:

  SQLDBC_DLLEXPORT
  SQLDBC_Retcode insertAllRows();

  SQLDBC_DLLEXPORT
  SQLDBC_Retcode insertOneRow();

  SQLDBC_DLLEXPORT
  SQLDBC_Retcode insertRow(SQLDBC_UInt4 position);


  SQLDBC_DLLEXPORT
  SQLDBC_Retcode updateRow(SQLDBC_UInt4 position);
  SQLDBC_DLLEXPORT
  SQLDBC_Retcode updateRow(int position);

  SQLDBC_DLLEXPORT
  SQLDBC_Retcode deleteRow(SQLDBC_UInt4 position);
  SQLDBC_DLLEXPORT
  SQLDBC_Retcode deleteRow(int position);

  SQLDBC_DLLEXPORT
  SQLDBC_Retcode nextParameter(SQLDBC_Int2& paramIndex, void*& paramAddr);

   SQLDBC_DLLEXPORT
   SQLDBC_Retcode nextParameterByIndex(SQLDBC_Int2& paramIndex, void*& paramAddr);


  SQLDBC_DLLEXPORT
  SQLDBC_Retcode putData(void *paramAddr, SQLDBC_Length  *paramLengthIndicator);


private:
  friend struct SQLDBC_ResultSetStorage;
  SQLDBC_UpdatableRowSet(IFR_ResultSet *resultset);
};

struct SQLDBC_ResultSetStorage;

class SQLDBC_ResultSet
  : public SQLDBC_ConnectionItem
{
public:

  SQLDBC_DLLEXPORT
  SQLDBC_ResultSetMetaData* getResultSetMetaData();

  SQLDBC_DLLEXPORT
  const SQLDBC_Int4 getResultCount () const;

  SQLDBC_DLLEXPORT
  SQLDBC_Retcode bindColumn (const SQLDBC_UInt4    Index,
                             const SQLDBC_HostType Type,
                             void                 *paramAddr,
                             SQLDBC_Length        *LengthIndicator,
                             const SQLDBC_Length   Size,
                             const SQLDBC_Bool     Terminate=SQLDBC_TRUE);

  /* #methodmap SQLDBC_ResultSet_bindColumnByPos bindColumn */
  SQLDBC_DLLEXPORT
  SQLDBC_Retcode bindColumn (const SQLDBC_UInt4    Index,
                             const SQLDBC_HostType Type,
                             void                 *paramAddr,
                             SQLDBC_Length        *LengthIndicator,
                             SQLDBC_Length        *PositionIndicator,
                             const SQLDBC_Length   Size,
                             const SQLDBC_Bool     Terminate=SQLDBC_TRUE);


  SQLDBC_DLLEXPORT
  SQLDBC_Retcode setBindingType(SQLDBC_size_t size);

  SQLDBC_DLLEXPORT
  SQLDBC_Retcode clearColumns();

   SQLDBC_DLLEXPORT
   void setFetchSize(SQLDBC_Int2 fetchsize);

  SQLDBC_DLLEXPORT
  void setRowSetSize (SQLDBC_UInt4 rowsetsize);

  SQLDBC_DLLEXPORT
  const SQLDBC_UInt4 getRowSetSize () const;

  SQLDBC_DLLEXPORT
  SQLDBC_RowSet *getRowSet ();

  SQLDBC_DLLEXPORT
  SQLDBC_UpdatableRowSet *getUpdatableRowSet ();

  SQLDBC_DLLEXPORT
  SQLDBC_Statement *getStatement();

  SQLDBC_DLLEXPORT
  SQLDBC_Bool isUpdatable();

  SQLDBC_DLLEXPORT
  SQLDBC_Retcode first();

  SQLDBC_DLLEXPORT
  SQLDBC_Retcode next();

  SQLDBC_DLLEXPORT
  SQLDBC_Retcode previous();

  SQLDBC_DLLEXPORT
  SQLDBC_Retcode last();

  SQLDBC_DLLEXPORT
  SQLDBC_Retcode absolute (int row);

  SQLDBC_DLLEXPORT
  SQLDBC_Retcode relative(int relativePos);

  SQLDBC_DLLEXPORT
  void close();

  SQLDBC_DLLEXPORT
  const SQLDBC_UInt4 getRowNumber() const;

  SQLDBC_DLLEXPORT
  SQLDBC_Retcode getObject(const SQLDBC_Int4     Index,
                           const SQLDBC_HostType Type,
                           void                 *paramAddr,
                           SQLDBC_Length        *LengthIndicator,
                           const SQLDBC_Length   Size,
                           const SQLDBC_Bool     Terminate=SQLDBC_TRUE);


  /* #methodmap SQLDBC_ResultSet_getObjectByPos getObject */
  SQLDBC_DLLEXPORT
  SQLDBC_Retcode getObject(const SQLDBC_Int4     Index,
                           const SQLDBC_HostType Type,
                           void                 *paramAddr,
                           SQLDBC_Length        *LengthIndicator,
                           const SQLDBC_Length   Size,
                           SQLDBC_Length         StartPos,
                           const SQLDBC_Bool     Terminate=SQLDBC_TRUE);

private:
  friend class SQLDBC_Statement;
  friend struct SQLDBC_StatementStorage;

  SQLDBC_ResultSet(SQLDBC_Statement* statement,
                   IFR_ResultSet *resultset);
  ~SQLDBC_ResultSet();

  SQLDBC_ResultSetStorage *m_cresult;
};

struct SQLDBC_StatementStorage;

class SQLDBC_Statement
  : public SQLDBC_ConnectionItem
{
public:
  SQLDBC_DLLEXPORT
  SQLDBC_Retcode execute(const char *sql,
                         const SQLDBC_Length sqlLength,
                         const SQLDBC_StringEncoding encoding);

  /* #methodmap SQLDBC_Statement_executeNTS execute */
  SQLDBC_DLLEXPORT
  SQLDBC_Retcode execute(const char *sql,
                         const SQLDBC_StringEncoding encoding);

  /* #methodmap SQLDBC_Statement_executeASCII execute */
  SQLDBC_DLLEXPORT
  SQLDBC_Retcode execute(const char *sql);

  SQLDBC_DLLEXPORT
  void setResultSetFetchSize(SQLDBC_Int2 rows);

  SQLDBC_DLLEXPORT
  void setMaxRows(SQLDBC_UInt4 rows);

  SQLDBC_DLLEXPORT
  const SQLDBC_UInt4 getMaxRows() const;

  SQLDBC_DLLEXPORT
  SQLDBC_ResultSet *getResultSet();

  SQLDBC_DLLEXPORT
  void setCursorName(const char    *buffer,
                     SQLDBC_Length  bufferLength,
                     const SQLDBC_StringEncoding encoding);

  SQLDBC_DLLEXPORT
    SQLDBC_Retcode setCommandInfo(const char *buffer,
                               SQLDBC_Length bufferLength,
                               SQLDBC_Int4 lineNumber);
  SQLDBC_DLLEXPORT
  SQLDBC_Retcode getCursorName(char         *buffer,
                               const SQLDBC_StringEncoding encoding,
                               const SQLDBC_Length bufferSize,
                               SQLDBC_Length *bufferLength) const;

  SQLDBC_DLLEXPORT
  SQLDBC_Retcode getTableName(char          *buffer,
                              const SQLDBC_StringEncoding encoding,
                              const SQLDBC_Length  bufferSize,
                              SQLDBC_Length *bufferLength) const;

  SQLDBC_DLLEXPORT
  SQLDBC_Bool isQuery() const;

  SQLDBC_DLLEXPORT
  SQLDBC_Int4 getKernelVersion() const;


  SQLDBC_DLLEXPORT
  const SQLDBC_Int4 getRowsAffected() const;

  enum ResultSetType {
    FORWARD_ONLY = 1, 
    SCROLL_SENSITIVE = 2, 
    SCROLL_INSENSITIVE = 3 
  };

  SQLDBC_DLLEXPORT
  void setResultSetType (ResultSetType type );

  SQLDBC_DLLEXPORT
  const ResultSetType getResultSetType() const;

  enum ConcurrencyType {
    CONCUR_UPDATABLE = 10, 
    CONCUR_READ_ONLY = 11  
  };

  SQLDBC_DLLEXPORT
  void setResultSetConcurrencyType (ConcurrencyType type);

  enum HoldabilityType {
    CURSOR_HOLD_OVER_COMMIT = 20, 
    CURSOR_CLOSE_ON_COMMIT = 21   
  };

  SQLDBC_DLLEXPORT
  const ConcurrencyType getResultSetConcurrencyType() const;

    SQLDBC_DLLEXPORT
    const SQLDBC_Int4 *getRowStatus() const;

    SQLDBC_DLLEXPORT
    const SQLDBC_UInt4 getBatchSize() const;

    SQLDBC_DLLEXPORT
    const SQLDBC_Retcode addBatch(const char *sql,
                                  SQLDBC_Length sqlLength,
                                  SQLDBC_StringEncoding encoding);

    /* #methodmap SQLDBC_Statement_addBatchNTS addBatch */
    SQLDBC_DLLEXPORT
    const SQLDBC_Retcode addBatch(const char *sql,
                                  SQLDBC_StringEncoding encoding);

    /* #methodmap SQLDBC_Statement_addBatchASCII addBatch */
    SQLDBC_DLLEXPORT
    const SQLDBC_Retcode addBatch(const char *sql);

    SQLDBC_DLLEXPORT
    const SQLDBC_Retcode executeBatch();

    SQLDBC_DLLEXPORT
    void clearBatch();

#ifdef SQLDBC_FEATURE_STATEMENT_GETCONNECTION

    SQLDBC_DLLEXPORT
    SQLDBC_Connection *getConnection();

#endif

    SQLDBC_DLLEXPORT
   SQLDBC_Retcode getLastInsertedKey(SQLDBC_Int4      tag,
                                     SQLDBC_HostType type,
                                     void            *paramAddr,
                                     SQLDBC_Length   *lengthIndicator,
                                     SQLDBC_Length    size,
                                     SQLDBC_Bool      terminate=SQLDBC_TRUE);


protected:
    void clearResultSet();


private:
  friend struct SQLDBC_ConnectionItemStorageForConnection;
  friend class SQLDBC_Connection;
  friend class SQLDBC_PreparedStatement;
  friend class SQLDBC77_Extension;
  SQLDBC_Connection *_getConnection();
    SQLDBC_Statement(IFR_Statement *stmt);
    SQLDBC_Statement(IFR_PreparedStmt *stmt);
  ~SQLDBC_Statement();
  SQLDBC_StatementStorage *m_cstmt;
};

struct SQLDBC_PreparedStatementStorage;

class SQLDBC_PreparedStatement
    : public SQLDBC_Statement
{
public:
  SQLDBC_DLLEXPORT
  SQLDBC_Retcode prepare(const char *sql,
                         const SQLDBC_Length sqlLength,
                         const SQLDBC_StringEncoding encoding);

  SQLDBC_DLLEXPORT
  SQLDBC_Retcode executeItab(void*, bool);

  SQLDBC_DLLEXPORT
  void *getItabReader();

    /* #methodmap SQLDBC_PreparedStatement_prepareNTS prepare */
  SQLDBC_DLLEXPORT
  SQLDBC_Retcode prepare(const char *sql,
                         const SQLDBC_StringEncoding encoding);

  /* #methodmap SQLDBC_PreparedStatement_prepareASCII prepare */
  SQLDBC_DLLEXPORT
  SQLDBC_Retcode prepare(const char *sql);

  /* #methodmap SQLDBC_PreparedStatement_executeASCII execute */
SQLDBC_DLLEXPORT
  SQLDBC_Retcode execute();

  SQLDBC_DLLEXPORT
  SQLDBC_ParameterMetaData* getParameterMetaData();

  SQLDBC_DLLEXPORT
  SQLDBC_ResultSetMetaData* getResultSetMetaData();

  SQLDBC_DLLEXPORT
  SQLDBC_Retcode setBatchSize(SQLDBC_UInt4 rowarraysize);

  SQLDBC_DLLEXPORT
  SQLDBC_UInt4 getPreferredBatchSize();


  SQLDBC_DLLEXPORT
  SQLDBC_Retcode clearParameters();


  SQLDBC_DLLEXPORT
  SQLDBC_Retcode bindParameter (const SQLDBC_UInt4     Index,
                                const SQLDBC_HostType  Type,
                                void                  *paramAddr,
                                SQLDBC_Length         *LengthIndicator,
                                const SQLDBC_Length    Size,
                                const SQLDBC_Bool      Terminate=SQLDBC_TRUE);

  SQLDBC_DLLEXPORT
  SQLDBC_Retcode bindParameterAddr(const SQLDBC_UInt2    Index,
                                   const SQLDBC_HostType Type,
                                   void                 *paramAddr,
                                   SQLDBC_Length        *LengthIndicator,
                                   const SQLDBC_Length   Size,
                                   const SQLDBC_Bool     Terminate=SQLDBC_TRUE);

  SQLDBC_DLLEXPORT
  SQLDBC_Retcode setBindingType(SQLDBC_size_t size);

  SQLDBC_DLLEXPORT
  SQLDBC_Retcode nextParameter(SQLDBC_Int2& paramIndex, void*& paramAddr);

   SQLDBC_DLLEXPORT
   SQLDBC_Retcode nextParameterByIndex(SQLDBC_Int2& paramIndex, void*& paramAddr);


  SQLDBC_DLLEXPORT
  SQLDBC_Retcode putData(void *paramAddr, SQLDBC_Length  *paramLengthIndicator);

  SQLDBC_DLLEXPORT
  SQLDBC_Retcode getObject(const SQLDBC_Int4     Index,
                           const SQLDBC_HostType Type,
                           void                 *paramAddr,
                           SQLDBC_Length        *LengthIndicator,
                           const SQLDBC_Length   Size,
                           const SQLDBC_Bool     Terminate=SQLDBC_TRUE);


   /* #methodmap SQLDBC_PreparedStatement_getObjectByPos getObject */
 SQLDBC_DLLEXPORT
  SQLDBC_Retcode getObject(const SQLDBC_Int4     Index,
                           const SQLDBC_HostType Type,
                           void                 *paramAddr,
                           SQLDBC_Length        *LengthIndicator,
                           const SQLDBC_Length   Size,
                           SQLDBC_Length         StartPos,
                           const SQLDBC_Bool     Terminate=SQLDBC_TRUE);


private:
  friend class SQLDBC_Connection;
  friend struct SQLDBC_ConnectionItemStorageForConnection;
  SQLDBC_PreparedStatement(IFR_PreparedStmt *stmt);
  ~SQLDBC_PreparedStatement();
  SQLDBC_PreparedStatementStorage *m_pcstmt;
};

class SQLDBC_ConnectProperties
{
public:
  SQLDBC_DLLEXPORT
  SQLDBC_ConnectProperties();

  SQLDBC_DLLEXPORT
  SQLDBC_ConnectProperties(const SQLDBC_ConnectProperties& copy);

  SQLDBC_ConnectProperties(SAPDBMem_IRawAllocator& allocator);

  SQLDBC_DLLEXPORT
  ~SQLDBC_ConnectProperties();


  SQLDBC_DLLEXPORT
  void setProperty(const char *key, const char *value);

  SQLDBC_DLLEXPORT
  const char *getProperty(const char *key, const char *defaultvalue=0) const;

private:
  friend class SQLDBC_Connection;
  friend class SQLDBC_Environment;
  IFR_ConnectProperties *m_prop;
};


class SQLDBC_Connection
  : public SQLDBC_ConnectionItem
{
public:
  SQLDBC_DLLEXPORT
  SQLDBC_Retcode connect(const char *connectURL,
                         SQLDBC_Length connectURLLength,
                         const char *connectCommand,
                         SQLDBC_Length connectCommandLength,
                         SQLDBC_StringEncoding connectCommandEncoding,
                         SQLDBC_ConnectProperties& connectProperties);

  SQLDBC_DLLEXPORT
  SQLDBC_Retcode connect(const char *servernode,
                         SQLDBC_Length servernodeLength,
                         const char *serverdb,
                         SQLDBC_Length serverdbLength,
                         const char *username,
                         SQLDBC_Length usernameLength,
                         const char *password,
                         SQLDBC_Length passwordLength,
                         const SQLDBC_StringEncoding userpwdEncoding);

  SQLDBC_DLLEXPORT
  SQLDBC_Retcode connect(const char *servernode,
                         const char *serverdb,
                         const char *username,
                         const char *password,
                         const SQLDBC_StringEncoding userpwdEncoding);

  SQLDBC_DLLEXPORT
  SQLDBC_Retcode connect(const char *servernode,
                         const char *serverdb,
                         const char *username,
                         const char *password);

  SQLDBC_DLLEXPORT
  SQLDBC_Retcode connect(const char* servernode,
                         SQLDBC_Length servernodeLength,
                         const char* serverdb,
                         SQLDBC_Length serverdbLength,
                         const char* username,
                         SQLDBC_Length usernameLength,
                         const char* password,
                         SQLDBC_Length passwordLength,
                         const SQLDBC_StringEncoding userpwdEncoding,
                         const SQLDBC_ConnectProperties& properties);

   /* #methodmap SQLDBC_Connection_connectNTS connect */
  SQLDBC_DLLEXPORT
  SQLDBC_Retcode connect(const char* servernode,
                         const char* serverdb,
                         const char* username,
                         const char* password,
                         const SQLDBC_StringEncoding userpwdEncoding,
                         const SQLDBC_ConnectProperties& properties);

  /* #methodmap SQLDBC_Connection_connectASCII connect */
  SQLDBC_DLLEXPORT
  SQLDBC_Retcode connect(const char* servernode,
                         const char* serverdb,
                         const char* username,
                         const char* password,
                         const SQLDBC_ConnectProperties& properties);


  SQLDBC_DLLEXPORT
  SQLDBC_Retcode connect();

  /* #methodmap SQLDBC_Connection_connectPROP connect */
  SQLDBC_DLLEXPORT
  SQLDBC_Retcode connect(const SQLDBC_ConnectProperties& properties);


#ifdef SQLDBC_FEATURE_CONNECTION_FEATURES

  /* #methodmap SQLDBC_Connection_connectPROP connect   *
   */
  SQLDBC_DLLEXPORT
  SQLDBC_Retcode getConnectionFeatures(SQLDBC_ConnectProperties& properties);
#endif

  SQLDBC_DLLEXPORT
  SQLDBC_Statement *createStatement();

  SQLDBC_DLLEXPORT
  SQLDBC_PreparedStatement *createPreparedStatement();

  SQLDBC_DLLEXPORT
  SQLDBC_Retcode commit();

  SQLDBC_DLLEXPORT
  SQLDBC_Retcode rollback();

  SQLDBC_DLLEXPORT
  void releaseStatement(SQLDBC_Statement *stmt);

  /* #methodmap SQLDBC_Connection_releasePreparedStatement releaseStatement */
  SQLDBC_DLLEXPORT
  void releaseStatement(SQLDBC_PreparedStatement *stmt);

  SQLDBC_DLLEXPORT
  SQLDBC_Retcode cancel();

  SQLDBC_DLLEXPORT
  SQLDBC_Retcode close();


  SQLDBC_DLLEXPORT
  SQLDBC_Retcode commitWorkRelease();


  SQLDBC_DLLEXPORT
  SQLDBC_Retcode disconnect();


  SQLDBC_DLLEXPORT
  void setAutoCommit(SQLDBC_Bool autocommit);

  SQLDBC_DLLEXPORT
  SQLDBC_Bool getAutoCommit() const;

  SQLDBC_DLLEXPORT
  void setSQLMode(SQLDBC_SQLMode sqlmode);

  SQLDBC_DLLEXPORT
  SQLDBC_Retcode setTransactionIsolation(SQLDBC_Int4 isolationlevel);

  SQLDBC_DLLEXPORT
  SQLDBC_Int4 getTransactionIsolation() const;

  SQLDBC_DLLEXPORT
  SQLDBC_Bool isConnected() const;

  SQLDBC_DLLEXPORT
  SQLDBC_Int4 getKernelVersion() const;

  SQLDBC_DLLEXPORT
  SQLDBC_Bool isUnicodeDatabase() const;

  SQLDBC_DLLEXPORT
  SQLDBC_DateTimeFormat::Format getDateTimeFormat() const;

private:
  friend class SQLDBC_Environment;
  friend class SQLDBC77_Extension;
  SQLDBC_Retcode _getConnectionFeatures(SQLDBC_ConnectProperties& properties);
  SQLDBC_Connection(IFR_Connection *conn);
};


#ifdef SQLDBC_FEATURE_PROFILE

class SQLDBC_Profile
{
public:

    SQLDBC_DLLEXPORT
  void resetCounters();

    SQLDBC_DLLEXPORT
    void collectCounters();

    SQLDBC_DLLEXPORT
    SQLDBC_UInt8 getCounter(SQLDBC_ProfileCountValues counter);

private:
  friend class SQLDBC_Environment;
  friend struct SQLDBC_EnvironmentItemStorage;
  SQLDBC_Profile(IFR_Profile *profile);
  IFR_Profile *m_profile;
};
#endif

struct SQLDBC_EnvironmentItemStorage;

class Runtime;

class SQLDBC_Environment
{
public:
  SQLDBC_DLLEXPORT
  SQLDBC_Environment(/*SQLDBC_IRuntime**/Runtime* runtime);

  SQLDBC_DLLEXPORT
  ~SQLDBC_Environment();

  SQLDBC_DLLEXPORT
  SQLDBC_Connection *createConnection() const;

  SQLDBC_DLLEXPORT
  SQLDBC_Connection *createConnection(SAPDBMem_IRawAllocator& allocator) const;

  SQLDBC_DLLEXPORT
  void releaseConnection(SQLDBC_Connection *connection) const;

  SQLDBC_DLLEXPORT
  const char *getLibraryVersion();

  SQLDBC_DLLEXPORT
  inline const char *getSDKVersion()
  const
  {
    return SQLDBCHeaderVersion;
  };

    SQLDBC_DLLEXPORT SAPDBMem_IRawAllocator&  getAllocator();

#ifdef SQLDBC_FEATURE_TRACEOPTION

    SQLDBC_DLLEXPORT
    void setTraceOptions(const SQLDBC_ConnectProperties& traceoptions);

    SQLDBC_DLLEXPORT
    void getTraceOptions(SQLDBC_ConnectProperties& traceoptions);
#endif

#ifdef SQLDBC_FEATURE_PROFILE

    SQLDBC_DLLEXPORT
  SQLDBC_Profile* getProfile();

#endif
private:
    SQLDBC_EnvironmentItemStorage *m_eitem;
};

SQLDBC_DLLEXPORT SQLDBC_IRuntime* GetClientRuntime(char *errorText, const SQLDBC_Int4 errorTextSize);

SQLDBC_DLLEXPORT SQLDBC_IRuntime* GetSingleThreadClientRuntime(char *errorText, const SQLDBC_Int4 errorTextSize);
} /* namespace SQLDBC */


#endif