//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

package nofreeriders;
import nofreeriders.NoFreeNode;

import nofreeriders.DataChannel;

//
// Generated network with random topology (10 nodes, 20 edges, seed=100).
//
network StaticNetwork {
    submodules:
        node0 : NoFreeNode;
        node1 : NoFreeNode;
        node2 : NoFreeNode;
        node3 : NoFreeNode;
        node4 : NoFreeNode;
        node5 : NoFreeNode;
        node6 : NoFreeNode;
        node7 : NoFreeNode;
        node8 : NoFreeNode;
        node9 : NoFreeNode;
    connections:
        node0.dataGate++ <--> DataChannel <--> node1.dataGate++;
        node0.dataGate++ <--> DataChannel <--> node2.dataGate++;
        node0.dataGate++ <--> DataChannel <--> node3.dataGate++;
        node0.dataGate++ <--> DataChannel <--> node4.dataGate++;
        node0.dataGate++ <--> DataChannel <--> node5.dataGate++;
        node0.dataGate++ <--> DataChannel <--> node6.dataGate++;
        node1.dataGate++ <--> DataChannel <--> node2.dataGate++;
        node1.dataGate++ <--> DataChannel <--> node3.dataGate++;
        node1.dataGate++ <--> DataChannel <--> node5.dataGate++;
        node2.dataGate++ <--> DataChannel <--> node4.dataGate++;
        node2.dataGate++ <--> DataChannel <--> node7.dataGate++;
        node2.dataGate++ <--> DataChannel <--> node8.dataGate++;
        node3.dataGate++ <--> DataChannel <--> node5.dataGate++;
        node3.dataGate++ <--> DataChannel <--> node6.dataGate++;
        node3.dataGate++ <--> DataChannel <--> node7.dataGate++;
        node4.dataGate++ <--> DataChannel <--> node7.dataGate++;
        node4.dataGate++ <--> DataChannel <--> node8.dataGate++;
        node5.dataGate++ <--> DataChannel <--> node6.dataGate++;
        node5.dataGate++ <--> DataChannel <--> node9.dataGate++;
        node7.dataGate++ <--> DataChannel <--> node9.dataGate++;
}
