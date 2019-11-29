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
/*
 * Let start your program with a main function
 */
int main(int argc, char *argv[])
{
    //sleep(20);
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

    rc = conn->connect(connectArgs.host, connectArgs.dbname,
                       connectArgs.username, connectArgs.password);
    if(SQLDBC_OK != rc) {
        fprintf(stderr, "Connecting to the database failed %s\n", conn->error().getErrorText());
        return (1);
    }
    printf("Sucessfull connected to %s as user %s\n",
           connectArgs.dbname, connectArgs.username);
    /*
     * Create a new statment object and execute it.
     */
#if 1
    {
        SQLDBC_PreparedStatement *stmt = conn->createPreparedStatement();

        rc = stmt->executeItab(nullptr, false);
        if(SQLDBC_OK != rc) {
            fprintf(stderr, "executeItab %s\n", stmt->error().getErrorText());
            //return (1);
        }
        void *p = stmt->getItabReader();
        if (SQLDBC_OK != rc) {
            fprintf(stderr, "getItabReader %s\n", stmt->error().getErrorText());
        }

        rc = stmt->prepare("SELECT 'Hello world' from DUMMY", SQLDBC_StringEncodingType::UTF8);
        if(SQLDBC_OK != rc) {
            fprintf(stderr, "Prepare failed %s\n", stmt->error().getErrorText());
            return (1);
        }

        rc = stmt->bindParameter(100, SQLDBC_HOSTTYPE_MIN, nullptr, nullptr, 0);
        if (SQLDBC_OK != rc) {
            fprintf(stderr, "bindParameter failed %s\n", stmt->error().getErrorText());
        }

        rc = stmt->execute();
        if(SQLDBC_OK != rc) {
            fprintf(stderr, "Execution failed %s\n", stmt->error().getErrorText());
            return (1);
        }
        /*
         * Check if the SQL command return a resultset and get a result set object.
         */
        SQLDBC_ResultSet *result;
        result = stmt->getResultSet();
        if(!result) {
            fprintf(stderr, "SQL command doesn't return a result set %s\n", stmt->error().getErrorText());
            return (1);
        }
        /*
         * Position the curors within the resultset by doing a fetch next call.
         */
        rc = result->next();
        if(SQLDBC_OK != rc) {
            fprintf(stderr, "Error fetching data %s\n", stmt->error().getErrorText());
            return (1);
        }
        char szString[30];
        SQLDBC_Length ind;
        /*
         * Get a string value from the column.
         */
        rc = result->getObject(1, SQLDBC_HOSTTYPE_ASCII, szString, &ind, sizeof(szString));
        if(SQLDBC_OK != rc) {
            fprintf(stderr, "Error getObject %s\n", stmt->error().getErrorText());
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