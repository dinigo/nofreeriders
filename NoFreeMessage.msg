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
    FILE_RESPONSE = 3;
    REPUTATION_RESPONSE = 4;
}

// Paquete básico al que hacer casting para sacar el tipo.
packet NoFreeMessage
{
    unsigned int messageTipe @enum(MessageType);    // Tipo de paquete.
    int sourceNodeId;           // Índice del nodo que envía el paquete.
    int destinationNodeId;      // Índice del nodo a quien se envía el paquete.
    int ttl=2;                  // Para que no se propaquen infinitamente.
}

// Petición de archivo.
packet FileRequest extends NoFreeMessage
{
    string displayString = "b=15,15,rect,green";
    messageTipe @enum(MessageType) = FILE_REQUEST;
}

// Petición de reputación.
packet ReputationRequest extends NoFreeMessage
{
    string displayString = "b=15,15,rect,yellow";
    messageTipe @enum(MessageType) = REPUTATION_REQUEST;
    int targetNodeId;       // Por qué nodo pregunta.
}

// Paquete que reprsenta la transferencia de un archivo entre dos nodos.
packet File extends NoFreeMessage
{
    string displayString = "b=15,15,rect,blue";
    messageTipe @enum(MessageType) = FILE_RESPONSE;
}

// Datos de la reputación de un nodo.
packet Reputation extends NoFreeMessage
{
    string displayString = "b=15,15,rect,blue";
    messageTipe @enum(MessageType) = REPUTATION_RESPONSE;
    int targetNodeId;       // De quién son los datos de reputación.
    int totalRequests;
    int acceptedRequests;
}
