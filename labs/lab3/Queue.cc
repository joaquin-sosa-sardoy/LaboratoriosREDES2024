#ifndef QUEUE
#define QUEUE

#include <string.h>
#include <omnetpp.h>

using namespace omnetpp;

class Queue: public cSimpleModule {
private:
    cQueue buffer;
    cMessage *endServiceEvent;
    simtime_t serviceTime;

    //Agregados debido al enunciado.
    //bufferSizeVector y packetDropVector son instancias de la clase cOutVector de la lib. Omnet++
    int packetsDropped;
    cOutVector bufferSizeVector;
    cOutVector packetsDroppedVector;

    //funciones privadas
    void enviarPaquete();
    void encolarMensaje(cMessage *msg);
public:
    Queue();
    virtual ~Queue();
protected:
    virtual void initialize();
    virtual void finish();
    virtual void handleMessage(cMessage *msg);
};

Define_Module(Queue);

Queue::Queue() {
    endServiceEvent = NULL;
}

Queue::~Queue() {
    cancelAndDelete(endServiceEvent);
}

void Queue::enviarPaquete() {
    if (!buffer.isEmpty()) {
        // dequeue packet
        cPacket *pkt = (cPacket *) buffer.pop();
        
        // send packet
        send(pkt, "out");

        // start new service
        simtime_t serviceTime = pkt->getDuration();
        scheduleAt(simTime() + serviceTime, endServiceEvent);
    }
}

void Queue::encolarMensaje(cMessage *msg) {
    // check buffer limit
    if (buffer.getLength() >= par("bufferSize").intValue()) {
        // drop the packet
        delete msg;
        
        this->bubble("packet dropped");

        // actualizar datos
        packetsDropped++;
        packetsDroppedVector.record(packetsDropped);
    } else {
        // enqueue the packet
        buffer.insert(msg);
        bufferSizeVector.record(buffer.getLength());

        // if the server is idle
        if (!endServiceEvent->isScheduled()) {
            // start the service now
            scheduleAt(simTime() + 0, endServiceEvent);
        }
    }
}

void Queue::initialize() {
    buffer.setName("buffer");
    endServiceEvent = new cMessage("endService");
    
    //Inicializo con nombre
    packetsDropped = 0;
    bufferSizeVector.setName("Buffer size");
    packetsDroppedVector.setName("Packet Drop");
}

void Queue::finish() {
    recordScalar("Paquetes dropeados", packetsDropped); // Al terminar quiero saber la cantidad de paquetes que perdi
}

void Queue::handleMessage(cMessage *msg) {

    // if msg is signaling an endServiceEvent
    if (msg == endServiceEvent) {
        // if packet in buffer, send next one
        enviarPaquete();
    } else { 
        encolarMensaje(msg);
    }
}

#endif /* QUEUE */
