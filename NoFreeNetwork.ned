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
        double requiredShareRate = default(0.8);
        double reputationRequestTimeout @unit(s) = default(10s);
        double fileRequestTimeout @unit(s) = default(0.5s);
        double downloadFileTimeout @unit(s) = default(0.1s);
        double kindness = default(uniform(0.5,0.9));
        @display("i=old/comp;is=n");
    gates:
        inout dataGate[];     // Para FileRequest y File
}

// ------------------ CHANNELS ------------------//
channel DataChannel extends ned.DatarateChannel
{
    datarate = 5Mbps;
    delay = 1ms;
    @display("ls=blue,2,s");
}
