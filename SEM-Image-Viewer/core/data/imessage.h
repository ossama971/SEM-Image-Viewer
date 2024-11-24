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

    IMessage(int msgId,int type,QString msg);
   ~IMessage();

   int GetId() const;
   QString GetType() const;
   QString GetMessage() const;
   QString GetBody() const  ;
   QString GetPath() const  ;
   void SetPath(QString path)  ;
   void SetBody(QString body)  ;

protected:
  int _id;
  int _type;
  QString _msg;
  QString _body;
  QString _path;
};

#endif
