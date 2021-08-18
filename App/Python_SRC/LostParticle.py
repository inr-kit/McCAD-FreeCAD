#!/usr/bin/env python
# -*- coding: utf-8 -*-

import FreeCAD as App
import FreeCADGui as Gui
import Part
from PySide import QtGui, QtCore
import OCC
import sys,re
import ui_LostParticle
from operator import attrgetter
import Paths

class Particle(object):   

    def __init__(self,x,y,z,u,v,w,num):
        self.__x1 = x * 10
        self.__y1 = y * 10
        self.__z1 = z * 10
        self.__u = u
        self.__v = v
        self.__w = w
        self.num = num         

    def getStartPoint(self):
        return (self.__x1,self.__y1,self.__z1)

    def getEndPoint(self):
        self.__x2 = self.__x1 + self.__u * self.length
        self.__y2 = self.__y1 + self.__v * self.length
        self.__z2 = self.__z1 + self.__w * self.length
        return (self.__x2,self.__y2,self.__z2)

    def getDirection(self):
        return (self.__u,self.__v,self.__w)

    def getNum(self):
        return self.num
    
    def setLength(self,length):
        self.length = length


class OutputFile(object): 
    
    filename = ''
    listParticle = []

    def __init__(self,fn):
        self.filename = fn
        self.ReadFile()
  
    def Print(self):
        print("Lost Particle: %f,%f,%f Dir: %f,%f,%f" % (self.x,self.y,self.z,self.u,self.v,self.w))

    def ReadFile(self):    
        file = open(self.filename)
        bFind = 0        
        x,y,z,u,v,w = 0.0,0.0,0.0,0.0,0.0,0.0
        num = 0
        for line in file.readlines():
            line = line.strip()
            if line.find('lost particle no.') > -1 and line.find('event log') > -1:
                findnum = re.search(r'lost particle no\.\s*(\d+)',line)
                num = int(findnum.group(1))
                bFind = 1
            if bFind:            
                if line.find('x,y,z coordinates:') > -1:                    
                    pp = re.compile(r'(-?\d+\.\d+[Ee]?[+-]?\d*)')
                    mp = pp.findall(line)
                    if len(mp) == 3:
                        x = float('{:.8f}'.format(float(mp[0])))                    
                        y = float('{:.8f}'.format(float(mp[1])))
                        z = float('{:.8f}'.format(float(mp[2])))
                    else:
                        print ("Particle position has errors.")                
                if line.find('u,v,w direction cosines:') > -1:                   
                    bFind = 0
                    pd = re.compile(r'(-?\d+\.\d+[Ee]?[+-]?\d*)')
                    md = pd.findall(line)
                    if len(md) == 3:
                        u = float('{:.8f}'.format(float(md[0])))                    
                        v = float('{:.8f}'.format(float(md[1])))
                        w = float('{:.8f}'.format(float(md[2])))
                    else:
                        print ("Particle direction has errors.")
                    obj = Particle(x,y,z,u,v,w,num)
                    self.listParticle.append(obj)
                    x,y,z,u,v,w = 0.0,0.0,0.0,0.0,0.0,0.0

    def getStartEndNum(self):       
        if not self.listParticle:
            return (0,0)
        else:
            sorted(self.listParticle,key = attrgetter('num'))
            startNum = self.listParticle[0].getNum()
            endNum = self.listParticle[-1].getNum()
            return (startNum, endNum)
    
    def getList(self):
        return self.listParticle

class TaskPanel:

    __filename = ""
    __list = []

    def __init__(self):
        self.path_to_ui = Paths.modulePath() + "/LostParticlePanel.ui"
        self.form = Gui.PySideUic.loadUi(self.path_to_ui)
        self.form.progressBar.setValue(0)
        #buttons = QtGui.QDialogButtonBox(QtGui.QDialogButtonBox.Ok|QtGui.QDialogButtonBox.Cancel)
        #buttons.button(QtGui.QDialogButtonBox.Ok).setText("Display")

    def accept(self):
        if self.__filename:
            self.display()
        #Gui.Control.closeDialog()

    def setupUi(self):
        mw = self.getMainWindow()
        form = mw.findChild(QtGui.QWidget,"TaskPanel")
        form.fileButton = mw.findChild(QtGui.QToolButton,"BnLoadFile")
        QtCore.QObject.connect(form.fileButton, QtCore.SIGNAL("clicked()"), self.loadFile)

    def loadFile(self):
        filename = QtGui.QFileDialog.getOpenFileName(QtGui.qApp.activeWindow(),'Open MCNP output file','*.*')
        fname = filename[0].decode("utf8")
        if fname:
            self.updateView(fname)

    def display(self):
        objlist = App.ActiveDocument.findObjects("App::DocumentObjectGroup","LostParticle")
        doc = App.ActiveDocument
        if (objlist):
             doc.getObject("LostParticle").removeObjectsFromDocument()
             doc.removeObject("LostParticle")
        self.startNum = int(self.form.lineEditStart.text())
        self.endNum = int(self.form.lineEditEnd.text())

        # if the item in the tree are checked, display the particle according to the treeitems
        num_list = []
        iterator = QtGui.QTreeWidgetItemIterator(self.form.tree)
        while iterator.value():
            item = iterator.value()
            if (item.childCount() != 0  and item.checkState(0) == QtCore.Qt.Checked):
                findnum = re.search(r'no\.\s*(\d+)',item.text(0))
                pn = int(findnum.group(1))
                num_list.append(pn)
            iterator += 1

        ObjGroup = doc.addObject("App::DocumentObjectGroup","LostParticle")
        self.form.progressBar.setMaximum(len(self.__list))
        for i in range(len(self.__list)):
            num = self.__list[i].getNum()
            self.form.progressBar.setValue(i+1)
            if len(num_list):       # tree items are checked
                if num not in num_list :
                    continue
            elif num < self.startNum or num > self.endNum : # no tree items are checked
                continue

            self.__list[i].setLength(1000)
            x1,y1,z1 = self.__list[i].getStartPoint()
            x2,y2,z2 = self.__list[i].getEndPoint()
            objName = '{0}{1}'.format('lost_',self.__list[i].getNum())
            lineObj = doc.addObject("Part::Line",objName);
            ObjGroup.addObject(lineObj)
            lineObj.X1 = x1
            lineObj.Y1 = y1
            lineObj.Z1 = z1
            lineObj.X2 = x2
            lineObj.Y2 = y2
            lineObj.Z2 = z2
            lineObj.ViewObject.LineColor = (1.0,0.0,0.0,1.0)
        Gui.SendMsgToActiveView("ViewFit")

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


    def updateView(self,fn):
        self.form.textEditFile.setText(fn)
        self.__filename = fn
        self.obj = OutputFile(self.__filename)

        startnum,endnum = self.obj.getStartEndNum()
        self.form.lineEditStart.setText(str(startnum))
        self.form.lineEditEnd.setText(str(endnum))

        self.form.tree.clear()
        self.form.tree.setHeaderLabels(['Particle No.      ','Value'])
        #self.form.tree.header().setStretchLastSection(False)

        self.__list = self.obj.getList()
        for i in range(len(self.__list)):
            num = self.__list[i].getNum()
            x,y,z = self.__list[i].getStartPoint()
            u,v,w = self.__list[i].getDirection()

            parent = QtGui.QTreeWidgetItem(self.form.tree)
            parent.setFlags(parent.flags()|QtCore.Qt.ItemIsUserCheckable)
            parent.setText(0,"no. {}".format(num))
            parent.setText(1,"{}".format(num))
            parent.setCheckState(0,QtCore.Qt.Unchecked)

            childPnt = QtGui.QTreeWidgetItem(parent)
            childPnt.setText(0,"StartPoint(x,y,z)")
            childPnt.setText(1,"{0:.4E}, {1:.4E}, {2:.4E}".format(x,y,z))

            childDir = QtGui.QTreeWidgetItem(parent)
            childDir.setText(0,"Direction(u,v,w)")
            childDir.setText(1,"{0:.4E}, {1:.4E}, {2:.4E}".format(u,v,w))

        self.form.tree.header().setResizeMode(QtGui.QHeaderView.ResizeToContents)
        self.form.tree.show()

def createTask():
    panel = TaskPanel()
    Gui.Control.showDialog(panel)
    panel.setupUi()
    return panel
