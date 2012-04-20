#include "services.h"

Service::Service(char id)
{
    _id = id;
    _prot = NULL;
}

void Service::init(Protocol *prot)
{
    _prot = prot;
}

bool Service::match(const Packet &msg)
{
    return msg.command == _id ? true : false;    
}



PingService::PingService() : Service(PROT_PING)
{
}

bool PingService::execute(Packet &msg)
{
    // i just forward the message
    _prot->send(msg);
    return true;
}
