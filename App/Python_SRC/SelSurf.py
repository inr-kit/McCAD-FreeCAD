#!/usr/bin/env python
# -*- coding: utf-8 -*-

import FreeCAD,Part,FreeCADGui
import Part,OCC

from OCC import TopoDS
from OCC import GeomAbs
from OCC.GeomAdaptor import GeomAdaptor_Surface
from OCC.TopoDS import topods,TopoDS_Shape,topods_Face as _TopoDS_face
from OCC.BRep import BRep_Tool_Surface
from OCC.BRepAdaptor import BRepAdaptor_Surface
from OCC.GeomAbs import GeomAbs_Cylinder,GeomAbs_Plane,GeomAbs_Cone,GeomAbs_Sphere,GeomAbs_Torus,GeomAbs_SurfaceOfRevolution
from OCC.Standard import Standard_Float

class SelectionObserver:
    def addSelection(self,doc,obj,sub,pos):
        sel = FreeCADGui.Selection.getSelectionEx()
        if len(sel) == 0: return

    def clearSelection(self,doc):
        sel = FreeCADGui.Selection.getSelectionEx()
        if len(sel) != 0: return

        objlist = FreeCAD.ActiveDocument.findObjects("App::Annotation","SurfInfo")
        if (objlist):
            objlist[0].LabelText = ""
        #FreeCAD.Console.PrintMessage("X")
        #FreeCAD.ActiveDocument.removeObject("SurfInfo")
        #FreeCADGui.Selection.removeObserver(obs)

    def getSurfType(self,surf_type):
        switcher = {
            GeomAbs_Plane: "Plane",
            GeomAbs_Cylinder: "Cylinder",
            GeomAbs_Cone: "Cone",
            GeomAbs_Sphere: "Sphere",
            GeomAbs_Torus: "Torus",
            GeomAbs_SurfaceOfRevolution: "Torus",
        }
        return switcher.get(surf_type, "SplineSurf")

    def getSurfEqua(self,surf_Info,adp):
        if (surf_Info == "Plane"):
            plane = adp.Plane()
            a,b,c,d  = plane.Coefficients()
            surf_Info += ' Dir:{0:.2f},{1:.2f},{2:.2f} Dis:{3:.4f}'.format(a,b,c,d)
        elif(surf_Info == "Cylinder"):
            cylinder = adp.Cylinder()
            pos = cylinder.Position()
            r  = cylinder.Radius()
            d = pos.Direction()
            surf_Info += ' Axis {0:.2f},{1:.2f},{2:.2f}  Radius:{3:.4f}'.format(d.X(),d.Y(),d.Z(),r)
        elif(surf_Info == "Cone"):
            cone = adp.Cone()
            top = cone.Apex()
            ang = cone.SemiAngle()
            aix = cone.Axis().Direction()
            surf_Info += ' Axis {0:.2f},{1:.2f},{2:.2f} Apex:{3:.2f},{3:.2f},{3:.2f} SemiAngle:{6:.2f}'\
                         .format(top.X(),top.Y(),top.Z(),aix.X(),aix.Y(),aix.Z(),ang)
        elif(surf_Info == "Sphere"):
            sphere = adp.Sphere()
            cen = sphere.Location()
            r = sphere.Radius()
            surf_Info += ' Centre {0:.2f},{1:.2f},{2:.2f} Radius:{3:.2f}'\
                          .format(cen.X(),cen.Y(),cen.Z(),r)
        elif(surf_Info == "Torus"):
            torus = adp.Torus()
            pos = torus.Position().Direction()
            minr = torus.MinorRadius()
            majr = torus.MajorRadius()
            surf_Info += ' Centre {0:.2f},{1:.2f},{2:.2f} MinorRadius:{3:.2f} MajorRadius:{4:.2f}'\
                          .format(pos.X(),pos.Y(),pos.Z(),minr,majr)
        return surf_Info

    def setPreselection(self,doc,obj,sub):

        sel = FreeCADGui.Selection.getSelectionEx()
        v = FreeCADGui.ActiveDocument.ActiveView
        d = FreeCADGui.ActiveDocument

        so = FreeCAD.ActiveDocument.getObject(obj)
        if not hasattr(so,"Shape"): return

        faces = so.Shape.Faces
        typ = sub[0:4]

        if not typ=="Face": return

        idx = int(sub[4:])
        face = faces[idx-1]

        txt = ""

        occface = Part.__toPythonOCC__(face)
        #surf =  occface.Surface()

        ts = _TopoDS_face(occface)

        face_adaptor = BRepAdaptor_Surface(ts)
        face_type = face_adaptor.GetType()
        surf_txt = self.getSurfType(face_type)

        txt = self.getSurfEqua(surf_txt,face_adaptor)

        #t = GeomAdaptor_Surface(TopoDS_Shape(occface)).GetType()
        #FreeCAD.Console.PrintMessage("%s\n" % (str(t)))
        #if (str(face.Surface) == "<Plane object>"):
            #txt = "Plane"
        #elif (str(face.Surface) == "<Cylinder object>"):
            #txt = "Cylinder"
        #elif (str(face.Surface) == "<Toriod object>"):
            #txt = "Torus"

        pnt = v.getPoint(v.getCursorPos())
        #FreeCAD.Console.PrintMessage("X %f Y %f Z %f\n" % (pnt.x, pnt.y, pnt.z))
        adoc = FreeCAD.ActiveDocument
        objlist = adoc.findObjects("App::Annotation","SurfInfo")

        if (objlist):
            objlist[0].LabelText = txt
            objlist[0].Position=(pnt.x,pnt.y,pnt.z)

    #def removePreselection(self,doc,obj,sub):
        #objlist = FreeCAD.ActiveDocument.findObjects("App::Annotation","SurfInfo")
        #if (objlist):
            #objlist[0].LabelText = ""

class ViewObserver:
    def logPosition(self, info):
        up = (info["State"] == "DOWN")
        bn = (info["Button"] == "BUTTON1")
        if (up and bn):
            objlist = FreeCAD.ActiveDocument.findObjects("App::Annotation","SurfInfo")
            if (objlist):
                FreeCAD.ActiveDocument.removeObject("SurfInfo")
                #objlist[0].LabelText = ""
                FreeCADGui.Selection.removeObserver(obs)
                #FreeCAD.ActiveDocument.removeObject("SurfInfo")
"""
        down = (info["State"] == "UP")
        bn2 = (info["Button"] == "BUTTON2")
        if (down and bn2):
            FreeCAD.Console.PrintMessage("STEP 1")
            objlist = FreeCAD.ActiveDocument.findObjects("App::Annotation","SurfInfo")
            if (objlist):
                FreeCAD.ActiveDocument.removeObject("SurfInfo")
                FreeCADGui.Selection.removeObserver(obs)
"""

def SelectSurf(self):

    obs = SelectionObserver()
    FreeCADGui.Selection.addObserver(obs)

    v = FreeCADGui.activeDocument().activeView()
    o = ViewObserver()
    #c = v.addEventCallback("SoMouseButtonEvent",o.logPosition)

    FreeCAD.ActiveDocument.addObject("App::Annotation","SurfInfo");
    txtobj=FreeCAD.ActiveDocument.getObject("SurfInfo")
    txtobj.ViewObject.TextColor = (1.0,0.0,0.0,0.5)
    txtobj.ViewObject.FontSize = 20

#def setPreselection(self,doc,obj,sub):
        #FreeCAD.Console.PrintMessage("Document: %s, Object: %s, Element: %s\n" % (doc,obj,sub))
