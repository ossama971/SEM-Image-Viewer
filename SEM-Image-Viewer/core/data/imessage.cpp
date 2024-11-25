#include "imessage.h"


IMessage::IMessage(int msgId,int type,const QString &msg) : _id(msgId),_type(type),_msg(msg){

}
IMessage::~IMessage(void) {
}

const int IMessage::GetId(void) const {
  return _id;
}

const QString IMessage::GetType(void) const {
  switch (_type) {
  case 0:
    return "Error";
  case 1:
    return "Warning";
  case 2:
    return "Info";
  default:
    return "Unknown";
  }
}

const QString IMessage::GetMessage() const{
    return _msg;
}
const QString IMessage::GetBody() const  {
    return _body;
}
const QString IMessage::GetPath() const  {
    return _path;
}
void IMessage::SetPath(const QString &path){
    this->_path=path;
}
void IMessage::SetBody(const QString &body){
    this->_body=body;
}
