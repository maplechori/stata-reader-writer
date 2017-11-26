#include "SQLite.h"
#include <cstdio>

static int callback(void *NotUsed, int argc, char **argv, char **azColName){
    int i;
    
    for(i=0; i<argc; i++){
        // printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    
    //printf("\n");
    return 0;
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
  
    /*
    rc = sqlite3_exec(db, "create table variables ( varname text not null ); ", callback, 0, &zErrMsg);
    
    if( rc != SQLITE_OK )
    {
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    }
    
    sqlite3_close(db);
    
    return 0;
  
  
    return true; */
}

bool SQLite::close() {

    return true;
}

SQLite::~SQLite() {

}