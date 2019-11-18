#include <QtWidgets/QApplication>
#include "Controller/SimpliestController.h"
#include "debugtrace.h"
#include "Model/Hexbattlefield/HexaGraph.h"
int main(int argc, char** argv)
{
	using namespace Model;
	QApplication app(argc, argv);



	QSurfaceFormat form;
	form.setProfile(QSurfaceFormat::OpenGLContextProfile::CompatibilityProfile);
	QSurfaceFormat::setDefaultFormat(form);
	Controller::SimpliestController c(&app);
	c.show();
	detrace_MSGIMP(testGraph());
	return app.exec();
}
