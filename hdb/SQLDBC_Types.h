//#line 1 "C:\\SAPDevelop\\DEV\\develop\\sys\\src\\SAPDB\\Interfaces\\SQLDBC\\SQLDBC_Types.h"
 
 #ifndef SQLDBC_TYPES_H
 #define SQLDBC_TYPES_H
 
 /*
  * Define the platform setup.
  *
  * WIN32           - Microsoft Windows
  * LINUX           - Linux
  * SUN             - Sun Solaris (Sparc)
  * SOLARIS         - Sun Solaris
  * HPUX            - HP Unix (HPPA)
  * AIX             - AIX (RS6000)
  * NMP             - Reliant.
  *
  * IA64            - IA 64 architecture.
  * BIT64           - 64 bit platform.
  */
 
 /* Do not set anything if BIT64 is set from MaxDB Development Tools */
 #if !defined(BIT64)
 
 #if defined(__osf__) 
 #    define BIT64 1
 #endif
 
 #if (defined(__linux__) && defined(__ia64__))
 #    define BIT64 1
 #endif
 
 #if (defined(__linux__) && defined(__s390x__)) 
 /* __s390x__ is 64-bit, __s390__ would be 32 bit. */
 #    define BIT64 1
 #endif
 
 #if (defined(__linux__) && defined(__x86_64__) )
 #    define BIT64 1
 #endif
 
 #if (defined(__linux__) && defined(__powerpc64__))
 /* __powerpc__ would be 32 bit. */
 #    define BIT64 1
 #endif
 
 #if defined(_WIN64)
 #    define BIT64 1
 #endif
 
 #if defined(_AIX) && defined(__64BIT__) 
 #  define BIT64 1
 #endif
 
 #if (defined(sun) || defined(__sun)) 
 #  include <sys/isa_defs.h>
 #  if defined(_LP64) 
 #    define BIT64 1
 #  endif
 #endif
 
 #if (defined(__hpux)||defined(sinix)) && defined(__LP64__)
 #  define BIT64 1
 #endif
 
 /* #if !defined(BIT64) */
 #endif
 
 
 #if !(defined(__linux__) && defined(__i386__)) && \
     !(defined(__linux__) && defined(__powerpc64__)) && \
     !(defined(__linux__) && defined(__s390x__)) && \
     !(defined(__linux__) && defined(__x86_64__)) && \
     !(defined(__linux__) && defined(__ia64__)) && \
     !defined(_WIN32) && \
     !((defined(sun) || defined(__sun)) && (defined(__sparc) || defined(__i386) || defined(__amd64))) &&\
     !defined(_AIX) && \
     !defined(__osf__) && \
     !defined(sinix) && \
     !defined(__hpux) && \
     !defined(__APPLE__)
 #error Unknown platform.
 #endif
 
 #if (defined (_AIX) && __IBMCPP__ < 500) || ( (defined(sun) || defined(__sun)) && !defined(_BOOL) ) \
   || !defined (__cplusplus)
 
 # define CPP_BOOL_NOT_IMPLEMENTED
 #endif
 
 #if defined(_WIN32) \
     || (defined(__linux__) && !defined(__powerpc__) && !defined(__s390x__)) \
     ||  defined(__osf__) \
     || (defined(__sun) && (defined(__i386) || defined(__amd64)))
 #   define SQLDBC_SWAPPED_UCS2
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
 
 #ifdef  SQLDBC_FEATURE_DECFLOAT_OFF
 #undef  SQLDBC_FEATURE_DECFLOAT
 #else
 #ifndef SQLDBC_FEATURE_DECFLOAT
 
 #define SQLDBC_FEATURE_DECFLOAT
 #endif
 #endif
 
 #ifdef  SQLDBC_FEATURE_LOB_KEEPALIVE_INDICATOR_OFF
 #undef  SQLDBC_FEATURE_LOB_KEEPALIVE_INDICATOR
 #else
 #ifndef SQLDBC_FEATURE_LOB_KEEPALIVE_INDICATOR
 
 #define SQLDBC_FEATURE_LOB_KEEPALIVE_INDICATOR
 #endif
 #endif
 
 #ifdef CPP_BOOL_NOT_IMPLEMENTED
 #if defined(_WIN32) || defined(_AIX) || defined(__linux__) || defined(__osf__) \
  || defined(__hpux) || defined(sinix) || (defined(sun) || defined(__sun))
 
 typedef unsigned char SQLDBC_Bool;
 typedef SQLDBC_Bool  bool;
 #endif
 #else
 typedef bool SQLDBC_Bool;
 #endif 
 
 #define SQLDBC_TRUE          ((SQLDBC_Bool)1)
 
 #define SQLDBC_FALSE         ((SQLDBC_Bool)0)
 
 #if defined(_WIN64)
 typedef unsigned __int64    SQLDBC_ULong;      /* long (fast, unsigned) */
 typedef signed __int64      SQLDBC_Long;       /* long (fast, signed)   */
 #else
 
 typedef unsigned long       SQLDBC_ULong;      /* long (fast, unsigned) */
 typedef signed long         SQLDBC_Long;       /* long (fast, signed)   */
 #endif
 
 typedef unsigned char       SQLDBC_Byte;
 
 typedef SQLDBC_ULong        SQLDBC_size_t;
 
 typedef SQLDBC_Byte         SQLDBC_UTF8;
 
 typedef unsigned char       SQLDBC_UInt1;
 typedef signed char         SQLDBC_Int1;
 typedef unsigned short      SQLDBC_UInt2;
 typedef signed short        SQLDBC_Int2;
 typedef unsigned int        SQLDBC_UInt4;
 typedef signed int          SQLDBC_Int4;
 
 #if defined(_WIN32) || defined(_WIN64)
 
 typedef unsigned __int64    SQLDBC_UInt8;
 typedef signed __int64      SQLDBC_Int8;
 #else
 
 typedef unsigned long long SQLDBC_UInt8;
 typedef long long          SQLDBC_Int8;
 #endif
 
 #if defined(BIT64)
 typedef SQLDBC_Int8        SQLDBC_Length;
 #else
 
 typedef SQLDBC_Int4        SQLDBC_Length;
 #endif
 
 typedef unsigned int       SQLDBC_UInt;
 
 typedef signed int         SQLDBC_Int;
 
 #ifdef SQLDBC_FEATURE_DECFLOAT
 
 typedef unsigned char       SQLDBC_DecFloat;
 #endif
 
 #define SQLDBC_NULL_DATA     (-1)
 
 #define SQLDBC_DATA_AT_EXEC  (-2)
 
 #define SQLDBC_NTS           (-3)
 
 #define SQLDBC_NO_TOTAL      (-4)
 
 #define SQLDBC_DEFAULT_PARAM (-5)
 
 #define SQLDBC_IGNORE        (-6)
 
 #ifdef SQLDBC_FEATURE_LOB_KEEPALIVE_INDICATOR
 
 #define SQLDBC_LOB_KEEPALIVE (-7)
 #endif
 
 #define SQLDBC_LEN_DATA_AT_EXEC_OFFSET (-100)
 
 #define SQLDBC_LEN_DATA_AT_EXEC(length) (-(length)+SQLDBC_LEN_DATA_AT_EXEC_OFFSET)
 
 #ifdef SQLDBC_FEATURE_DECIMAL
 
 #define SQLDBC_LEN_DECIMAL(digits, fraction)  (0x40000000 | ((digits & 0xFF) << 8) | (fraction & 0xFF))
 
 #define SQLDBC_DECIMAL_LENGTH(digits, fraction) ((digits + 2) / 2)
 
 
 #define SQLDBC_DECIMAL_DIGITS(indicator)  (     \
     ((indicator) & 0xFFFF0000) == 0x40000000    \
     ? ((indicator) & 0x0000FF00) >> 8           \
     : (-1)                                      \
 )
 
 #define SQLDBC_DECIMAL_FRACTION(indicator)  (   \
     ((indicator) & 0xFFFF0000) == 0x40000000    \
     ? ((indicator) & 0x000000FF)                \
     : (-1)                                      \
 )
 
 #endif
 
 #ifndef __cplusplus
 typedef
 #endif
 enum SQLDBC_Retcode {
   SQLDBC_INVALID_OBJECT           =-10909,    
      SQLDBC_OK                       = 0,     
      SQLDBC_NOT_OK                   = 1,     
      SQLDBC_DATA_TRUNC               =2,      
      SQLDBC_OVERFLOW                 =3,      
      SQLDBC_SUCCESS_WITH_INFO        =4,      
      SQLDBC_NO_DATA_FOUND            =100,    
      SQLDBC_NEED_DATA                =99      
 }
 #ifndef __cplusplus
 SQLDBC_Retcode
 #endif
 ;
 
 #ifndef __cplusplus
 typedef
 #endif
 enum SQLDBC_Rowstatus
 {
     SQLDBC_EXECUTE_FAILED            = -2,      
     SQLDBC_SUCCESS_NO_INFO           = -3,      
     SQLDBC_DELETED                   = -4,      
     SQLDBC_UPDATED                   = -5,      
     SQLDBC_NOROW                     = -6,      
     SQLDBC_ADDED                     = -7,      
     SQLDBC_ROW_IGNORE                = -8       
 }
 #ifndef __cplusplus
 SQLDBC_Rowstatus
 #endif
 ;
 
 
 #ifndef __cplusplus
 typedef
 #endif
 enum SQLDBC_SQLMode {
      SQLDBC_INTERNAL                 =2,    
      SQLDBC_ANSI                     =3,    
      SQLDBC_DB2                      =4,    
      SQLDBC_ORACLE                   =5,    
      SQLDBC_SAPR3                    =6     
 }
 #ifndef __cplusplus
 SQLDBC_SQLMode
 #endif
 ;
 
 #ifdef __cplusplus
 class SQLDBC_StringEncodingType
 {
 public:
   enum Encoding {
     Unknown     = 0,
     Ascii       = 1,
     UCS2        = 2,
     UCS2Swapped = 3,
     UTF8        = 4
   };
 };
 typedef SQLDBC_StringEncodingType::Encoding SQLDBC_StringEncoding;
 #endif
 typedef enum SQLDBC_StringEncodingType_Encoding
 {
     SQLDBC_StringEncodingType_Encoding_Unknown     = 0,
     SQLDBC_StringEncodingType_Encoding_Ascii       = 1,
     SQLDBC_StringEncodingType_Encoding_UCS2        = 2,
     SQLDBC_StringEncodingType_Encoding_UCS2Swapped = 3,
     SQLDBC_StringEncodingType_Encoding_UTF8        = 4
 }SQLDBC_StringEncodingType_Encoding;
 
 #define SQLDBC_StringEncodingUnknown      SQLDBC_StringEncodingType::Unknown
 
 #define SQLDBC_StringEncodingAscii        SQLDBC_StringEncodingType::Ascii
 
 #define SQLDBC_StringEncodingUCS2         SQLDBC_StringEncodingType::UCS2
 
 #define SQLDBC_StringEncodingUCS2Swapped  SQLDBC_StringEncodingType::UCS2Swapped
 
 #if defined(SQLDBC_SWAPPED_UCS2)
 #define SQLDBC_StringEncodingUCS2Native SQLDBC_StringEncodingType::UCS2Swapped
 #else
 #define SQLDBC_StringEncodingUCS2Native SQLDBC_StringEncodingType::UCS2
 #endif
 
 #define SQLDBC_StringEncodingUTF8         SQLDBC_StringEncodingType::UTF8
 
 #ifndef __cplusplus
 typedef
 #endif
 enum SQLDBC_SQLType {
   SQLDBC_SQLTYPE_MIN       = 0,            
   SQLDBC_SQLTYPE_FIXED     = SQLDBC_SQLTYPE_MIN, 
   SQLDBC_SQLTYPE_FLOAT     = 1,            
   SQLDBC_SQLTYPE_CHA       = 2,            
   SQLDBC_SQLTYPE_CHE       = 3,            
   SQLDBC_SQLTYPE_CHB       = 4,            
   SQLDBC_SQLTYPE_ROWID     = 5,            
   SQLDBC_SQLTYPE_STRA      = 6,            
   SQLDBC_SQLTYPE_STRE      = 7,            
   SQLDBC_SQLTYPE_STRB      = 8,            
   SQLDBC_SQLTYPE_STRDB     = 9,            
   SQLDBC_SQLTYPE_DATE      = 10,           
   SQLDBC_SQLTYPE_TIME      = 11,           
   SQLDBC_SQLTYPE_VFLOAT    = 12,           
   SQLDBC_SQLTYPE_TIMESTAMP = 13,           
   SQLDBC_SQLTYPE_UNKNOWN   = 14,           
   SQLDBC_SQLTYPE_NUMBER    = 15,           
   SQLDBC_SQLTYPE_NONUMBER  = 16,           
   SQLDBC_SQLTYPE_DURATION  = 17,           
   SQLDBC_SQLTYPE_DBYTEEBCDIC = 18,         
   SQLDBC_SQLTYPE_LONGA     = 19,           
   SQLDBC_SQLTYPE_LONGE     = 20,           
   SQLDBC_SQLTYPE_LONGB     = 21,           
   SQLDBC_SQLTYPE_LONGDB    = 22,           
   SQLDBC_SQLTYPE_BOOLEAN   = 23,           
   SQLDBC_SQLTYPE_UNICODE   = 24,           
   SQLDBC_SQLTYPE_DTFILLER1 = 25,           
   SQLDBC_SQLTYPE_DTFILLER2 = 26,           
   SQLDBC_SQLTYPE_DTFILLER3 = 27,           
   SQLDBC_SQLTYPE_VOID      = 27,           
   SQLDBC_SQLTYPE_DTFILLER4 = 28,           
   SQLDBC_SQLTYPE_SMALLINT  = 29,           
   SQLDBC_SQLTYPE_INTEGER   = 30,           
   SQLDBC_SQLTYPE_VARCHARA  = 31,           
   SQLDBC_SQLTYPE_VARCHARE  = 32,           
   SQLDBC_SQLTYPE_VARCHARB  = 33,           
   SQLDBC_SQLTYPE_STRUNI    = 34,           
   SQLDBC_SQLTYPE_LONGUNI   = 35,           
   SQLDBC_SQLTYPE_VARCHARUNI = 36,          
   SQLDBC_SQLTYPE_UDT       = 37,           
   SQLDBC_SQLTYPE_ABAPTABHANDLE = 38,       
   SQLDBC_SQLTYPE_DWYDE     = 39,           
   SQLDBC_SQLTYPE_MAX = SQLDBC_SQLTYPE_DWYDE            
 }
 #ifndef __cplusplus
 SQLDBC_SQLType
 #endif
 ;
 
 #ifndef __cplusplus
 typedef
 #endif
 enum SQLDBC_HostType {
     SQLDBC_HOSTTYPE_MIN         = 0, 
     SQLDBC_HOSTTYPE_PARAMETER_NOTSET = 0, 
     SQLDBC_HOSTTYPE_BINARY      =  1, 
     SQLDBC_HOSTTYPE_ASCII       =  2, 
     SQLDBC_HOSTTYPE_UTF8        =  4, 
     SQLDBC_HOSTTYPE_UINT1       =  5, 
     SQLDBC_HOSTTYPE_INT1        =  6, 
     SQLDBC_HOSTTYPE_UINT2       =  7,  
     SQLDBC_HOSTTYPE_INT2        =  8, 
     SQLDBC_HOSTTYPE_UINT4       =  9, 
     SQLDBC_HOSTTYPE_INT4        = 10, 
     SQLDBC_HOSTTYPE_UINT8       = 11, 
     SQLDBC_HOSTTYPE_INT8        = 12, 
     SQLDBC_HOSTTYPE_DOUBLE      = 13, 
     SQLDBC_HOSTTYPE_FLOAT       = 14, 
     SQLDBC_HOSTTYPE_ODBCDATE    = 15, 
     SQLDBC_HOSTTYPE_ODBCTIME    = 16, 
     SQLDBC_HOSTTYPE_ODBCTIMESTAMP = 17, 
     SQLDBC_HOSTTYPE_ODBCNUMERIC = 18, 
     SQLDBC_HOSTTYPE_GUID        = 19, 
     SQLDBC_HOSTTYPE_UCS2        =  20, 
     SQLDBC_HOSTTYPE_UCS2_SWAPPED=  21, 
 #if defined(SQLDBC_SWAPPED_UCS2)
     SQLDBC_HOSTTYPE_UCS2_NATIVE =  SQLDBC_HOSTTYPE_UCS2_SWAPPED, 
 #else
     SQLDBC_HOSTTYPE_UCS2_NATIVE =  SQLDBC_HOSTTYPE_UCS2, 
 #endif
     SQLDBC_HOSTTYPE_BLOB         = 22, 
     SQLDBC_HOSTTYPE_ASCII_CLOB   = 23, 
     SQLDBC_HOSTTYPE_UTF8_CLOB    = 24, 
     SQLDBC_HOSTTYPE_UCS2_CLOB    = 25, 
     SQLDBC_HOSTTYPE_UCS2_SWAPPED_CLOB = 26, 
 #if defined(SQLDBC_SWAPPED_UCS2)
     SQLDBC_HOSTTYPE_UCS2_NATIVE_CLOB =  SQLDBC_HOSTTYPE_UCS2_SWAPPED_CLOB,
 #else
     SQLDBC_HOSTTYPE_UCS2_NATIVE_CLOB =  SQLDBC_HOSTTYPE_UCS2_CLOB,
 #endif
     SQLDBC_HOSTTYPE_STREAM      = 27,  
     SQLDBC_HOSTTYPE_RAWHEX      = 28,  
 #ifdef SQLDBC_FEATURE_DECIMAL
     SQLDBC_HOSTTYPE_DECIMAL         = 29, /*<! BCD encoded decimal number. */
     SQLDBC_HOSTTYPE_OMS_PACKED_8_3  = 30, /*<! OMS packed decimal (a @c FIXED(15,3) ). */
     SQLDBC_HOSTTYPE_OMS_PACKED_15_3 = 31, /*<! OMS packed decimal (a @c FIXED(29,3) ). */
     SQLDBC_HOSTTYPE_OMS_TIMESTAMP   = 32, /*<! OMS timestamp (a @c FIXED(15,0) ). */
 #endif
 
 #ifdef SQLDBC_FEATURE_OMS_ASCII
     SQLDBC_HOSTTYPE_OMS_ASCII       = 33, /*<! Special OMS Ascii type that does allow only 7-bit under certain conditions. */ 
 #endif
 
 #ifdef SQLDBC_FEATURE_DECFLOAT
     SQLDBC_HOSTTYPE_DECFLOAT        = 34, /*<! IEEE-754r floating point number. */
 #endif
 
     SQLDBC_HOSTTYPE_USERDEFINED = 100, 
     SQLDBC_HOSTTYPE_MAX = SQLDBC_HOSTTYPE_USERDEFINED  
 }
 #ifndef __cplusplus
 SQLDBC_HostType
 #endif
 ;
 
 #ifndef __cplusplus
 typedef
 #endif
 enum SQLDBC_SQLWarningCode
 {
     SQLDBC_SQLWARNING_DATA_TRUNCATED = 1, 
     SQLDBC_SQLWARNING_MAX                 
 }
 #ifndef __cplusplus
 SQLDBC_SQLWarningCode
 #endif
 ;
 #ifdef __cplusplus
 struct SQLDBC_DateTimeFormat
 {
     enum Format {
         None_C = 0,      
         Normal_C = 1,    
         Iso_C = 2,       
         Usa_C = 3,       
         Eur_C = 4,       
         Jis_C = 5,       
         OracleDate_C = 6, 
         WasAnsiNowIsSameAsIso_C = 7, 
         TsEur_C = 8      
     };
 };
 #endif
 
 typedef  enum SQLDBC_DateTimeFormat_Format {
         SQLDBC_DateTimeFormat_Format_None_C = 0,      
         SQLDBC_DateTimeFormat_Format_Normal_C = 1,    
         SQLDBC_DateTimeFormat_Format_Iso_C = 2,       
         SQLDBC_DateTimeFormat_Format_Usa_C = 3,       
         SQLDBC_DateTimeFormat_Format_Eur_C = 4,       
         SQLDBC_DateTimeFormat_Format_Jis_C = 5,       
         SQLDBC_DateTimeFormat_Format_OracleDate_C = 6, 
         SQLDBC_DateTimeFormat_Format_WasAnsiNowIsSameAsIso_C = 7, 
         SQLDBC_DateTimeFormat_Format_TsEur_C = 8      
     }SQLDBC_DateTimeFormat_Format;
 
 
 #ifndef __cplusplus
 typedef
 #endif
 
 enum SQLDBC_SerialTag {
     SQLDBC_FIRST_INSERTED_SERIAL = 32768,  
     SQLDBC_LAST_INSERTED_SERIAL  = 32769   
 }
 #ifndef __cplusplus
 SQLDBC_SerialTag;
 #else
 ;
 #endif
 
 #ifndef __cplusplus
 typedef
 #endif
 
 enum SQLDBC_ProfileCountValues {
     
  SQLDBC_ALLOCATECOUNT           = 0 ,   
  SQLDBC_DEALLOCATECOUNT         = 1 ,   
  SQLDBC_CONNECTIONCOUNT         = 2 ,   
  SQLDBC_COMMITCOUNT             = 3 ,   
  SQLDBC_ROLLBACKCOUNT           = 4 ,   
  SQLDBC_SETISOLATIONCOUNT       = 5 ,   
  SQLDBC_SENDCOUNT               = 6 ,   
  SQLDBC_SENDSIZE                = 7 ,   
  SQLDBC_RECEIVESIZE             = 8 ,   
  SQLDBC_CANCELCOUNT             = 9 ,   
  SQLDBC_STATEMENTCOUNT          = 10,   
  SQLDBC_PREPAREDSTATEMENTCOUNT  = 11,   
  SQLDBC_NOTSELECTFETCHOPTIMIZED = 12,   
  SQLDBC_DROPPARSEIDCOUNT        = 13,   
  SQLDBC_DROPCURSORCOUNT         = 14,   
  SQLDBC_DROPLONGDESCCOUNT       = 15,   
  SQLDBC_PREPARECOUNT            = 16,   
  SQLDBC_PARSEINFOHITCOUNT       = 17,   
  SQLDBC_PARSINFOMISSCOUNT       = 18,   
  SQLDBC_PARSEAGAINCOUNT         = 19,   
  SQLDBC_PARSEINFOSQUEEZECOUNT   = 20,   
  SQLDBC_EXECUTECOUNT            = 21,   
  SQLDBC_INSERTCOUNT             = 22,   
  SQLDBC_UPDATECOUNT             = 23,   
  SQLDBC_SELECTCOUNT             = 24,   
  SQLDBC_CALLDBPROCEDURECOUNT    = 25,   
  SQLDBC_DELETECOUNT             = 26,   
  SQLDBC_FETCHCOUNT              = 27,   
  SQLDBC_SQLERRORCOUNT           = 28,   
  SQLDBC_ROWNOTFOUNDCOUNT        = 29,   
  SQLDBC_FETCHROWCOUNT           = 30,   
  SQLDBC_DATAREADSIZE            = 31,   
  SQLDBC_DATAWRITESIZE           = 32,   
  SQLDBC_LONGDATAREADSIZE        = 33,   
  SQLDBC_LONGDATAWRITESIZE       = 34,   
  SQLDBC_PROFILE_MAX             = 35   
  }
 #ifndef __cplusplus
 SQLDBC_ProfileCountValues;
 #else
 ;
 #endif
 
 #endif