#include "IMessage.h"


IMessage::IMessage(int msgId,int type,QString msg) : _id(msgId),_type(type),_msg(msg){

}
IMessage::~IMessage(void) {
}

int IMessage::GetId(void) const {
  return _id;
}

QString IMessage::GetType(void) const {
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

QString IMessage::GetMessage() const{
    return _msg;
}
QString IMessage::GetBody() const  {
    return _body;
}
QString IMessage::GetPath() const  {
    return _path;
}
void IMessage::SetPath(QString path){
    this->_path=path;
}
void IMessage::SetBody(QString body){
    this->_body=body;
}
