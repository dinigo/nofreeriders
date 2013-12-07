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
// Generated network with random topology (50 nodes, 60 edges, seed=100).
//
network BigNetwork {
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
        node10 : NoFreeNode;
        node11 : NoFreeNode;
        node12 : NoFreeNode;
        node13 : NoFreeNode;
        node14 : NoFreeNode;
        node15 : NoFreeNode;
        node16 : NoFreeNode;
        node17 : NoFreeNode;
        node18 : NoFreeNode;
        node19 : NoFreeNode;
        node20 : NoFreeNode;
        node21 : NoFreeNode;
        node22 : NoFreeNode;
        node23 : NoFreeNode;
        node24 : NoFreeNode;
        node25 : NoFreeNode;
        node26 : NoFreeNode;
        node27 : NoFreeNode;
        node28 : NoFreeNode;
        node29 : NoFreeNode;
        node30 : NoFreeNode;
        node31 : NoFreeNode;
        node32 : NoFreeNode;
        node33 : NoFreeNode;
        node34 : NoFreeNode;
        node35 : NoFreeNode;
        node36 : NoFreeNode;
        node37 : NoFreeNode;
        node38 : NoFreeNode;
        node39 : NoFreeNode;
        node40 : NoFreeNode;
        node41 : NoFreeNode;
        node42 : NoFreeNode;
        node43 : NoFreeNode;
        node44 : NoFreeNode;
        node45 : NoFreeNode;
        node46 : NoFreeNode;
        node47 : NoFreeNode;
        node48 : NoFreeNode;
        node49 : NoFreeNode;
    connections:
        node0.dataGate++ <--> DataChannel <--> node1.dataGate++;
        node0.dataGate++ <--> DataChannel <--> node21.dataGate++;
        node0.dataGate++ <--> DataChannel <--> node6.dataGate++;
        node0.dataGate++ <--> DataChannel <--> node22.dataGate++;
        node0.dataGate++ <--> DataChannel <--> node11.dataGate++;
        node1.dataGate++ <--> DataChannel <--> node16.dataGate++;
        node1.dataGate++ <--> DataChannel <--> node19.dataGate++;
        node1.dataGate++ <--> DataChannel <--> node2.dataGate++;
        node1.dataGate++ <--> DataChannel <--> node3.dataGate++;
        node1.dataGate++ <--> DataChannel <--> node5.dataGate++;
        node2.dataGate++ <--> DataChannel <--> node4.dataGate++;
        node2.dataGate++ <--> DataChannel <--> node8.dataGate++;
        node2.dataGate++ <--> DataChannel <--> node41.dataGate++;
        node3.dataGate++ <--> DataChannel <--> node7.dataGate++;
        node3.dataGate++ <--> DataChannel <--> node10.dataGate++;
        node4.dataGate++ <--> DataChannel <--> node24.dataGate++;
        node4.dataGate++ <--> DataChannel <--> node27.dataGate++;
        node4.dataGate++ <--> DataChannel <--> node46.dataGate++;
        node5.dataGate++ <--> DataChannel <--> node33.dataGate++;
        node5.dataGate++ <--> DataChannel <--> node12.dataGate++;
        node5.dataGate++ <--> DataChannel <--> node14.dataGate++;
        node6.dataGate++ <--> DataChannel <--> node18.dataGate++;
        node6.dataGate++ <--> DataChannel <--> node41.dataGate++;
        node6.dataGate++ <--> DataChannel <--> node13.dataGate++;
        node7.dataGate++ <--> DataChannel <--> node34.dataGate++;
        node7.dataGate++ <--> DataChannel <--> node17.dataGate++;
        node7.dataGate++ <--> DataChannel <--> node9.dataGate++;
        node7.dataGate++ <--> DataChannel <--> node26.dataGate++;
        node8.dataGate++ <--> DataChannel <--> node9.dataGate++;
        node8.dataGate++ <--> DataChannel <--> node28.dataGate++;
        node9.dataGate++ <--> DataChannel <--> node35.dataGate++;
        node9.dataGate++ <--> DataChannel <--> node37.dataGate++;
        node10.dataGate++ <--> DataChannel <--> node31.dataGate++;
        node12.dataGate++ <--> DataChannel <--> node15.dataGate++;
        node14.dataGate++ <--> DataChannel <--> node49.dataGate++;
        node14.dataGate++ <--> DataChannel <--> node38.dataGate++;
        node17.dataGate++ <--> DataChannel <--> node42.dataGate++;
        node17.dataGate++ <--> DataChannel <--> node25.dataGate++;
        node18.dataGate++ <--> DataChannel <--> node45.dataGate++;
        node19.dataGate++ <--> DataChannel <--> node32.dataGate++;
        node19.dataGate++ <--> DataChannel <--> node20.dataGate++;
        node19.dataGate++ <--> DataChannel <--> node44.dataGate++;
        node22.dataGate++ <--> DataChannel <--> node23.dataGate++;
        node23.dataGate++ <--> DataChannel <--> node32.dataGate++;
        node23.dataGate++ <--> DataChannel <--> node47.dataGate++;
        node24.dataGate++ <--> DataChannel <--> node35.dataGate++;
        node24.dataGate++ <--> DataChannel <--> node46.dataGate++;
        node26.dataGate++ <--> DataChannel <--> node36.dataGate++;
        node26.dataGate++ <--> DataChannel <--> node31.dataGate++;
        node27.dataGate++ <--> DataChannel <--> node29.dataGate++;
        node28.dataGate++ <--> DataChannel <--> node30.dataGate++;
        node29.dataGate++ <--> DataChannel <--> node40.dataGate++;
        node30.dataGate++ <--> DataChannel <--> node42.dataGate++;
        node31.dataGate++ <--> DataChannel <--> node45.dataGate++;
        node33.dataGate++ <--> DataChannel <--> node49.dataGate++;
        node34.dataGate++ <--> DataChannel <--> node43.dataGate++;
        node35.dataGate++ <--> DataChannel <--> node39.dataGate++;
        node37.dataGate++ <--> DataChannel <--> node46.dataGate++;
        node39.dataGate++ <--> DataChannel <--> node44.dataGate++;
        node47.dataGate++ <--> DataChannel <--> node48.dataGate++;
}
