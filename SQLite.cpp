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


    for (int j = 0; j < ctx.vList.size(); j++)
    {
        sql_query << "insert into variables values ( NULL," 
                     "'" << ((StataVariables)*ctx.vList[j]).varname << 
                     "'," << ((StataVariables)*ctx.vList[j]).type <<
                     ",'" << ((StataVariables)*ctx.vList[j]).format << "'" <<
                     ",'" << ((StataVariables)*ctx.vList[j]).vallbl << "'" <<
                     ",'" << ((StataVariables)*ctx.vList[j]).varlbl << "')"; 
    
        cout << sql_query.str().c_str() << endl;
        rc = sqlite3_exec(db, sql_query.str().c_str() , NULL, 0, &zErrMsg);   

        if( rc != SQLITE_OK )
        {
            cerr << "SQL error: " << zErrMsg << endl;
            sqlite3_free(zErrMsg);
            return false;
        }

        sql_query.str("");
    }  
  
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
    
    sqlite3_stmt *stmt = NULL;
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
                        //sql_query << /*GetLSF<double>(*/(*ctx.vData[k])[j]/*, 8)*/ << ", 8);";
                        sz=8;
                        break;
                    case ST_FLOAT:
                        //sql_query << /*GetLSF<float>(*/(*ctx.vData[k])[j] /*, 4)*/ << ", 4);";
                        sz=4;
                        break;
                    case ST_LONG:
                        //sql_query << /*GetLSF<long>(*/(*ctx.vData[k])[j] /*, 4)*/ << ", 4);";
                        sz=4;
                        break;
                    case ST_INT:
                        //sql_query << /*GetLSF<short>(*/(*ctx.vData[k])[j] /*, 2)*/ << ", 2);";
                        sz=2;
                        break;
                    case ST_BYTE:
                        //sql_query << /*GetLSF<int8_t>(*/(*ctx.vData[k])[j]/*, 1)*/ << ", 1);";
                        sz=1;
                        break;
                    default: 
                        //sql_query << /*"'" <<  GetLSF<string>(*/(*ctx.vData[k])[j] /*, strlen((*ctx.vData[k])[j]) << "'" << */ << "," << strlen((*ctx.vData[k])[j]) << ");";                
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
                
                sqlite3_reset(stmt);
            }
        }
    } 
    
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