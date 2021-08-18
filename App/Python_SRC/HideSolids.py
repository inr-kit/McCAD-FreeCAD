#!/usr/bin/env python
# -*- coding: utf-8 -*-
import FreeCAD as App
import FreeCADGui as Gui
import Part

def hide():

    select_list = Gui.Selection.getSelectionEx()
    if (len(select_list) == 0):
        return
    print "step1"
    for obj in App.ActiveDocument.Objects:
        if not obj.isDerivedFrom("Part::Feature"):
            continue
        bfind = False
        for sel in select_list:
            select_obj = sel.Object
            if (obj.Name == select_obj.Name):
                bfind = True
                break

        print bfind
        if(bfind):
            Gui.ActiveDocument.getObject(obj.Name).Visibility = True
        else:
            Gui.ActiveDocument.getObject(obj.Name).Visibility = False
