#include <ErrorObj.h>

ErrorObj::ErrorObj(const char *m, const char *i, int l)
  //  :Id(i),Msg(m),Src(NULL),Message(NULL)
{
  Id = i; Msg = m;
  Level=l;
}

void ErrorObj::SetSource(const char *s)
{
  if (s) {Src = s;}
}

const char* ErrorObj::what()
{
  Message=Id+": "+Msg;
  return Message.c_str();
}

ostream &operator<<(ostream& o, const ErrorObj &E)
{
  if (E.Id.size() > 0)   o << E.Id;
  if (E.Msg.size() > 0)  o <<": "<< E.Msg;

  return o;
}
