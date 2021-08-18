# McCad gui init module
# (c) 2001 Juergen Riegel LGPL

class McCadWorkbench ( Workbench ):
	"McCad workbench object"
	MenuText = "McCad"
	ToolTip = "McCad workbench"
	def Initialize(self):
		# load the module
		import McCadGui
	def GetClassName(self):
		return "McCadGui::Workbench"

Gui.addWorkbench(McCadWorkbench())
