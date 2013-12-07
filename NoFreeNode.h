//
// Copyright (c) 2013 Daniel I帽igo, Efren Suarez, Yuriy Batrakov, Jos茅 Sklatz
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
// file: NoFreeNode.h
// author: Daniel I駃go, Efr閚 Su醨ez
//

#ifndef __NOFREENODE_H_
#define __NOFREENODE_H_

#include <stdio.h>
#include <string.h>
#include <map>
#include <set>
#include <omnetpp.h>
#include "NoFreeMessage_m.h"
using namespace std;

/**
 * Clase muy b谩sica para almacenar la reputaci贸n
 */
struct PeerReputation {
    int acceptedRequest;    // Peticiones aceptadas.
    int totalRequest;       // Peticiones aceptadas.
    /** Constructor por defecto */
    PeerReputation() {acceptedRequest=0; totalRequest=0;}
    /** Constructor con par谩metros */
    PeerReputation(int a, int t) {acceptedRequest=a; totalRequest=t;}
};

class NoFreeNode : public cSimpleModule
{
protected:
    PeerReputation tempReputation;      // Almacena temporalmente la reputaci贸n
                                        // de un nodo para decidir si servirle o no.
    double requiredShareRate;          // Ratio necesario de reputaci贸n para compartir (p.ej: 0.8);
    int nodeRequested;                 // Nodo al que se ha pedido el archivo.
    int nodeServed;                    // Nodo al que se va a enviar el archivo.
    int nodeServedGate;                // Id de la puerta por la que servir el archivo.
    set <int> nodeContributed;          // Lista de nodos que han aportado su reputaci贸n.
    double downloadFileTimeout;        // Tiempo hasta que se realize peticion
    double reputationTimeout;          // Tiempo de validez de la reputaci贸n.
    double reputationRequestTimeout;   // Tiempo que se esperan msg tipo R.
    double fileRequestTimeout;         // Tiempo que se espera a que se sirva
                                        // un archivo (despu茅s se considera al servidor un freerider)
    double kindness;                   // Probabilidad de enviar el archivo
                                        // <0.8 freerider, >0.8 buen peer.
    cMessage *reputationRequestTimer;   // Timer para esperar mensajes de reputaci贸n.
                                        // Funciona con reputationRequestTimeout.
    cMessage *fileRequestTimer;         // Timer para esperar a que me sirvan un archivo.
                                        // Funciona con fileRequestTimeout.
    cMessage *downloadFileTimer;        // Timer para encolar nuevas peticiones de archivo.
    map <int, PeerReputation> nodeMap;// Lista de nodos adyacentes y su reputaci贸n.


public:
    /**
     * Constructor por defecto.
     */
    NoFreeNode ();

    /**
     * Cancela y Elimina los selfmessage que se usaban como timer para que no
     * queden por ah铆 ejecut谩ndose deattached de forma que consuman memoria pero
     * no puedan borrarse (imporante para redes grandes).
     */
    ~NoFreeNode ();

    /**
     * Lee las variables por par谩metro que se declaran en la topolog铆a o en la
     * definici贸n de la simulaci贸n. Instancia los mensajes que ser谩n timers y
     * lanza el primer timer para descargar un archivo que desencadenar谩 el
     * resto de eventos.
     */
    virtual void initialize ();

    /**
     * Recibe un mensaje, mira del tipo que es (RR, FR, R o timer) y
     * delega su procesado a la funci贸n correspondiente.
     */
    virtual void handleMessage ( cMessage *msg  );

    /**
     * Maneja los 'selfmessage', que son timers. En funci贸n de cu谩l haya
     * expirado se invoca a una funci贸n u otra.
     */
    virtual void handleTimerEvent ( cMessage *msg );

    /**
     * Recibe una petici贸n para servir un archivo que desencadena el proceso
     * de decision. Habr谩 que mirar si se tiene la reputaci贸n del nodo
     *
     *      trabajar con mapas en c++:
     *      http://www.yolinux.com/TUTORIALS/CppStlMultiMap.html
     */
    virtual void handleFileRequest  ( FileRequest *msg );

    /**
     * Recibe el archivo que hab铆a pedido as铆 que incrementa 1 el contador de
     * "acceptedRequests".
     */
    virtual void handleFileResponse ( File *msg );

    /**
     * Recibe una petici贸n de reputaci贸n para un nodo. Mira si lo tiene, si no
     * lo tiene reenv铆a el mensaje (actualmente no se usa).
     */
    virtual void handleReputationRequest ( ReputationRequest *msg );

    /**
     * Recibe la reputaci贸n que hab铆a pedido y decide si servir o no el archivo
     * en funci贸n de lo que haya recibido y de un par谩metro de "kindness"
     * decide enviar el archivo o no.
     */
    virtual void handleReputationResponse ( Reputation *msg );

    /**
     * Transcurrido un tiempo aleatorio pide un archivo a otro nodo.
     * Aumenta en uno el contador de peticiones totales a ese nodo.
     * Se elije un nodo aleatoriamente entre los nodos conectados (recorrer
     * el array de puertas y elegir uno).
     */
    virtual void fileRequest ( );

    /**
     * Petici贸n de reputaci贸n: al principio se har谩 empleando el array de nodos
     * directamente y buscando pero luego se usar谩 un algoritmo de flooding.
     */
    virtual void reputationRequest ( );

    /**
     * Graba estad铆sticas y logs.
     */
    virtual void finish ( );

    /**
     * Actualiza la informac铆on que se muestra en cada nodo (porque los watch
     * no funcionan sobre mapas. MUCHAS GRACIAS OMNET!!!
     */
    virtual void updateDisplay ( );
};

#endif
