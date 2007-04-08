//-*-C++-*-
#if !defined(ERROROBJ_H)
#define      ERROROBJ_H
#include <string>
#include <iostream>

using namespace std;

class ErrorObj{
 public:
  enum {Informational=100,Recoverable,Severe,Fatal};
  ErrorObj() {Id.resize(0);Msg.resize(0);Src.resize(0);Message.resize(0);};
  ErrorObj(const char *m,const char *i,int l=0);
  ~ErrorObj() {};
  
  void SetSource(const char *m=0);
  const char *Source()               {return Src.c_str();}
  int Severity()                     {return Level;}
  const char *what();

  ostream &operator<<(const char *m) {return cerr << m;}
  ostream &operator<<(ErrorObj &E)   {return cerr << E;}
  friend ostream &operator<<(ostream& o,const ErrorObj&);

 private:
  string Id,Msg,Src,Message;
  int Level;
};

ostream &operator<<(ostream& o, const ErrorObj &E);

#endif
