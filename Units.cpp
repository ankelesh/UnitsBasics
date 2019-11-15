#include <QtWidgets/QApplication>
#include "Controller/SimpliestController.h"


int main(int argc, char** argv)
{
	using namespace Model;
	QApplication app(argc, argv);



	QSurfaceFormat form;
	form.setProfile(QSurfaceFormat::OpenGLContextProfile::CompatibilityProfile);
	QSurfaceFormat::setDefaultFormat(form);
	Controller::SimpliestController c(&app);
	c.show();

	return app.exec();
}
