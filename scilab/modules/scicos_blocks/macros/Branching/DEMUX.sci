//  Scicos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
//
// See the file ../license.txt
//

function [x,y,typ]=DEMUX(job,arg1,arg2)
    x=[];
    y=[];
    typ=[];
    select job
    case "set" then
        x=arg1;
        graphics=arg1.graphics;
        exprs=graphics.exprs
        model=arg1.model;
        while %t do
            [ok,out,exprs]=scicos_getvalue(..
            msprintf(_("Set %s block parameters"), "DEMUX"),..
            [_("Number of output ports or vector of sizes")],list("intvec",-1),exprs)
            if ~ok then
                break,
            end
            if size(out,"*")==1 then
                if out<2|out>31 then
                    message(_("The block must have from 2 to 31 output ports"))
                    ok=%f
                else
                    [model,graphics,ok]=check_io(model,graphics,0,-[1:out]',[],[])
                end
            else
                if size(out,"*")<2| or(out==0)|size(out,"*")>31 then
                    msg = _("The block must have from 2 to 31 output ports.<br>size 0 is not allowed")
                    message(msg)
                    ok=%f
                else
                    if min(out)<0 then
                        nin=0,
                    else
                        nin=sum(out),
                    end
                    [model,graphics,ok]=check_io(model,graphics,nin,out(:),[],[])
                    if ok then
                        out=size(out,"*"),
                    end
                end
            end
            if ok then
                graphics.exprs=exprs;
                model.ipar=out
                x.graphics=graphics;
                x.model=model
                break
            end
        end
    case "define" then
        out=2
        model=scicos_model()
        model.sim=list("multiplex",4)
        model.in=0 //means equal to the sum of the outputs
        model.out=-[1:out]'
        model.ipar=out
        model.blocktype="c"
        model.firing=[]
        model.dep_ut=[%t %f]

        exprs=string(out)
        gr_i=[]
        x=standard_define([.5 2],model,exprs,gr_i)
    end
endfunction
