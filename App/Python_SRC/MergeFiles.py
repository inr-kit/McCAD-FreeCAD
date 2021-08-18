import FreeCAD as App
import FreeCADGui as Gui
import Part
from PySide import QtGui, QtCore
import sys
import Paths

class MergeFiles(object):

    __filename = ''

    def __init__(self,fn):
        self.__filename = fn

    def ReadFile(self):
        file = open(self.__filename)

        CellNumStart = 0
        CellNumEnd = 0
        SurfNumStart = 0
        SurfNumEnd = 0

    def getFileNames(self):
        return (self.__filename)


class TaskPanel:

    __filename1 = ""
    __filename2 = ""

    def __init__(self):

        self.path_to_ui = Paths.modulePath() + "/MergeFilesPanel.ui"
        self.form = Gui.PySideUic.loadUi(self.path_to_ui)
        #buttons = QtGui.QDialogButtonBox(QtGui.QDialogButtonBox.Ok|QtGui.QDialogButtonBox.Cancel)
        #buttons.button(QtGui.QDialogButtonBox.Ok).setText("Display")
        self.form.tree.setHeaderLabels(['Item','Parameters'])
        self.form.tree.header().setResizeMode(QtGui.QHeaderView.ResizeToContents)

    def accept(self):
        if self.__filename1 and self.__filename2:
            self.updateTree()

    def addTreeItem(self,fn,path):
        parent = QtGui.QTreeWidgetItem(self.form.tree)

        list = self.form.tree.findItems(fn,QtCore.Qt.MatchExactly,0)
        if len(list) == 1:
            parent=list[0]
            parent.setText(1,path)
        else:
            parent.setText(0,fn)
            parent.setText(1,path)

        cldCellNumStart= QtGui.QTreeWidgetItem(parent)
        cldCellNumStart.setText(0,"Start Cell No.")
        cldCellNumStart.setText(1,"1")

    def updateTree(self):
        self.form.tree.show()

    def setupUi(self):
        mw = self.getMainWindow()
        form = mw.findChild(QtGui.QWidget,"TaskPanel")
        form.fileButton1 = mw.findChild(QtGui.QPushButton,"BnFile_1")
        QtCore.QObject.connect(form.fileButton1, QtCore.SIGNAL("pressed()"), self.loadFile_1)
        form.fileButton2 = mw.findChild(QtGui.QPushButton,"BnFile_2")
        QtCore.QObject.connect(form.fileButton2, QtCore.SIGNAL("pressed()"), self.loadFile_2)

    def loadFile_1(self):
        filename = QtGui.QFileDialog.getOpenFileName(QtGui.qApp.activeWindow(),'Choose the file 1','*.txt')
        fname = filename[0].decode("utf8")
        if fname:
            self.__filename1 = fname           
            self.addTreeItem("File_1",fname)

    def loadFile_2(self):
        filename = QtGui.QFileDialog.getOpenFileName(QtGui.qApp.activeWindow(),'Choose the file 2','*.txt')
        fname = filename[0].decode("utf8")
        if fname:
            self.__filename2 = fname            
            self.addTreeItem("File_2",fname)

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
