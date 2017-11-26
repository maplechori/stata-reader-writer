#include "Stata.h"
#include "StataDB.h"
#include <sqlite3.h>


class SQLite : public Database {
  
public:
    bool connect(void *);
    bool close();
    ~SQLite();
private:
    sqlite3 *db;
    

};