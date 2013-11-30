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
// author: Daniel Iñigo
//

#include <stdio.h>
#include <string.h>
#include <omnetpp.h>

#include "NoFreeNode.h"
#include "NoFreeMessage_m.h"

// Declara el módulo para que pueda usarse en el archivo de topología
Define_Module(NoFree);



NoFreeNode::NoFreeNode()
{

}

NoFreeNode::~NoFreeNode()
{
    cancelAndDelete(reputationRequestTimer);
    cancelAndDelete(fileRequestTimer);
}

NoFreeNode::initialize()
{
    // Lee los valores de las variables desde el archivo de topología.
    reputationTimeout = par("reputationTiemout");
    reputationRequestTimeout = par("reputationRequestTimeout");
    fileRequestTimeout = par("fileRequestTimeout");
    kindness = par("kindness");
    // Instancia los timer con un mensaje descriptivo.
    reputationRequestTimer = new cMessage("reputationRequestTiemout");
    fileRequestTimer = new cMessage("fileRequestTiemout");
}

NoFreeNode::fileRequest()
{
    // Lee el tiempo hasta la próxima petición por parámetro para que se llame
    // a la función aleatoria de cada vez. De esta forma cada vez que se invoca
    // `timeTillFileRequest` tiene un valor diferente.
    timeTillFileRequest = &par("timeTillFileRequest");
    scheduleAt(simTime()+timeTillFileRequest, msg);
}

NoFreeNode::handleTimerEvent( cMessage *msg )
{
    // Es hora de descargarse un archivo de alguien.
    if(msg == fileRequestTimer){
        // Elijo a la persona de entre los conectados a mi
        int n = gateSize("gate$o");
        int k = intuniform(0,n-1);
        // Le envío una petición
        FileRequest *frmsg = new FileRequest("fileRequest");
        frmsg->setSourceNodeId();
        frmsg->setDestinationNodeId();
    }
}
