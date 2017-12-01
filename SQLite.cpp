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
    
 

    for (int j = 0; j < ctx.vList.size(); j++)
    {
        sql_query << ((StataVariables)*ctx.vList[j]).varname << endl;
    }  

    cout << sql_query.str() << endl;
    sql_query.str("");

    rc = sqlite3_exec(db, "create table variables (obs integer primary key asc, varname varchar(255), vartype integer, varformat varchar(20), valuelabel varchar(255), variablelabel varchar(255));" , NULL, 0, &zErrMsg);
    
    if( rc != SQLITE_OK )
    {
      cerr << "SQL error: " << zErrMsg << endl;
      sqlite3_free(zErrMsg);
      return false;
    }



    for (int k = 0; k < ctx.vData.size(); k++)
    {
          sql_query << "insert into variables values ( NULL, ";

        for (int j = 0; j < ctx.vList.size(); j++)
        {
            switch(ctx.vList[j]->type)
            {
                case ST_STRL:
                    cout << "Strl " << endl;             
                    break;
                case ST_DOUBLE:
                    sql_query << "'";
                    sql_query << GetLSF<double>(ctx.vData[k][j], 8);
                    sql_query << "'";
                    break;
                case ST_FLOAT:
                    sql_query << "'";
                    sql_query << GetLSF<float>(ctx.vData[k][j], 4);
                    sql_query << "'";
                    break;
                case ST_LONG:
                    sql_query << "'";
                    sql_query << GetLSF<long>(ctx.vData[k][j], 4);
                    sql_query << "'";
                    break;
                case ST_INT:
                    sql_query << "'";
                    sql_query << GetLSF<short>(ctx.vData[k][j], 2);
                    sql_query << "'";
                    break;
                case ST_BYTE:
                    sql_query << "'";
                    sql_query << GetLSF<int8_t>(ctx.vData[k][j], 1);
                    sql_query << "'";                
                    break;
                default: 
                    sql_query << "'"; 
                    cout << GetLSF<string>(ctx.vData[k][j], ctx.vData[k].size()) << endl;
                    sql_query << GetLSF<string>(ctx.vData[k][j], ctx.vData[k].size());
                    sql_query << "'";                
                    break;

            }                  

            if (ctx.vList.size() - 1 == j)
                sql_query << ")";
            else
                sql_query << ","; 
         
      
        }
            
             //cout << sql_query.str() << endl;
             //cout << "[" << k << "]**********************************" << endl;
             sql_query.str("");
    }    

    //string sql_statement = "insert into variables values ( NULL, '";
    //sqlite3_exec(db, sql_statement.c_str());
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