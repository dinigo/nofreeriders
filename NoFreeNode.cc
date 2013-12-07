//
// Copyright (c) 2013 Daniel Iñigo, Efren Suarez, Yuriy Batrakov, José Sklatz
//
// Permission is hereby granted, free of charge, to any
// person obtaining a copy of this software and associated
// documentation files (the "Software"), to deal in the
// Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the
// Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice
// shall be included in all copies or substantial portions of
// the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY
// KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
// WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
// PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS
// OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

//
// file: NoFreeNode.cc
// author: Daniel I�igo, Efr�n Su�rez
//

#include <stdio.h>
#include <string.h>
#include <map>
#include <set>
#include <omnetpp.h>
#include <sstream>

#include "NoFreeNode.h"
#include "NoFreeMessage_m.h"

// Declara el módulo para que pueda usarse en el archivo de topología
Define_Module(NoFreeNode);

using namespace std;

NoFreeNode::NoFreeNode() : NOBODY(-1){ }

NoFreeNode::~NoFreeNode()
{
    cancelAndDelete(reputationRequestTimer);
    cancelAndDelete(fileRequestTimer);
    cancelAndDelete(downloadFileTimer);
}

void NoFreeNode::initialize()
{
    const int NOBODY = -1;
    requiredShareRate       = par("requiredShareRate");
    // Indicadores de archivo pedido a este nodo y siendo servido por él.
    nodeRequested = NOBODY;
    nodeServed    = NOBODY;
    // Lee los valores de las variables desde el archivo de topología.
    reputationTimeout       = par("reputationTimeout");
    reputationRequestTimeout= par("reputationRequestTimeout");
    fileRequestTimeout      = par("fileRequestTimeout");
    downloadFileTimeout     = par("downloadFileTimeout");
    kindness                = par("kindness");
    // Instancia los timer con un mensaje descriptivo.
    reputationRequestTimer  = new cMessage("reputationRequestTimer");
    fileRequestTimer        = new cMessage("fileRequestTimer");
    downloadFileTimer       = new cMessage("downloadFileTimer");
    // Encolo la primera descarga dentro de un tiempo "downloadFileTimeout".
    downloadFileTimeout     = par("downloadFileTimeout");

    WATCH(nodeRequested);
    WATCH(nodeServed);
    WATCH(tempReputation);
    WATCH_SET(nodeContributed);
    WATCH_MAP(nodeMap);
    WATCH(kindness);

    scheduleAt(simTime()+downloadFileTimeout, downloadFileTimer);
    updateDisplay();
}

void NoFreeNode::fileRequest()
{
    // Elijo a la persona de entre los conectados a mi.
    int n = gateSize("dataGate$o");
    int k = intuniform(0,n-1);
    // Construyo un paquete.
    FileRequest *frmsg = new FileRequest("fileRequest");
    // Recupero la ID del módulo conectado por esa puerta
    nodeRequested = gate("dataGate$o", k)->getNextGate()->getOwnerModule()->getId();
    frmsg->setSourceNodeId(getId());
    frmsg->setDestinationNodeId(nodeRequested);
    // Le envío una petición.
    send(frmsg, "dataGate$o", k);
    EV<<"Nodo["<<getIndex()<<"]:    FileRequest->Nodo["<<nodeRequested<<"]"<<endl;
    // Si no tengo reputación del nodo al que pido, la creo.
    if(nodeMap.find(nodeRequested) == nodeMap.end()){
        nodeMap[nodeRequested] = PeerReputation();
    }
    // Considero que es un buen mal par de entrada y no le subo las peticiones aceptadas.
    nodeMap[nodeRequested].totalRequest++;
    // Encolo un nuevo evento dentro de un tiempo aleatorio.
    downloadFileTimeout = par("downloadFileTimeout");
    scheduleAt(simTime()+downloadFileTimeout, downloadFileTimer);
    updateDisplay();
}

void NoFreeNode::handleTimerEvent( cMessage *msg )
{
    // Si no está conectado a ningún otro nodo no pedir archivos
    if(gateSize("dataGate$o")==0) return;
    // Es hora de descargarse un archivo de alguien.
    if(msg == downloadFileTimer){
        fileRequest();
    }
    // He pedido un archivo y no me lo han dado, pongo mala reputación.
    else if(msg == fileRequestTimer){
        if(nodeRequested != NOBODY){
            nodeRequested=NOBODY;
        }
    }
    // Ha expirado el tiempo para recibir reputación, decidir si se envía o no.
    else if(msg == reputationRequestTimer){
        reputationRequest();
    }
    updateDisplay();
}

void NoFreeNode::handleMessage( cMessage *msg )
{
    // Si es un automensaje vemos qué timer ha saltado
    if(msg->isSelfMessage()){
        handleTimerEvent(msg);
        return; // No es buena práctica poenr aquí return, pero la alternativa es peor.
    }
    // Se hace cast al tipo de mensaje que heredan todos los demás
    NoFreeMessage *auxmsg = check_and_cast<NoFreeMessage *>(msg);
    // Se checkea el TTL para ver si ha hecho demasiados saltos ya.
    int ttl = auxmsg->getTtl();
    if(ttl==0){
        cancelAndDelete(msg);
        return;
    }
    else auxmsg->setTtl(ttl-1);
    // Según su tipo se hace casting al tipo adecuado y se pasa a la función.
    switch(auxmsg->getMessageTipe()){
        case FILE_REQUEST:
        {
            FileRequest *auxmsg = check_and_cast<FileRequest *>(msg);
            handleFileRequest(auxmsg);
            break;
        }
        case REPUTATION_REQUEST:
        {
            ReputationRequest *auxmsg = check_and_cast<ReputationRequest *>(msg);
            handleReputationRequest(auxmsg);
            break;
        }
        case FILE_RESPONSE:
        {
            File *auxmsg = check_and_cast<File *>(msg);
            handleFileResponse(auxmsg);
            break;
        }
        case REPUTATION_RESPONSE:
        {
            Reputation *auxmsg = check_and_cast<Reputation *>(msg);
            handleReputationResponse(auxmsg);
        }
    }
}

void NoFreeNode::handleFileRequest( FileRequest *msg )
{
    // Si ya estamos sirviendo a otro nodo salimos.
    if(nodeServed != NOBODY){
        cancelAndDelete(msg);
        return;
    }
    //dentro de ahora mas el timer de reputation me mando el sms de repitationRequestTimer
    else scheduleAt(simTime()+reputationRequestTimeout, reputationRequestTimer);
    // Borra la lista de nodos de los que se ha recibido reputación.
    nodeContributed.clear();
    // Mira a quien queremos servir.
    nodeServed = msg->getSourceNodeId();
    nodeServedGate = msg->getArrivalGate()->getIndex();
    ev << "HandleFileReq: [" << nodeServed << "]-->[" << getId() << "]" << endl;
    // Si ya tenemos almacenada reputacion de este nodo la usamos.
    if(nodeMap.find(nodeServed) != nodeMap.end()){
        tempReputation = nodeMap[nodeServed];
    }
    // Si no, se borra la tempReputation que se tenia, ya que seria de uan vez anterior.
    else{
        tempReputation=PeerReputation();
    }
    // Crea un mensaje ReputationRequest para el nodo que pide.
    ReputationRequest *rrmsg = new ReputationRequest("ReputationRequest");
    rrmsg->setSourceNodeId(getId());    //asigna el origen
    rrmsg->setTargetNodeId(nodeServed); //asigna el objetivo que buscamos
    // Reenvia copias del ReputationRequest a todos menos a quien
    for(int i=0; i<gateSize("dataGate$o"); i++){
        if(msg->getArrivalGate()->getIndex() != i){
            int destinationNodeId = gate("dataGate$o", i)->getNextGate()->getOwnerModule()->getId();
            rrmsg->setDestinationNodeId(destinationNodeId);
            send(rrmsg->dup(),"dataGate$o", i);
        }
    }
    cancelAndDelete(msg);
}

void NoFreeNode::handleFileResponse( File *msg )
{
    // Para caundo me responden con el archivo, si aun no ha vencido el
    // temporizador avisa de que ha recibido y aumenta las peticiones aceptadas.
    if(nodeRequested != NOBODY){
        nodeMap[nodeRequested].acceptedRequest++;
        nodeRequested = NOBODY;
    }
    // Borra el mensaje.
    cancelAndDelete(msg);
    updateDisplay();
}

void NoFreeNode::handleReputationRequest( ReputationRequest *msg )
{
    int targetNode = msg->getTargetNodeId();

    // Si tenemos reputacion de este nodo la enviamos.
    if(nodeMap.find(targetNode) != nodeMap.end()){
        // Crea un mensaje.
        Reputation *rmsg = new Reputation("Reputation");
        rmsg->setTargetNodeId(msg->getTargetNodeId());
        rmsg->setSourceNodeId(getId());
        rmsg->setDestinationNodeId(msg->getSourceNodeId());
        rmsg->setTotalRequests(nodeMap[targetNode].totalRequest);
        rmsg->setAcceptedRequests(nodeMap[targetNode].acceptedRequest);
        // La reenvia por la puerta que llegó.
        send(rmsg,"dataGate$o", msg->getArrivalGate()->getIndex());
    }
    // Y la pedimos por todas las bocas menos por la que llegó:
    for(int i=0; i<gateSize("dataGate$o"); i++){
        if(msg->getArrivalGate()->getIndex() != i){
            int destinationNodeId = gate("dataGate$o", i)->getNextGate()->getOwnerModule()->getId();
            // Si el objetivo es el mismo que el destino no le envio la petición
            if(targetNode != destinationNodeId){
                msg->setDestinationNodeId(destinationNodeId);
                send(msg->dup(),"dataGate$o", i);
            }
        }
    }
    // Borra el mensaje original.
    cancelAndDelete(msg);
    updateDisplay();
}

void NoFreeNode::handleReputationResponse( Reputation *msg )
{
    ev << "targetNode:" << msg->getTargetNodeId() << "   servedNode:" << nodeServed << "   destinationNoe:" << msg->getDestinationNodeId() << "   myNode:" << getId() << endl;
    // Si el mensaje de reputación es del nodo que he preguntado, y me lo mandaban a mi.
    if((msg->getTargetNodeId() == nodeServed) && (msg->getDestinationNodeId() == getId())){
        // Si aún no tengo sumada la opinión de ese nodo me la quedo.
        if(nodeContributed.find(nodeServed) != nodeContributed.end()){
            ev << "suma la opinión del nodo [" << nodeServed << "] a la que ya tengo " << tempReputation;
            int a = msg->getAcceptedRequests();
            int t = msg->getTotalRequests();
            tempReputation = PeerReputation(a, t);
            // A�ado el nodo a la lista de los que han contribuido para no coger mas.
            nodeContributed.insert(msg->getSourceNodeId());
        }
    }
    // Si no era para mi lo reenvío por todas las salidas menos la que llega.
    else{
        for(int i=0; i<gateSize("dataGate$o"); i++){
            if(msg->getArrivalGate()->getIndex() != i){
                int destinationNodeId = gate("dataGate$o", i)->getNextGate()->getOwnerModule()->getId();
                msg->setDestinationNodeId(destinationNodeId);
                send(msg->dup(),"dataGate$o", i);
            }
        }
    }
    // Borro el mensaje original, que para eso se enviaron duplicados.
    cancelAndDelete(msg);
    updateDisplay();
}

void NoFreeNode::reputationRequest( )
{
    //calcular el ratio
    double rate = (double)tempReputation.acceptedRequest / (double)tempReputation.totalRequest;
    //si las peticiones totales dentro de la reputacion temporal que tengo es 0 es que es un nuevo
    bool isNewNode   = (tempReputation.totalRequest == 0)? true : false;
    //miro si el ratio es meyor que el necesario
    bool isGoodRatio = (rate >= requiredShareRate)? true : false;
    //probabilidad de que sea un free rider
    bool isGoodNode  = (uniform(0,1)<kindness)? true: false;
    // Decide si el nodo al que servir es digno de ser servido.
    if((isNewNode || isGoodRatio) && isGoodNode){
        // Estos campos no son necesarios, pero podría implementarse un factory que lo hiciese por mi.
        File *fmsg = new File("File");
        fmsg->setSourceNodeId(getId());
        fmsg->setDestinationNodeId(nodeServed);
        send(fmsg,"dataGate$o", nodeServedGate);
    }
    // Ya se ha decidido si se sirve o no y el nodo queda libre para servir a otra persona.
    nodeServed = NOBODY;
    updateDisplay();
}

void NoFreeNode::finish( )
{
    // TODO
}

void NoFreeNode::updateDisplay( )
{
    //ostringstream buffer;
    //buffer << "nodeRequested: " << nodeRequested;
    //getDisplayString().setTagArg("t", 0, buffer.srt().c_srt());
    //buffer.
    //stringComposer = "nodeServed: ";
    //stringComposer += to_string(nodeServed);
    //getDisplayString().setTagArg("t", 1, stringComposer.c_str());
    //stringComposer = "tempReputation: TOTAL:";
    //stringComposer += to_string(tempReputation.totalRequest);
    //string)+ "  ACEPTADAS:"+ to_str(tempReputation.acceptedRequest);
    //getDisplayString().setTagArg("t", 2, stringComposer.c_str());
    //stringComposer = "";
    //for (auto it=nodeMap.begin(); it!=nodeMap.end(); ++it){
        //stringComposer += "Nodo[" + it->first + "]:{T:" + to_string(tempReputation.totalRequest)
                //+ "  A:"+ to_str(tempReputation.acceptedRequest) + "}\n";
    //}
    //getDisplayString().setTagArg("tt", 0, stringComposer.c_str());
}

/**
 * Operador salida estandar para poder usar el WATCH_MAP()
 * Inspirador por: https://svn-itec.uni-klu.ac.at/trac2/dash/browser/trunk/omnet_simulation/omnet/test/anim/watch/watchtest.cc?rev=143
 */
ostream& operator<<(ostream& os, const PeerReputation& p)
{
    return os << "(" << p.acceptedRequest << "/" << p.totalRequest << ")";
}

/**
 * Operador entrada estandar para poder usar el WATCH_MAP() en modo r/w
 * Inspirador por: https://svn-itec.uni-klu.ac.at/trac2/dash/browser/trunk/omnet_simulation/omnet/test/anim/watch/watchtest.cc?rev=143
 */
istream& operator>>(istream& is, PeerReputation& p)
{
    char dummy;
    return is >> dummy >> p.acceptedRequest >> dummy >> p.totalRequest >> dummy;
}
