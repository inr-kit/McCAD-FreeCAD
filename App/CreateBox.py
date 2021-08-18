#!/usr/bin/env python
# -*- coding: utf-8 -*-

import FreeCAD,Part,FreeCADGui

class CreateBox:
   def CreateBox():
       length = 15
       width = 50
       height = 30
       box = Part.makeBox(length,width,height)
       Part.show(box)

class SelectionObserver:
    def addSelection(self,doc,obj,sub,pos):
        FreeCAD.Console.PrintMessage("Document: %s, Object: %s, Element: %s, Pos: (%d,%d,%d)\n" % (doc,obj,sub, pos[0], pos[1], pos[2]))

    #def setPreselection(self,doc,obj,sub):
        #FreeCAD.Console.PrintMessage("Document: %s, Object: %s, Element: %s\n" % (doc,obj,sub))
