#ifndef IMESSAGE_H
#define IMESSAGE_H


#include <QString>
class IMessage {
public:
  enum class MessageLevel : uint8_t {
    ERROR = 1,
    WARNING,
    INFO,
  };


public:

    IMessage(int msgId,int type,const QString &msg);
   ~IMessage();

   const int GetId() const;
   const QString GetType() const;
   const QString GetMessage() const;
   const QString GetPath() const  ;
   void SetPath(const QString &path)  ;

protected:
  int _id;
  int _type;
  QString _msg;
  QString _path;
};

#endif
