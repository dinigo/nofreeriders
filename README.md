PROPÓSITO
=========
Simulación de un protocolo de reputación que defina una arquitectura de red.
El protocolo se emplea para decidir si se sirven archivos o no a un nodo de una red P2P pura (todos los nodos son iguales, sin trackers ni supernodos).
La hay dos tipos de plano/conexión/grafo sobre los mismo nodos:
* Conexión de control: grafo completo entre los nodos. Destinado a alimentar el algoritmo de decisión basado en reputación.
* Conexión de datos: grafo conexo entre los nodos. Destinado a hacer peticiones.

CONSIDERACIONES
===============
Dado que la simulación modela un algoritmo de reputación que se ejecuta sobre un protocolo P2P de búsqueda se ovbia este, se supone que los nodos "conocen" a quién pedir el archivo y cómo encaminarlo. Por ello en las conexiones de datos o se envía un paquete, el de aceptada, o ninguno.


HERRAMIENTAS
============
Se emplea el framework de simulación OmNet++ sin ningún plugin (librerías o imports externos al proyecto), ya que el uso de alguno escala rápidamente la complejidad de la simulación  sin ninguna ventaja para esta.
