#include <InterfaceKit.h>
#include <storage/Path.h>
#include "InterApp.h"
#include "BFPrefs.h"
#include "Constants.h"
#include <stdio.h>	//DEBUG
#include <unistd.h>

//----------------------------------------------------------------------------------
InterApp::InterApp() : BApplication(APP_SIGNATURE),	nav(NULL) {
	initialFile[0]=0;	
	// Read preferences
	prefs = new BFPrefs();
	prefs->Read();
		
	// Open Main window
	theWin = new InterWindow();
	nav = new FSNav(theWin, initialFile);
}

//----------------------------------------------------------------------------------
InterApp::~InterApp() 
{
	// Save attributes
	prefs->Write();
}

//----------------------------------------------------------------------------------
void InterApp::ArgvReceived(int32 argc, char **argv)
{	
	entry_ref ref;
	
	if (argc>1)
	{
		// Forward message to the window
		BMessage* message = new BMessage(B_SIMPLE_DATA);
		BEntry* entry = new BEntry(argv[1]);
		entry->GetRef(&ref);
		message->AddRef("refs",&ref);
		theWin->PostMessage(message);	
		
		delete entry;	
	}		
}


//----------------------------------------------------------------------------------
void InterApp::RefsReceived(BMessage *message)
{
	// Forward message to the window
	message->what = B_SIMPLE_DATA;
	theWin->PostMessage(message);	
}


//----------------------------------------------------------------------------------
void InterApp::AboutRequested()
{
	char aboutStr[2048];
	if (prefs->GetLanguage() == FRENCH) {
		sprintf(aboutStr, "BUTTERFLY visionneur d'images - version %s\n"
		                  "Ecrit par Thomas THERY \n"
	    	              "Maintenu par Ben THERY \n"
	    	              "\n"
	    	              "Traduction russe par Oleg \"2sheds\" Kourapov \n"
	    	              "Traduction brésilienne par Leonardo Rota-Rossi (BUG-RJ)\n"
	    	              "Traduction italienne par Mario Lupi\n"
	    	              "Traduction espagnole et basque par Xabier Urrutia\n"
	    	              "Traduction allemande par Finn Bastiansen\n"
	        	          "\n"
	                	  "emails : dragfly_kj@rocketmail.com\n"
		                  "           bthery@chez.com\n"
		                  "           2sheds@newmail.ru\n"
		                  "           bug-rj@bug-br.org.br\n"
		                  "           Esaltato@libero.it\n"
		                  "           ehiza@bigfoot.com\n"
		                  "           finn@bastiansen.de\n"           
		                  "\nhttp://www.chez.com/bthery/butterfly\n"
		                  "\n"
		                  B_UTF8_COPYRIGHT"2000-2001\n", APP_VERSION);
	}else if (prefs->GetLanguage() == RUSSIAN) {
		sprintf(aboutStr, "графический браузер BUTTERFLY - релиз %s\n"
		                  "Автор : Thomas THERY \n"
	    	              "Сопровождение : Ben THERY \n"
	    	              "\n"
	    	              "Перевод : Олег \"2sheds\" Курапов \n"
	        	          "\n"
	                	  "emails : dragfly_kj@rocketmail.com\n"
		                  "           bthery@chez.com\n"
		                  "           2sheds@newmail.ru\n"
		                  "           bug-rj@bug-br.org.br\n"
		                  "           Esaltato@libero.it\n"
		                  "\nhttp://www.chez.com/bthery/butterfly\n"
		                  "\n"
		                  B_UTF8_COPYRIGHT"2000-2001\n", APP_VERSION);
	}else if (prefs->GetLanguage() == PORTUGUES) {
		sprintf(aboutStr, "Visualizador de Imagens BUTTERFLY - release %s\n"
		                  "Escrito por Thomas THERY \n"
	    	              "Mantido por Ben THERY \n"
	    	              "\n"
	    	              "Tradução Russa por Oleg \"2sheds\" Kourapov\n"
	    	              "Tradução para Português-Brasileiro por BeOS User Group - Rio de Janeiro/Brasil (BUG-RJ)\n"
	        	          "\n"
	                	  "e-mails : dragfly_kj@rocketmail.com\n"
		                  "           bthery@chez.com\n"
		                  "           2sheds@newmail.ru\n"
		                  "           bug-rj@bug-br.org.br\n"
		                  "           Esaltato@libero.it\n"
		                  "\nhttp://www.chez.com/bthery/butterfly\n"
		                  "\n"
		                  B_UTF8_COPYRIGHT"2000-2001\n", APP_VERSION); 
	} else if (prefs->GetLanguage() == ITALIAN) {
		sprintf(aboutStr, "BUTTERFLY Visualizzatore d'immagini - versione %s\n"
		                  "Scritto da Thomas THERY \n"
	    	              "Curato da Ben THERY \n"
	    	              "\n"
	    	              "Traduzione russa di Oleg \"2sheds\" Kourapov \n"
	    	              "Traduzione braziliana di Leonardo Rota-Rossi (BUG-RJ)\n"
	    	              "Traduzione italiana di Mario Lupi \n"
	        	          "\n"
	                	  "e-mails : dragfly_kj@rocketmail.com\n"
		                  "           bthery@chez.com\n"
		                  "           2sheds@newmail.ru\n"
		                  "           bug-rj@bug-br.org.br\n"
		                  "           Esaltato@libero.it\n"
		                  "\nhttp://www.chez.com/bthery/butterfly\n"
		                  "\n"
		                  B_UTF8_COPYRIGHT"2000-2001 \n", APP_VERSION);
	} else if (prefs->GetLanguage() == SPANISH) {
		sprintf(aboutStr, "Visualizador de imagenes BUTTERFLY - version %s\n"
						"Escrito by Thomas THERY\n"
						"Mantenido by Ben THERY\n"
						"\n"
						"Traduccion al ruso por Oleg \"2sheds\" Kourapov\n"
						"Traduccion al portuges por Leonardo Rota-Rossi (BUG-RJ)\n"
						"Traduccion al italiano por Mario Lupi \n"
						"Traduccion al castellano por Xabier Urrutia\n"
						"Traduccion al euskera por Xabier Urrutia\n"
	        	          "\n"
	                	  "e-mails : dragfly_kj@rocketmail.com\n"
		                  "           bthery@chez.com\n"
		                  "           2sheds@newmail.ru\n"
		                  "           bug-rj@bug-br.org.br\n"
		                  "           Esaltato@libero.it\n"
		                  "           ehiza@bigfoot.com\n"
		                  "\nhttp://www.chez.com/bthery/butterfly\n"
		                  "\n"
		                  B_UTF8_COPYRIGHT"2000-2001\n", APP_VERSION);
	} else if (prefs->GetLanguage() == BASQUE) {
		sprintf(aboutStr, "BUTTERFLY irudi ikuskaria - %s bertsioa\n"
						"Idazlea: Thomas THERY\n"
						"Mantentzailea: Ben THERY\n"
	        	          "\n"
						"Errusierar itzulpena: Oleg \"2sheds\" Kourapov\n"
						"Portugalderar itzulpena: Leonardo Rota-Rossi (BUG-RJ)\n"
						"Italierar itzulpena: Mario Lupi \n"
						"Gaztelerar itzulpena: Xabier Urrutia\n"
						"Euskerara itzulpena: Xabier Urrutia\n"
	        	          "\n"
	                	  "e-mails : dragfly_kj@rocketmail.com\n"
		                  "           bthery@chez.com\n"
		                  "           2sheds@newmail.ru\n"
		                  "           bug-rj@bug-br.org.br\n"
		                  "           Esaltato@libero.it\n"
		                  "           ehiza@bigfoot.com\n"
		                  "\nhttp://www.chez.com/bthery/butterfly\n"
		                  "\n"
		                  B_UTF8_COPYRIGHT"2000-2001\n", APP_VERSION);		
	} else if (prefs->GetLanguage() == GERMAN) {
		sprintf(aboutStr, "BUTTERFLY Picture Viewer - Release %s\n" 
						  "Geschrieben von Thomas THERY \n" 
						  "Gepflegt von Ben THERY \n" 
						  "\n" 
						  "Russische Übersetzung von Oleg \"2sheds\" Kourapov \n" 
						  "Brasilianische Übersetzung von Leonardo Rota-Rossi (BUG-RJ)\n" 
						  "Italienische Übersetzung von Mario Lupi\n" 
						  "Euskera und Spanish Übersetzung von Xabier Urrutia\n" 
						  "Deutsche Übersetzung von Finn Bastiansen\n" 
						  "\n" 
						  "e-mails : dragfly_kj@rocketmail.com\n" 
						  " 		bthery@chez.com\n" 
						  " 		2sheds@newmail.ru\n" 
						  " 		bug-rj@bug-br.org.br\n" 
						  " 		Esaltato@libero.it\n" 
						  " 		ehiza@bigfoot.com\n" 
						  " 		finn@bastiansen.de\n" 
						  "\nhttp://www.chez.com/bthery/butterfly\n" 
						  "\n" 
		                  B_UTF8_COPYRIGHT"2000-2001\n", APP_VERSION);
	} else {
		sprintf(aboutStr, "BUTTERFLY picture viewer - release %s\n"
		                  "Written by Thomas THERY \n"
	    	              "Maintained by Ben THERY \n"
	    	              "\n"
	    	              "Russian translation by Oleg \"2sheds\" Kourapov \n"
	    	              "Brasilian translation by Leonardo Rota-Rossi (BUG-RJ)\n"
	    	              "Italian translation by Mario Lupi\n"
	    	              "Euskera and Spanish translation by Xabier Urrutia\n"
	    	              "German translation by Finn Bastiansen\n"	    	              
	        	          "\n"
	                	  "e-mails : dragfly_kj@rocketmail.com\n"
		                  "           bthery@chez.com\n"
		                  "           2sheds@newmail.ru\n"
		                  "           bug-rj@bug-br.org.br\n"
		                  "           Esaltato@libero.it\n"
		                  "           ehiza@bigfoot.com\n"
		                  "           finn@bastiansen.de\n"           
		                  "\nhttp://www.chez.com/bthery/butterfly\n"
		                  "\n"
		                  B_UTF8_COPYRIGHT"2000-2001\n", APP_VERSION);
	}
	                               
	BAlert *the_alert = new BAlert("About...", aboutStr, "OK");
	the_alert->Go();
}

