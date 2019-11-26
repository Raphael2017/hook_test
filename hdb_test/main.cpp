/*
 * First you have to include SQLDBC.h
 */
#include "SQLDBC.h"
#include <unistd.h>
#include <stdio.h>
//#include "subhook.h"
#include <dlfcn.h>

typedef struct ConnectArgsT {
    char * username;
    char * password;
    char * dbname;
    char * host;
} ConnectArgsT;

static void parseArgs (ConnectArgsT * connectArgs, int argc, char **argv);

using namespace SQLDBC;

typedef SQLDBC_Retcode (*ConnectFun)( SQLDBC::SQLDBC_Connection *self,
                                      const char *servernode,
                                      const char *serverdb,
                                      const char *username,
                                      const char *password);
//subhook::Hook *hk = nullptr;
SQLDBC_Retcode connect_new( SQLDBC::SQLDBC_Connection *self,
                            const char *servernode,
                            const char *serverdb,
                            const char *username,
                            const char *password) {
    printf("self: %p, servernode: %s, serverdb: %s, username: %s, password: %s\n", self, servernode, serverdb, username, password);
    //printf("GetTrampoline: %p\n", hk->GetTrampoline());
    //return ((ConnectFun)hk->GetTrampoline())(self, servernode, serverdb, username, password);
}

void loadMsg() {
    void *h = dlopen("libSQLDBCHDB.so", RTLD_LAZY);
    if (h == NULL)
        return;
    void *p = dlsym(h, "_ZN6SQLDBC17SQLDBC_Connection7connectEPKcS2_S2_S2_");
    if (p != NULL) {
        /*
        hk = new subhook::Hook(p, (void*)&connect_new, subhook::HookFlag64BitOffset);
        printf("GetTrampoline: %p\n", hk->GetTrampoline());
        if (hk->Install()) {
            printf("GetTrampoline: %p\n", hk->GetTrampoline());
        } else {
            printf("hook install fail\n");
        }*/
    }
}

/*
 * Let start your program with a main function
 */
int main(int argc, char *argv[])
{
    loadMsg();
    sleep(25);
    ConnectArgsT connectArgs;
    parseArgs (&connectArgs, argc, argv);

    char errorText[200];
    /*
     * Every application has to initialize the SQLDBC library by getting a
     * reference to the ClientRuntime and calling the SQLDBC_Environment constructor.
     */
    SQLDBC_IRuntime *runtime;
    runtime = SQLDBC::GetClientRuntime(errorText, sizeof(errorText));
    if (!runtime) {
        fprintf(stderr, "Getting instance of the ClientRuntime failed %s", errorText);
        return (1);
    }
    SQLDBC_Environment env((Runtime*)runtime);
    /*
     * Create a new connection object and open a session to the database.
     */
    SQLDBC_Connection *conn = env.createConnection();
    SQLDBC_Retcode rc;
    rc = conn->connect(0/*connectArgs.host*/, /*connectArgs.dbname*/ 0,
                       /*connectArgs.username*/ 0, /*connectArgs.password*/ 0);
    if(SQLDBC_OK != rc) {
        fprintf(stderr, "Connecting to the database failed %s", conn->error().getErrorText());
        return (1);
    }
    printf("Sucessfull connected to %s as user %s\n",
           connectArgs.dbname, connectArgs.username);
    /*
     * Create a new statment object and execute it.
     */
#if 0
    {
        SQLDBC_PreparedStatement *stmt = conn->createPreparedStatement();
        rc = stmt->prepare("SELECT 'Hello world' from DUMMY");
        rc = stmt->execute();
        if(SQLDBC_OK != rc) {
            fprintf(stderr, "Execution failed %s", stmt->error().getErrorText());
            return (1);
        }
        /*
         * Check if the SQL command return a resultset and get a result set object.
         */
        SQLDBC_ResultSet *result;
        result = stmt->getResultSet();
        if(!result) {
            fprintf(stderr, "SQL command doesn't return a result set %s", stmt->error().getErrorText());
            return (1);
        }
        /*
         * Position the curors within the resultset by doing a fetch next call.
         */
        rc = result->next();
        if(SQLDBC_OK != rc) {
            fprintf(stderr, "Error fetching data %s", stmt->error().getErrorText());
            return (1);
        }
        char szString[30];
        SQLDBC_Length ind;
        /*
         * Get a string value from the column.
         */
        rc = result->getObject(1, SQLDBC_HOSTTYPE_ASCII, szString, &ind, sizeof(szString));
        if(SQLDBC_OK != rc) {
            fprintf(stderr, "Error getObject %s", stmt->error().getErrorText());
            return (1);
        }
        printf("%s\n", szString);
        /*
         * Finish your program with a returncode.
         */
        return 0;
    }
#endif

    SQLDBC_Statement *stmt = conn->createStatement();


    rc = stmt->execute("SELECT 'Hello world' from DUMMY");
    if(SQLDBC_OK != rc) {
        fprintf(stderr, "Execution failed %s", stmt->error().getErrorText());
        return (1);
    }
    /*
     * Check if the SQL command return a resultset and get a result set object.
     */
    SQLDBC_ResultSet *result;
    result = stmt->getResultSet();
    if(!result) {
        fprintf(stderr, "SQL command doesn't return a result set %s", stmt->error().getErrorText());
        return (1);
    }
    /*
     * Position the curors within the resultset by doing a fetch next call.
     */
    rc = result->next();
    if(SQLDBC_OK != rc) {
        fprintf(stderr, "Error fetching data %s", stmt->error().getErrorText());
        return (1);
    }
    char szString[30];
    SQLDBC_Length ind;
    /*
     * Get a string value from the column.
     */
    rc = result->getObject(1, SQLDBC_HOSTTYPE_ASCII, szString, &ind, sizeof(szString));
    if(SQLDBC_OK != rc) {
        fprintf(stderr, "Error getObject %s", stmt->error().getErrorText());
        return (1);
    }
    printf("%s\n", szString);
    /*
     * Finish your program with a returncode.
     */
    return 0;
}

static void parseArgs (ConnectArgsT * connectArgs, int argc, char **argv)
{
    /*
     * setting defaults for demo database
     */
    connectArgs->username = (char*)"SYSTEM";
    connectArgs->password = (char*)"123Blue123Blue";
    connectArgs->dbname = (char*)"HXE";
    connectArgs->host = (char*)"10.23.60.140:39015";
    /*
     * use values from command line
     */
    if (argc > 4) {
        connectArgs->host = argv [4];
    }
    if (argc > 3) {
        connectArgs->dbname = argv [3];
    }
    if (argc > 2) {
        connectArgs->password = argv [2];
    }
    if (argc > 1) {
        connectArgs->username = argv [1];
    }
}