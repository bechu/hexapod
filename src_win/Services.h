#ifndef SERVICES_H
#define SERVICES_H

#include "protocol.h"

class Service
{
public:
	Service(char id);
	void init(Protocol *prot);
    bool match(const Packet &msg);
	virtual bool execute(Packet &msg) = 0;
protected:
	char _id;
	Protocol *_prot;
};

class PingService : public Service
{
public:
    PingService();
    bool execute(Packet &msg);
};

#endif

