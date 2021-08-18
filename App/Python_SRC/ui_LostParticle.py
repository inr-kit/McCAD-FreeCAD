# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'LostParticle.ui'
#
# Created: Thu Jul 12 07:47:45 2018
#      by: pyside-uic 0.2.15 running on PySide 1.2.2
#
# WARNING! All changes made in this file will be lost!

from PySide import QtCore, QtGui

class Ui_DlgParticleLost(object):
    
    __filename = ""

    def setupUi(self, DlgParticleLost):
        DlgParticleLost.setObjectName("DlgParticleLost")
        DlgParticleLost.resize(551, 672)
        self.buttonBox = QtGui.QDialogButtonBox(DlgParticleLost)
        self.buttonBox.setGeometry(QtCore.QRect(200, 630, 341, 32))
        self.buttonBox.setOrientation(QtCore.Qt.Horizontal)
        self.buttonBox.setStandardButtons(QtGui.QDialogButtonBox.Cancel|QtGui.QDialogButtonBox.Ok)
        self.buttonBox.setObjectName("buttonBox")
        self.treeView = QtGui.QTreeView(DlgParticleLost)
        self.treeView.setGeometry(QtCore.QRect(10, 110, 531, 511))
        self.treeView.setObjectName("treeView")
        self.horizontalLayoutWidget_2 = QtGui.QWidget(DlgParticleLost)
        self.horizontalLayoutWidget_2.setGeometry(QtCore.QRect(10, 60, 531, 41))
        self.horizontalLayoutWidget_2.setObjectName("horizontalLayoutWidget_2")
        self.horizontalLayout_2 = QtGui.QHBoxLayout(self.horizontalLayoutWidget_2)
        self.horizontalLayout_2.setContentsMargins(0, 0, 0, 0)
        self.horizontalLayout_2.setObjectName("horizontalLayout_2")
        self.lbFrom = QtGui.QLabel(self.horizontalLayoutWidget_2)
        self.lbFrom.setObjectName("lbFrom")
        self.horizontalLayout_2.addWidget(self.lbFrom)
        self.lEFrom = QtGui.QLineEdit(self.horizontalLayoutWidget_2)
        self.lEFrom.setObjectName("lEFrom")
        self.horizontalLayout_2.addWidget(self.lEFrom)
        self.lbto = QtGui.QLabel(self.horizontalLayoutWidget_2)
        self.lbto.setObjectName("lbto")
        self.horizontalLayout_2.addWidget(self.lbto)
        self.lEto = QtGui.QLineEdit(self.horizontalLayoutWidget_2)
        self.lEto.setObjectName("lEto")
        self.horizontalLayout_2.addWidget(self.lEto)
        self.BnDisplay = QtGui.QToolButton(self.horizontalLayoutWidget_2)
        self.BnDisplay.setMinimumSize(QtCore.QSize(150, 0))
        self.BnDisplay.setMaximumSize(QtCore.QSize(16777215, 32))
        self.BnDisplay.setObjectName("BnDisplay")
        self.horizontalLayout_2.addWidget(self.BnDisplay)
        self.horizontalLayoutWidget = QtGui.QWidget(DlgParticleLost)
        self.horizontalLayoutWidget.setGeometry(QtCore.QRect(10, 10, 531, 41))
        self.horizontalLayoutWidget.setObjectName("horizontalLayoutWidget")
        self.horizontalLayout = QtGui.QHBoxLayout(self.horizontalLayoutWidget)
        self.horizontalLayout.setContentsMargins(0, 0, 0, 0)
        self.horizontalLayout.setObjectName("horizontalLayout")
        self.BnLoadFile = QtGui.QToolButton(self.horizontalLayoutWidget)
        self.BnLoadFile.setMinimumSize(QtCore.QSize(100, 0))
        self.BnLoadFile.setMaximumSize(QtCore.QSize(2000, 32))
        self.BnLoadFile.setObjectName("BnLoadFile")
        self.horizontalLayout.addWidget(self.BnLoadFile)
        self.lineEditFile = QtGui.QLineEdit(self.horizontalLayoutWidget)
        self.lineEditFile.setMaximumSize(QtCore.QSize(16777215, 34))
        self.lineEditFile.setObjectName("lineEditFile")
        self.horizontalLayout.addWidget(self.lineEditFile)
        self.line = QtGui.QFrame(DlgParticleLost)
        self.line.setGeometry(QtCore.QRect(10, 50, 531, 16))
        self.line.setFrameShape(QtGui.QFrame.HLine)
        self.line.setFrameShadow(QtGui.QFrame.Sunken)
        self.line.setObjectName("line")

        self.retranslateUi(DlgParticleLost)
        QtCore.QObject.connect(self.BnLoadFile, QtCore.SIGNAL("pressed()"), self.loadFile)
        QtCore.QMetaObject.connectSlotsByName(DlgParticleLost)
    
    def loadFile(self):
        filename = QtGui.QFileDialog.getOpenFileName(QtGui.qApp.activeWindow(),'Open MCNP output file','*.*')
        if filename:            
            self.lineEditFile.setText(filename[0])
            self.__filename = filename[0]

    def retranslateUi(self, DlgParticleLost):
        DlgParticleLost.setWindowTitle(QtGui.QApplication.translate("DlgParticleLost", "Display Lost Particles", None, QtGui.QApplication.UnicodeUTF8))
        self.lbFrom.setText(QtGui.QApplication.translate("DlgParticleLost", "From", None, QtGui.QApplication.UnicodeUTF8))
        self.lbto.setText(QtGui.QApplication.translate("DlgParticleLost", "To", None, QtGui.QApplication.UnicodeUTF8))
        self.BnDisplay.setText(QtGui.QApplication.translate("DlgParticleLost", "Display Particles", None, QtGui.QApplication.UnicodeUTF8))
        self.BnLoadFile.setText(QtGui.QApplication.translate("DlgParticleLost", "Load File", None, QtGui.QApplication.UnicodeUTF8))

class run:
    def __init__(self):
        self.d = QtGui.QWidget()
        self.ui = Ui_DlgParticleLost()
        self.ui.setupUi(self.d)
        self.d.show()

