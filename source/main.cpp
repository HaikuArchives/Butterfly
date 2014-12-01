#include <InterfaceKit.h>
#include "InterApp.h"





// Create application object and start it
int main()
{
//	srand(real_time_clock());
	InterApp *the_app = new InterApp();
	the_app->Run();
	delete the_app;
	return 0;
}
