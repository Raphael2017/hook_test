#include "microtest.h"
#include "interface.h"
#include "SQLDBC.h"

using namespace SQLDBC;

TEST(TABLE_META_DATA) {
    
    char errorText[200];
    /*
     * Every application has to initialize the SQLDBC library by getting a
     * reference to the ClientRuntime and calling the SQLDBC_Environment constructor.
     */
    SQLDBC_IRuntime *runtime;
    runtime = SQLDBC::GetClientRuntime(errorText, sizeof(errorText));
    if (!runtime) {
        fprintf(stderr, "Getting instance of the ClientRuntime failed %s", errorText);
    }
    SQLDBC_Environment env((Runtime*)runtime);
    /*
     * Create a new connection object and open a session to the database.
     */
    SQLDBC_Connection *conn = env.createConnection();
    SQLDBC_Retcode rc;

    rc = conn->connect("10.23.60.140:39015", "HXE",
    "SYSTEM", "123Blue123Blue");
    if(SQLDBC_OK != rc) {
        fprintf(stderr, "Connecting to the database failed %s\n", conn->error().getErrorText());
    }
    S4HException exception;
    auto t = create_table_metadata(conn, u"USERS", exception);
    ASSERT_NOTNULL(t);
    ASSERT_TRUE(exception._code == S4H_SUCCESS);
    ASSERT_TRUE(t->HasColumn(u"USER_ID"));
    ASSERT_TRUE(!t->HasColumn(u"ABC"));
    ASSERT_TRUE(t->IsColumnCompatibleWithString(u"CITY"));
    delete (t); t = nullptr;

    t = create_table_metadata(conn, u"USERS_ATTRIBUTES", exception);
    ASSERT_NOTNULL(t);
    ASSERT_TRUE(exception._code == S4H_SUCCESS);
    delete (t); t = nullptr;

    t = create_table_metadata(conn, u"ABC", exception);
    ASSERT_FALSE(exception._code == S4H_SUCCESS);
    ASSERT_NULL(t);
    env.releaseConnection(conn); conn = nullptr;
}