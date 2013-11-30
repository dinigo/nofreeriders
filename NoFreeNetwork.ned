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
// file: NoFreeNode.ned
// author: Daniel Iñigo, Efrén Suarez
//

package nofreeriders;

// ------------------ MODULES ------------------//
simple NoFreeNode
{
    parameters:
        double reputationTimeout = default(30.0);
        double reputationRequestTimeout = default(1);
        double fileRequestTimeout = default(0.5);
        double downloadFileTiemout = default(0.1);
        double kindness = default(uniform(0.5,0.9));
        double timeTillFileRequest = default(uniform(1,50));

        @display("i=old/bwcomp;is=n");
    gates:
        inout dataGate[];     // Para FileRequest y File
        inout controlGate[];  // Para ReputationRequest y Reputation
}

// ------------------ CHANNELS ------------------//
channel DataChannel extends ned.DatarateChannel
{
    datarate = 5Mbps;
    delay = 100us;
    @display("ls=blue,2,da");
}

channel ControlChannel
{
    // TODO: añadir delay para simular la lectura de bases de datos procesos.
    @display("ls=green,2,da");
}


// ------------------ NETWORKS ------------------//
network MyNetwork
{
    parameters:
        int numNodes = default(10);         // Numero de nodos tipo NoFreeNode
        int connectivity = default(0.5);    // probabilidad de conectar con otro
        @display("i=block/network2");

    submodules:
        noFreeNode[numNodes]: NoFreeNode;
    connections:
        // Hace las conexiones de datos aleatoriamente
        for i=1..numNodes-1, for j=0..i, if (uniform(0,1)>connectivity) {
            noFreeNode[i].dataGate++ <--> DataChannel <--> noFreeNode[j].dataGate++;
        }
        // Hace un grafo completo para las conexiones de control.
        for i=1..numNodes-1, for j=0..i {
            noFreeNode[i].controlGate++ <--> ControlChannel <--> noFreeNode[j].controlGate++;
        }
}
