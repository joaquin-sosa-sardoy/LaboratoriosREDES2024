#ifndef GENERATOR
#define GENERATOR

#include <string.h>
#include <omnetpp.h>

using namespace omnetpp;

class Generator : public cSimpleModule {
private:
    cMessage *sendMsgEvent;
    cStdDev transmissionStats;

    //Agregados
    int packetsSent;
    //tal vez no haga falta (no me queda claro QUE es transmissionStats)
    cOutVector packetsSentVector;

    //funciones privadas
    void enviarPaquete();
public:
    Generator();
    virtual ~Generator();
protected:
    virtual void initialize();
    virtual void finish();
    virtual void handleMessage(cMessage *msg);
};
Define_Module(Generator);

Generator::Generator() {
    sendMsgEvent = NULL;

}

Generator::~Generator() {
    cancelAndDelete(sendMsgEvent);
}

//Agregado del enunciado
void Generator::enviarPaquete() {
    cPacket *pkt = new cPacket("packet");
    pkt->setByteLength(par("packetByteSize"));
    //lo generamos y enviamos en la misma funcion
    send(pkt, "out");
}

void Generator::initialize() {
    transmissionStats.setName("TotalTransmissions");
    // inicializamos valores
    packetsSent = 0;
    packetsSentVector.setName("Sent Packets");
    // create the send packet
    sendMsgEvent = new cMessage("sendEvent");
    // schedule the first event at random time
    scheduleAt(par("generationInterval"), sendMsgEvent);
}

void Generator::finish() {
    recordScalar("Paquetes enviados", packetsSent); // Al terminar quiero saber la cantidad de paquetes que mande
}

void Generator::handleMessage(cMessage *msg) {
    // llamada para enviar un nuevo paquete
    enviarPaquete();
    // actualizar datos
    packetsSent++;
    packetsSentVector.record(packetsSent);

    // compute the new departure time
    simtime_t departureTime = simTime() + par("generationInterval");
    // schedule the new packet generation
    scheduleAt(departureTime, sendMsgEvent);
}

#endif /* GENERATOR */
