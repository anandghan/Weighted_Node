// This is the Network which implements the object 'Node'
//
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

package net;

//
// TODO auto-generated type
//
network net
{
    @display("bgb=671,250");
    submodules:
        node: Node {
            @display("p=220,59");
            address = 0;
        }
        node1: Node {
            @display("p=375,59");
            address = 1;
        }
        node2: Node {
            @display("p=493,128");
            address = 2;
        }
        node3: Node {
            @display("p=381,192");
            address = 3;
        }
        node4: Node {
            @display("p=220,192");
            address = 4;
        }

        node5: Node {
            @display("p=116,128");
            address = 5;
        }
    connections:
        node.out++ --> node1.in++;
        node1.out++ --> node.in++;
        node1.out++ --> node2.in++;
        node2.out++ --> node1.in++;
        node4.out++ --> node5.in++;
        node5.out++ --> node4.in++;
        node5.out++ --> node.in++;
        node.out++ --> node5.in++;
        node1.out++ --> node4.in++;
        node4.out++ --> node1.in++;
        node3.out++ --> node2.in++;
        node2.out++ --> node3.in++;
        node4.out++ --> node3.in++;
        node3.out++ --> node4.in++;
        node.out++ --> node3.in++;
        node3.out++ --> node.in++;
}
