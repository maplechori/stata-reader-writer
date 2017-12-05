#include "SQLite.h"
#include <cstdio>
#include <sstream>
#include <boost/shared_ptr.hpp>


static int callback(void *NotUsed, int argc, char **argv, char **azColName){
    int i;
    
    for(i=0; i<argc; i++){
        // printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    
    //printf("\n");
    return 0;
  }

SQLite::SQLite() {

}
  
bool SQLite::write(Context & ctx) {
    char *zErrMsg = 0;
    int rc;
    stringstream sql_query;
    sqlite3_stmt *stmt = NULL;

    


    rc = sqlite3_exec(db,  "create table variables ("
                           "varpos integer primary key asc,"
                           "varname varchar(255),"
                           "vartype integer,"
                           "varformat varchar(20),"
                           "valuelabel varchar(255),"
                           "variablelabel varchar(255));" , NULL, 0, &zErrMsg);
    
    if( rc != SQLITE_OK )
    {
      cerr << "SQL error: " << zErrMsg << endl;
      sqlite3_free(zErrMsg);
      return false;
    }

    sql_query.str("");

    rc = sqlite3_prepare_v2(db, "INSERT INTO VARIABLES (VARPOS, VARNAME, VARTYPE, VARFORMAT, VALUELABEL, VARIABLELABEL ) VALUES ( NULL, ?, ?, ?, ?, ? )", -1, &stmt, NULL);

    if (rc != SQLITE_OK) {
        cerr << "SQLError prepare failed: " << sqlite3_errmsg(db) << endl;
        return false;
    } 

    sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, &zErrMsg);

    for (int j = 0; j < ctx.vList.size(); j++)
    {
        rc = sqlite3_bind_text(stmt, 1, ((StataVariables)*ctx.vList[j]).varname.c_str(), ((StataVariables)*ctx.vList[j]).varname.length(), SQLITE_STATIC);

        if (rc != SQLITE_OK) {
            cerr << "SQLError: varname bind failed: " << sqlite3_errmsg(db) << endl;
            return false;
        }

        rc = sqlite3_bind_int(stmt, 2, ((StataVariables)*ctx.vList[j]).type);

        if (rc != SQLITE_OK) {
            cerr << "SQLError: type bind failed: " << sqlite3_errmsg(db) << endl;
            return false;
        }

        rc = sqlite3_bind_text(stmt, 3, ((StataVariables)*ctx.vList[j]).format.c_str(), ((StataVariables)*ctx.vList[j]).format.length(), SQLITE_STATIC);

        if (rc != SQLITE_OK) {
            cerr << "SQLError: format bind failed: " << sqlite3_errmsg(db) << endl;
            return false;
        }

        rc = sqlite3_bind_text(stmt, 4, ((StataVariables)*ctx.vList[j]).vallbl.c_str(), ((StataVariables)*ctx.vList[j]).vallbl.length(), SQLITE_STATIC);

        if (rc != SQLITE_OK) {
            cerr << "SQLError: vallbl bind failed: " << sqlite3_errmsg(db) << endl;
            return false;
        }

        rc = sqlite3_bind_text(stmt, 5, ((StataVariables)*ctx.vList[j]).varlbl.c_str(), ((StataVariables)*ctx.vList[j]).varlbl.length(), SQLITE_STATIC);

        if (rc != SQLITE_OK) {
            cerr << "SQLError: varlbl bind failed: " << sqlite3_errmsg(db) << endl;
            return false;
        }
        
        rc =  sqlite3_step(stmt);

        if( rc != SQLITE_DONE )
        {
            cerr << "SQL error: "  << sqlite3_errmsg(db) << endl;
            return false;
        }
                    
        sqlite3_clear_bindings(stmt);    /* Clear bindings */
        sqlite3_reset(stmt);
    

    }  

    sqlite3_exec(db, "END TRANSACTION", NULL, NULL, &zErrMsg);

  
    rc = sqlite3_exec(db,  "create table vardata ("
                           "obs integer,"
                           "varpos integer,"
                           "value blob,"
                           "sizer int);" , NULL, 0, &zErrMsg);
    
    if( rc != SQLITE_OK )
    {
      cerr << "SQL error: " << zErrMsg << endl;
      sqlite3_free(zErrMsg);
      return false;
    }

    sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, &zErrMsg);

    rc = sqlite3_prepare_v2(db, "INSERT INTO VARDATA ( OBS, VARPOS, VALUE, SIZER ) VALUES ( ?, ?, ?, ? )", -1, &stmt, NULL);
    
    if (rc != SQLITE_OK) {
        cerr << "SQLError prepare failed: " << sqlite3_errmsg(db) << endl;
        return false;
    } 
    else 
    {
    
        for (int k = 0; k < ctx.vData.size(); k++)
        {
            for (int j = 0; j < ctx.vList.size(); j++)
            {
                int sz = 0;
                rc = sqlite3_bind_int(stmt, 1, k+1);

                
                if (rc != SQLITE_OK) {
                    cerr << "SQLError: obs bind failed: " << sqlite3_errmsg(db) << endl;
                    return false;
                }

                rc = sqlite3_bind_int(stmt, 2, j+1);
                if (rc != SQLITE_OK) {
                    cerr << "SQLError: varpos bind failed: " << sqlite3_errmsg(db) << endl;
                    return false;
                }                 
                
                switch(ctx.vList[j]->type)
                {
                    case ST_STRL:
                        cout << "Strl " << endl;             
                        break;
                    case ST_DOUBLE:  
                        sz=8;
                        break;
                    case ST_FLOAT:
                        sz=4;
                        break;
                    case ST_LONG:
                        sz=4;
                        break;
                    case ST_INT:
                        sz=2;
                        break;
                    case ST_BYTE:
                        sz=1;
                        break;
                    default: 
                        sz=strlen((*ctx.vData[k])[j]);
                        break;
                }                  

                rc = sqlite3_bind_blob(stmt,3, (*ctx.vData[k])[j], sz, SQLITE_STATIC);
                
                if (rc != SQLITE_OK) {
                    cerr << "SQLError blob bind failed: " << sqlite3_errmsg(db) << endl;
                    return false;
                } 

                rc = sqlite3_bind_int(stmt, 4, sz);
                if (rc != SQLITE_OK) {
                    cerr << "SQLError: sizer bind failed: " << sqlite3_errmsg(db) << endl;
                    return false;
                } 
                
                rc =  sqlite3_step(stmt);

                if( rc != SQLITE_DONE )
                {
                    cerr << "SQL error: "  << sqlite3_errmsg(db) << endl;
                    return false;
                }
                    
                sqlite3_clear_bindings(stmt);    /* Clear bindings */
                sqlite3_reset(stmt);
            }
        }
    } 
    
    sqlite3_exec(db, "END TRANSACTION", NULL, NULL, &zErrMsg);
    sqlite3_finalize(stmt);    
    return true;
}

bool SQLite::connect(void * ptr) {

    char *zErrMsg = 0;
    int rc;
    char * filename = (char *)ptr;
  
    rc = sqlite3_open(filename, &db);
    
    if (rc)
    {
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      sqlite3_close(db);
      return false;
    }

    return true;
}

bool SQLite::close() {

    return true;
}

SQLite::~SQLite() {

}