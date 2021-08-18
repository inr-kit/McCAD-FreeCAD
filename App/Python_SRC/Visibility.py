#!/usr/bin/env python
# -*- coding: utf-8 -*-
import FreeCAD as App
import FreeCADGui as Gui
import Part

def showSelection():

    select_list = Gui.Selection.getSelectionEx()
    if (len(select_list) == 0):
        return

    for obj in App.ActiveDocument.Objects:
        if not obj.isDerivedFrom("Part::Feature"):
            continue
        if obj.isDerivedFrom("Part::Line"):
            continue
        bfind = False
        for sel in select_list:
            select_obj = sel.Object
            print obj.Name
            print select_obj.Name
            if (obj.Name == select_obj.Name):
                bfind = True
                break

        if(bfind):
            Gui.ActiveDocument.getObject(obj.Name).Visibility = True
        else:
            Gui.ActiveDocument.getObject(obj.Name).Visibility = False

def showAll():

    for obj in App.ActiveDocument.Objects:
        if not obj.isDerivedFrom("Part::Feature"):
            continue
        Gui.ActiveDocument.getObject(obj.Name).Visibility = True
