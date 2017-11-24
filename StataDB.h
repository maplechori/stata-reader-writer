

enum DatabaseTypes {
    SQLITE = 0
};


class Database
{
public:
  virtual bool connect() = 0;
  virtual bool close() = 0;
  virtual ~Database() {};
};

