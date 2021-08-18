import FreeCAD as App
import FreeCADGui as Gui
import Part
from PySide import QtGui, QtCore
from LostParticle import OutputFile
from LostParticle import Particle
import sys

 = "/home/mccad/Programme/FreeCAD-Debug/Mod/McCad/LostParticlePanel.ui"

class TaskPanel:

    __filename = ""
    __list = []

    def __init__(self):
        self.form = Gui.PySideUic.loadUi()
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

        ObjGroup = doc.addObject("App::DocumentObjectGroup","LostParticle")        
        self.form.progressBar.setMaximum(len(self.__list))
        for i in range(len(self.__list)):
            num = self.__list[i].getNum()
            self.form.progressBar.setValue(i+1)
            if num < self.startNum or num > self.endNum :
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
        self.form.tree.setHeaderLabels(['Particle No.','Value'])
        self.form.tree.header().setResizeMode(QtGui.QHeaderView.ResizeToContents)
        #self.form.tree.header().setStretchLastSection(False)        

        self.__list = self.obj.getList()
        for i in range(len(self.__list)):
            num = self.__list[i].getNum()
            x,y,z = self.__list[i].getStartPoint()
            u,v,w = self.__list[i].getDirection()

            parent = QtGui.QTreeWidgetItem(self.form.tree)
            parent.setFlags(parent.flags()|QtCore.Qt.ItemIsUserCheckable)
            parent.setText(0,"no. {}".format(num))
            parent.setCheckState(0,QtCore.Qt.Unchecked)
            
            childPnt = QtGui.QTreeWidgetItem(parent)            
            childPnt.setText(0,"StartPoint(x,y,z)")
            childPnt.setText(1,"{0:.4E}, {1:.4E}, {2:.4E}".format(x,y,z))

            childDir = QtGui.QTreeWidgetItem(parent)            
            childDir.setText(0,"Direction(u,v,w)")
            childDir.setText(1,"{0:.4E}, {1:.4E}, {2:.4E}".format(u,v,w))
        
        self.form.tree.show()        

def createTask():
    panel = TaskPanel()
    Gui.Control.showDialog(panel)
    panel.setupUi()
    return panel 
