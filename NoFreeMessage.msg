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
// file: NoFreeMessage.msg
// author: Daniel Iñigo
//

// Enum con el tipo de mensajes que se van a utilizar para que sea más fácil
// luego distinguirlos
enum MessageType
{
    FILE_REQUEST = 1;
    REPUTATION_REQUEST = 2;
    FILE = 3;
    REPUTATION = 4;
}

// Paquete básico al que hacer casting para sacar el tipo.
packet NoFreeMessage
{
    unsigned int messageTipe @enum(MessageType);
}

// Petición de archivo.
packet FileRequest extends NoFreeMessage
{
    messageTipe @enum(MessageType) = FILE_REQUEST;
    int sourceNodeId;
    int destinationNodeId;
}

// Petición de reputación.
packet ReputationRequest extends NoFreeMessage
{
    messageTipe @enum(MessageType) = REPUTATION_REQUEST;
    int sourceNodeId;       // Quien pregunta.
    int destinationNodeId;  // A quién pregunta.
    int targetNodeId;       // Por qué nodo pregunta.
}

// Paquete que reprsenta la transferencia de un archivo entre dos nodos.
packet File extends NoFreeMessage
{
    messageTipe @enum(MessageType) = FILE;
    int sourceNodeId;       // Quién envía el archivo.
    int destinationNodeId;  // A quién se lo manda.
}

// Datos de la reputación de un nodo.
packet Reputation extends NoFreeMessage
{
    messageTipe @enum(MessageType) = REPUTATION;
    int targetNodeId;       // De quién son los datos de reputación.
    int totalRequests;
    int acceptedRequests;
    double lastUpdate;      // la fecha de ultima actualización en double
                             // se puede convertir con `SIMTIME_DBL(simtime_t)` 
}