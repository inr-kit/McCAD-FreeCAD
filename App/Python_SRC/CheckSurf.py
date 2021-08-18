import FreeCAD as App
import FreeCADGui as Gui
import Part
from PySide import QtGui, QtCore
import sys
import Paths

from OCC import TopoDS
from OCC import GeomAbs
from OCC.GeomAdaptor import GeomAdaptor_Surface
from OCC.TopoDS import topods,TopoDS_Shape,topods_Face as _TopoDS_face
from OCC.BRep import BRep_Tool_Surface
from OCC.BRepAdaptor import BRepAdaptor_Surface
from OCC.GeomAbs import GeomAbs_Cylinder,GeomAbs_Plane,GeomAbs_Cone,GeomAbs_Sphere,GeomAbs_Torus,GeomAbs_SurfaceOfRevolution



class CreateFace(object):

    __surfType = ''
    __faceObj = None
    __num = 0
    __obj = None

    def __init__(self,face,n,obj):
        self.__faceObj = face
        self.__num = n
        self.__obj = obj
        self.identifySurf()

    def getSurfType(self):
        return self.__surfType    

    def identifySurf(self):

        occface = Part.__toPythonOCC__(self.__faceObj)
        ts = _TopoDS_face(occface)
        face_adaptor = BRepAdaptor_Surface(ts)
        face_type = face_adaptor.GetType()        

        if (face_type == GeomAbs_Plane):
            self.__surfType = "Plane"
        elif(face_type == GeomAbs_Cylinder):
            self.__surfType = "Cylinder"
        elif(face_type == GeomAbs_Sphere):
            self.__surfType = "Sphere"
        elif(face_type == GeomAbs_Cone):
            self.__surfType = "Cone"
        elif(face_type == GeomAbs_Torus):
            self.__surfType = "Toroid"
        elif(face_type == GeomAbs_SurfaceOfRevolution):
            self.__surfType = "Revolution"
        else:
            self.__surfType = "Spline"

    def getObject(self):
        #obj = App.ActiveDocument.getObject(self.__faceObj.Name)
        return self.__obj    

    def getSurfNum(self):
        facenum = "Face" + str(self.__num)
        return facenum


class CreateSolid(object):
    __face_list = []
    __obj = None

    def __init__(self,obj):
        self.__obj = obj
        self.__plane = False
        self.__cylinder = False
        self.__sphere = False
        self.__cone = False
        self.__toroid = False
        self.__revsurf = False
        self.__spline = False

    def addFace(self,face):
        self.__face_list.append(face)

    def getFace(self):
        return self.__face_list

    def getObject(self):
        return self.__obj

    def get_plane(self):
        return self.__plane

    def set_plane(self,hasPlane):
        self.__plane = hasPlane

    plane = property(get_plane,set_plane)

    def get_plane(self):
        return self.__plane

    def set_plane(self,hasPlane):
        self.__plane = hasPlane

    plane = property(get_plane,set_plane)

    def get_cylinder(self):
        return self.__cylinder

    def set_cylinder(self,hasCylinder):
        self.__cylinder = hasCylinder

    cylinder = property(get_cylinder,set_cylinder)

    def get_sphere(self):
        return self.__sphere

    def set_sphere(self,hasSphere):
        self.__sphere = hasSphere

    sphere = property(get_sphere,set_sphere)

    def get_cone(self):
        return self.__cone

    def set_cone(self,hasCone):
        self.__cone = hasCone

    cone = property(get_cone,set_cone)

    def get_toroid(self):
        return self.__toroid

    def set_toroid(self,hasToroid):
        self.__toroid = hasToroid

    toroid = property(get_toroid,set_toroid)

    def get_revsurf(self):
        return self.__revsurf

    def set_revsurf(self,hasRev):
        self.__revsurf = hasRev

    revsurf = property(get_revsurf,set_revsurf)

    def get_spline(self):
        return self.__spline

    def set_spline(self,hasSpline):
        self.__spline = hasSpline

    spline = property(get_spline,set_spline)


class TaskPanel:

    __listObj = []
    __switch = 0
    __SolidList = []

    def __init__(self):

        self.path_to_ui = Paths.modulePath() + "/CheckSurfPanel.ui"
        self.form = Gui.PySideUic.loadUi(self.path_to_ui)
        self.form.tree.setHeaderLabels(['Surface Type','Number'])
        self.form.tree.header().setResizeMode(QtGui.QHeaderView.ResizeToContents)

        self.parent_a = QtGui.QTreeWidgetItem(self.form.tree)
        self.parent_a.setText(0,"Analytic Surfaces")
        self.parent_s = QtGui.QTreeWidgetItem(self.form.tree)
        self.parent_s.setText(0,"Spline Surfaces")

        if (len(self.__listObj) != 0):
            self.__listObj[:] = []

        if (len(self.__SolidList) != 0):
            self.__SolidList[:] = []

        self.__switch = 0
        self.form.progressBar.setValue(0)

    def accept(self):
        if (self.__switch == 1):
            return
        self.__listObj[:] = []
        self.createObjects()
        self.updateTree()
        self.__switch = 1

    def createObjects(self):
        self.form.progressBar.setMaximum(len(App.ActiveDocument.Objects))
        step = 1
        for obj in App.ActiveDocument.Objects:
            step += 1
            self.form.progressBar.setValue(step)
            compt = 0
            if obj.isDerivedFrom("Part::Feature") and obj.Shape.Faces:
                solid_obj = CreateSolid(obj)
                for i in enumerate(obj.Shape.Faces):
                    face = obj.Shape.Faces[compt]
                    compt += 1                    
                    face_obj = CreateFace(face,compt,obj)
                    if (face_obj.getSurfType() == "Spline"):
                        App.Console.PrintError("{} {} is spline surfaces, please check it\n".format(obj.Label,face_obj.getSurfNum()))
                    self.findSurfType(face_obj,solid_obj)
                    self.__listObj.append(face_obj)
                self.__SolidList.append(solid_obj)

    def findSurfType(self,face_obj,solid_obj):
        if (face_obj.getSurfType() == "Plane"):
            solid_obj.plane = True
        elif (face_obj.getSurfType() == "Cylinder"):
            solid_obj.cylinder = True
        elif (face_obj.getSurfType() == "Sphere"):
            solid_obj.sphere = True
        elif (face_obj.getSurfType() == "Cone"):
            solid_obj.cone = True
        elif (face_obj.getSurfType() == "Revolution"):
            solid_obj.revsurf = True
        elif (face_obj.getSurfType() == "Toroid"):
            solid_obj.toroid = True
        elif (face_obj.getSurfType() == "Spline"):
            solid_obj.spline = True

    def updateTree(self):

        self.form.progressBar.setValue(0)

        list_a = self.form.tree.findItems("Analytic Surfaces",QtCore.Qt.MatchExactly,0)
        if len(list_a) == 1:
            self.parent_a = list_a[0]

        list_s = self.form.tree.findItems("Spline Surfaces",QtCore.Qt.MatchExactly,0)
        if len(list_s) == 1:
            self.parent_s = list_s[0]

        self.cldPln = QtGui.QTreeWidgetItem(self.parent_a)
        self.cldPln.setFlags(self.cldPln.flags()|QtCore.Qt.ItemIsUserCheckable)
        self.cldPln.setCheckState(0,QtCore.Qt.Unchecked)
        self.cldPln.setText(0,"Plane")

        self.cldCyln = QtGui.QTreeWidgetItem(self.parent_a)
        self.cldCyln.setText(0,"Cylinder")
        self.cldCyln.setFlags(self.cldCyln.flags()|QtCore.Qt.ItemIsUserCheckable)
        self.cldCyln.setCheckState(0,QtCore.Qt.Unchecked)

        self.cldSphere = QtGui.QTreeWidgetItem(self.parent_a)
        self.cldSphere.setText(0,"Sphere")
        self.cldSphere.setFlags(self.cldSphere.flags()|QtCore.Qt.ItemIsUserCheckable)
        self.cldSphere.setCheckState(0,QtCore.Qt.Unchecked)

        self.cldCone = QtGui.QTreeWidgetItem(self.parent_a)
        self.cldCone.setText(0,"Cone")
        self.cldCone.setFlags(self.cldCone.flags()|QtCore.Qt.ItemIsUserCheckable)
        self.cldCone.setCheckState(0,QtCore.Qt.Unchecked)

        self.cldToroid = QtGui.QTreeWidgetItem(self.parent_a)
        self.cldToroid.setText(0,"Toroid")
        self.cldToroid.setFlags(self.cldToroid.flags()|QtCore.Qt.ItemIsUserCheckable)
        self.cldToroid.setCheckState(0,QtCore.Qt.Unchecked)

        self.cldRev = QtGui.QTreeWidgetItem(self.parent_a)
        self.cldRev.setText(0,"Revolution surface")
        self.cldRev.setFlags(self.cldRev.flags()|QtCore.Qt.ItemIsUserCheckable)
        self.cldRev.setCheckState(0,QtCore.Qt.Unchecked)

        self.cldSpline = QtGui.QTreeWidgetItem(self.parent_s)
        self.cldSpline.setText(0,"Spline surface")
        self.cldSpline.setFlags(self.cldSpline.flags()|QtCore.Qt.ItemIsUserCheckable)
        self.cldSpline.setCheckState(0,QtCore.Qt.Unchecked)

        self.num_plane = 0;
        self.num_cylinder = 0;
        self.num_sphere = 0;
        self.num_cone = 0;
        self.num_toroid = 0;
        self.num_revolution = 0;
        self.num_spline = 0;

        self.form.progressBar.setMaximum(len(self.__listObj))
        for i in range(len(self.__listObj)):
            self.form.progressBar.setValue(i+1)
            surfType = self.__listObj[i].getSurfType()
            if (surfType == "Plane"):
                self.num_plane += 1
            elif(surfType == "Cylinder"):
                self.num_cylinder += 1
            elif(surfType == "Sphere"):
                self.num_sphere += 1
            elif(surfType == "Cone"):
                self.num_cone += 1
            elif(surfType == "Toroid"):
                self.num_toroid += 1
            elif(surfType == "Revolution"):
                self.num_revolution += 1
            elif(surfType == "Spline"):
                self.num_spline += 1

        self.cldPln.setText(1,"{}".format(self.num_plane))
        self.cldCyln.setText(1,"{}".format(self.num_cylinder))
        self.cldSphere.setText(1,"{}".format(self.num_sphere))
        self.cldCone.setText(1,"{}".format(self.num_cone))
        self.cldToroid.setText(1,"{}".format(self.num_toroid))
        self.cldRev.setText(1,"{}".format(self.num_revolution))
        self.cldSpline.setText(1,"{}".format(self.num_spline))

        self.form.tree.expandAll()
        self.form.tree.show()

    def setupUi(self):
        mw = self.getMainWindow()
        form = mw.findChild(QtGui.QWidget,"TaskPanel")        
        form.btnSurf = mw.findChild(QtGui.QPushButton,"BtnSelectSurf")
        QtCore.QObject.connect(form.btnSurf, QtCore.SIGNAL("pressed()"), self.selectSurface)
        form.btnSolid = mw.findChild(QtGui.QPushButton,"BtnSelectSolid")
        QtCore.QObject.connect(form.btnSolid, QtCore.SIGNAL("pressed()"), self.selectSolid)

    def selectSurface(self):

        self.form.progressBar.setValue(0)
        self.form.progressBar.setMaximum(len(self.__listObj))

        Gui.Selection.clearSelection()
        selObj = []
        for i in range(len(self.__listObj)):
            surfType = self.__listObj[i].getSurfType()
            self.form.progressBar.setValue(i+1)
            if (self.checkTreeItem(surfType)):
                obj = self.__listObj[i].getObject()
                num = self.__listObj[i].getSurfNum()
                Gui.Selection.addSelection(obj,num)
                selObj.append(self.__listObj[i])

    def selectSolid(self):
        if (len(self.__SolidList) == 0):
            return

        select_list = []

        self.form.progressBar.setValue(0)
        self.form.progressBar.setMaximum(len(self.__SolidList))

        for i in range(len(self.__SolidList)):
            solid = self.__SolidList[i]
            self.form.progressBar.setValue(i+1)
            obj = solid.getObject()
            if (self.checkTreeSolid(solid)):
                #Gui.Selection.addSelection(obj)
                select_list.append(obj)

        if (len(select_list) == 0):
            return

        self.form.progressBar.setValue(0)
        self.form.progressBar.setMaximum(len(App.ActiveDocument.Objects))
        step = 0
        for doc_obj in App.ActiveDocument.Objects:
            step += 1
            self.form.progressBar.setValue(step+1)
            if not doc_obj.isDerivedFrom("Part::Feature"):
                continue
            bfind = False
            for i in range(len(select_list)):
                select_solid = select_list[i]
                if (doc_obj.Name == select_solid.Name):
                    bfind = True
                    break

            if(bfind):
                Gui.ActiveDocument.getObject(doc_obj.Name).Visibility = True
            else:
                Gui.ActiveDocument.getObject(doc_obj.Name).Visibility = False


    def checkTreeSolid(self,solid):
        if (self.parent_a.childCount() == 0 and self.parent_s.childCount() == 0):
            return False
        if (self.cldPln.checkState(0) == QtCore.Qt.Checked and solid.plane):
            return True
        if (self.cldCyln.checkState(0) == QtCore.Qt.Checked and solid.cylinder):
            return True
        if (self.cldSphere.checkState(0) == QtCore.Qt.Checked and solid.sphere):
            return True
        if (self.cldCone.checkState(0) == QtCore.Qt.Checked and solid.cone):
            return True
        if (self.cldToroid.checkState(0) == QtCore.Qt.Checked and solid.toroid):
            return True
        if (self.cldRev.checkState(0) == QtCore.Qt.Checked and solid.revsurf):
            return True
        if (self.cldSpline.checkState(0) == QtCore.Qt.Checked and solid.spline):
            return True
        else:
            return False


           # colorlist = []
           #  faceindex = 0
           # idx = int(num[4:])
           # for subface in obj.Shape.Faces:
           #     faceindex += 1
           #     if faceindex == idx:
           #         colorlist.append((1.0,0.0,0.0))
           #     else:
           #         colorlist.append((1.0,1.0,1.0))
           # obj.ViewObject.DiffuseColor = colorlist

    def checkTreeItem(self,type):
        if (self.cldPln.checkState(0) == QtCore.Qt.Checked and type == "Plane"):
            return True
        if (self.cldCyln.checkState(0) == QtCore.Qt.Checked and type == "Cylinder"):
            return True
        if (self.cldSphere.checkState(0) == QtCore.Qt.Checked and type == "Sphere"):
            return True
        if (self.cldCone.checkState(0) == QtCore.Qt.Checked and type == "Cone"):
            return True
        if (self.cldToroid.checkState(0) == QtCore.Qt.Checked and type == "Toroid"):
            return True
        if (self.cldRev.checkState(0) == QtCore.Qt.Checked and type == "Revolution"):
            return True
        if (self.cldSpline.checkState(0) == QtCore.Qt.Checked and type == "Spline"):
            return True
        else:
            return False

    def getMainWindow(self):
        "returns the main window"
        # using QtGui.qApp.activeWindow() isn't very reliable because if another
        # widget than the mainwindow is active (e.g. a dialog) the wrong widget is
        # returned
        toplevel = QtGui.qApp.topLevelWidgets()
        for i in toplevel:
            if i.metaObject().className() == "Gui::MainWindow":
                return i
        raise RuntimeError("No main window found")

def createTask():
    panel = TaskPanel()
    Gui.Control.showDialog(panel)
    panel.setupUi()
    return panel
