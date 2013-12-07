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
// file: NoFreeNode.h
// author: Daniel I�igo, Efr�n Su�rez
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
 * Clase muy básica para almacenar la reputación
 */
struct PeerReputation {
    int acceptedRequest;    // Peticiones aceptadas.
    int totalRequest;       // Peticiones aceptadas.
    /** Constructor por defecto */
    PeerReputation() {acceptedRequest=0; totalRequest=0;}
    /** Constructor con parámetros */
    PeerReputation(int a, int t) {acceptedRequest=a; totalRequest=t;}
};

class NoFreeNode : public cSimpleModule
{
protected:
    PeerReputation tempReputation;      // Almacena temporalmente la reputación
                                        // de un nodo para decidir si servirle o no.
    double requiredShareRate;          // Ratio necesario de reputación para compartir (p.ej: 0.8);
    int nodeRequested;                 // Nodo al que se ha pedido el archivo.
    int nodeServed;                    // Nodo al que se va a enviar el archivo.
    int nodeServedGate;                // Id de la puerta por la que servir el archivo.
    set <int> nodeContributed;          // Lista de nodos que han aportado su reputación.
    double downloadFileTimeout;        // Tiempo hasta que se realize peticion
    double reputationTimeout;          // Tiempo de validez de la reputación.
    double reputationRequestTimeout;   // Tiempo que se esperan msg tipo R.
    double fileRequestTimeout;         // Tiempo que se espera a que se sirva
                                        // un archivo (después se considera al servidor un freerider)
    double kindness;                   // Probabilidad de enviar el archivo
                                        // <0.8 freerider, >0.8 buen peer.
    cMessage *reputationRequestTimer;   // Timer para esperar mensajes de reputación.
                                        // Funciona con reputationRequestTimeout.
    cMessage *fileRequestTimer;         // Timer para esperar a que me sirvan un archivo.
                                        // Funciona con fileRequestTimeout.
    cMessage *downloadFileTimer;        // Timer para encolar nuevas peticiones de archivo.
    map <int, PeerReputation> nodeMap;// Lista de nodos adyacentes y su reputación.


public:
    /**
     * Constructor por defecto.
     */
    NoFreeNode ();

    /**
     * Cancela y Elimina los selfmessage que se usaban como timer para que no
     * queden por ahí ejecutándose deattached de forma que consuman memoria pero
     * no puedan borrarse (imporante para redes grandes).
     */
    ~NoFreeNode ();

    /**
     * Lee las variables por parámetro que se declaran en la topología o en la
     * definición de la simulación. Instancia los mensajes que serán timers y
     * lanza el primer timer para descargar un archivo que desencadenará el
     * resto de eventos.
     */
    virtual void initialize ();

    /**
     * Recibe un mensaje, mira del tipo que es (RR, FR, R o timer) y
     * delega su procesado a la función correspondiente.
     */
    virtual void handleMessage ( cMessage *msg  );

    /**
     * Maneja los 'selfmessage', que son timers. En función de cuál haya
     * expirado se invoca a una función u otra.
     */
    virtual void handleTimerEvent ( cMessage *msg );

    /**
     * Recibe una petición para servir un archivo que desencadena el proceso
     * de decision. Habrá que mirar si se tiene la reputación del nodo
     *
     *      trabajar con mapas en c++:
     *      http://www.yolinux.com/TUTORIALS/CppStlMultiMap.html
     */
    virtual void handleFileRequest  ( FileRequest *msg );

    /**
     * Recibe el archivo que había pedido así que incrementa 1 el contador de
     * "acceptedRequests".
     */
    virtual void handleFileResponse ( File *msg );

    /**
     * Recibe una petición de reputación para un nodo. Mira si lo tiene, si no
     * lo tiene reenvía el mensaje (actualmente no se usa).
     */
    virtual void handleReputationRequest ( ReputationRequest *msg );

    /**
     * Recibe la reputación que había pedido y decide si servir o no el archivo
     * en función de lo que haya recibido y de un parámetro de "kindness"
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
     * Petición de reputación: al principio se hará empleando el array de nodos
     * directamente y buscando pero luego se usará un algoritmo de flooding.
     */
    virtual void reputationRequest ( );

    /**
     * Graba estadísticas y logs.
     */
    virtual void finish ( );

    /**
     * Actualiza la informacíon que se muestra en cada nodo (porque los watch
     * no funcionan sobre mapas. MUCHAS GRACIAS OMNET!!!
     */
    virtual void updateDisplay ( );
};

#endif
