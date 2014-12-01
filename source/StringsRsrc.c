#include "StringsRsrc.h"

char** StringsRsrc;

char* FrenchStrings[]  = {
	"A propos...", 
	"Quitter",
	"Ok",
	"Annuler",
	"Plein écran",
	"Zoom auto",
	"Convertir...",
	"Fond d'écran courant",
	"Tous les fonds d'écran",
	"Centré",
	"Redimensionné",
	"Mosaïque",
	"Zoomer                         +",
	"Dé-Zoomer                     -",
	"Préférences...",
	"Préférences",
	"Couleur de la barre d'état",
	"Mode du déplacement de l'image à la souris",
	"L'image suit la souris",
	"L'image fuit la souris",
	"Français",
	"Langue",
	"Vous avez changez la langue.\nCe changement ne prendra effet qu'au prochain démarrage de Butterfly.",
	"Patience...",
	"Sélection du format",
	"Format de sauvegarde",
	"Utiliser",
	"Informations sur le traducteur",
	"Préférences du traducteur",
	"Etes-vous sûr de vouloir effacer",
	"Non, surtout pas",
	"Oui, à la poubelle!"
};

char* EnglishStrings[] = {
	"About...",
	"Quit",
	"Ok",
	"Cancel",
	"FullScreen",
	"Auto fit",
	"Convert...",
	"Set Current Desktop",
	"Set All Desktops",
	"Centered",
	"Scaled to fit",
	"Tiled",
	"Zoom In                         +",
	"Zoom Out                      -",
	"Preferences...",
	"Preferences",
	"Status bar colour",
	"Pan mode",
	"Picture follows mouse",
	"Picture flees mouse",
	"English",
	"Language",
	"You have changed the language.\nThis modification will be taken in account at next launch of Butterfly.",
	"Loading...",
	"Select format",
	"Output Formats",
	"Use",
	"Translator Information",
	"Translator Settings",
	"Are you sure you want to delete",
	"No, don't do that!",
	"Do it!"
};

// These words are written in Russian using UTF code table:
char* RussianStrings[] = {
	"Инфо...",
	"Выйти",
	"Ок",
	"Отмена",
	"На весь экран",
	"Авторазмер",
	"Преобразовать...",
	"На рабочий стол",
	"На все рабочие столы",
	"По центру",
	"Растянуть",
	"Рядом",
	"Увеличить                        +", // I`ve added some spaces so that + and - are above each other (I hope)
	"Уменьшить                       -",
	"Настройки...",
	"Настройки",
	"Цвет панели информации",
	"Режим прокрутки",
	"Обычный",
	"Инвертированный",
	"Русский [RU]", // Just as I wrote in e-mail
	"Язык",
	"Вы сменили язык интерфейса.\nИзменения вступят в силу при следующем запуске Butterfly", //????
	"Загрузка...",
	"Выберите формат",
	"Форматы",
	"Преобразовать",
	"О трансляторе",
	"Установки транслятора",
	"Вы на самом деле хотите удалить",
	"Нет, я передумал",
	"Уверен!"
};

// P.S. In Russian Butterfly sounds "Babochka"


// ---------- 'About' box translation ------------//
// In English:
// BUTTERFLY picture viewer - release 1.5
// Written by Thomas THERY
// Maintained by Ben THERY
// Russian translation by Oleg Kourapov

// emails, https here
//						"Ok" button here

// ----------------------------------
// In Russian:
// графический браузер BUTTERFLY - релиз 1.5
// Автор : Thomas THERY
// Сопровождение : Ben THERY
// Перевод : Олег "2sheds" Курапов

// emails, https here (add my e-mail under yours plz)
//						"Ок" button here

char* PortuguesStrings[] = {
        "Sobre...",
        "Sair",
        "Ok",
        "Cancelar",
        "Maximizar",
        "Auto Ajuste",
        "Converter...",
        "Definir como Desktop Atual",
        "Definir como Todos os Desktops",
        "Centralizado",
        "Imagem Inteira",
        "Lado a Lado",
        "Mais Zoom                                +",
        "Menos Zoom                              -",
        "Preferências...",
        "Preferências",
        "Cor da Barra de Status",
        "Modo de Navegação",
        "Direção do Mouse",
        "Direção Oposta ao Mouse",
        "Português",
        "Idioma",
        "Idioma Alterado.\nEsta Modificação Entrará em Vigor na Próxima Vez que o Butterfly for Iniciado.",
        "Aguarde...",
        "Selecione o Filtro",
        "Filtros de Exportação",
        "Exportar",
        "Sobre o Filtro",
        "Ajustes do Filtro",
        "Tem Certeza que Deseja Apagar",
        "Não, Não Faça Isso",
        "Faça Isso!"
};

char* ItalianStrings[] = {
        "Informazioni...",
        "Esci",
        "Ok",
        "Annulla",
        "Schermo intero",
        "Auto-Adatta",
        "Converti...",
        "Sfondo del desktop attuale",
        "Sfondo di tutti i desktop",
        "Centrato",
        "Adattato",
        "Affiancato",
        "Ingrandisci                    +",
        "Diminuisci                    -",
        "Preferenze...",
        "Preferenze",
        "Colore della barra di stato",
        "Modalità di trascinamento",
        "Il mouse trascina l'immagine",
        "L'immagine segue il mouse",
        "Italiano",
        "Lingua",
        "Hai cambiato la lingua.\nQuesta modifica avrà effetto alla prossima apertura di Butterfly.",
        "Caricamento...",
        "Scegli il formato",
        "Formati finali",
        "Usa",
        "Informazioni sulla traduzione",
        "Impostazioni della traduzione",
        "Vuoi davvero cancellare",
        "No, non farlo!",
        "Fallo!"
};

char* EuskeraStrings[] = {
        "-ri buruz",
        "Irten",
        "Bai",
        "Ez",
        "Pantailla osoa",
        "Berez egokitu",
        "Itzuli...",
        "Idazleku Onetan Jarri",
        "Idazleku Denetan Jarri",
        "Erdiraturik",
        "Egokitzeko eskalaratu",
        "Mosaikoa",
        "Aunditu                         +",
        "Txikitu                           -",
        "PreferenTziak...",
        "Preferetziak",
        "Egoera barraren margoa",
        "Sagua erabiltzeko era",
        "Irudiak sagua jarraitu",
        "Irudia sagutik urrundu",
        "Euskera/Basque",
        "Hizkuntza",
        "Hizkuntza aldatu duzu.\nAldaketa ahu, Butterfly berriz irekitzen duzunean artuko da kontuan.",
        "Kargatzen...",
        "Formatua aukeratu",
        "Irteera formatuak",
        "Erabili",
        "Itzultzailearen Informazioa",
        "Itzultzailearen Preferentziak",
        "Ziur ezabatu nahi duzula",
        "Ez, ez egin hori!",
        "Egin!"

};

char* SpanishStrings[] = {
        "Acerca de...",
        "Salir",
        "Aceptar",
        "Cancelar",
        "Pantalla completa",
        "Ajuste automatico",
        "Convertir...",
        "Establecen en el escritorio actual",
        "Establecer en todos los escritorios",
        "Centrado",
        "Escalar para ajustar",
        "Mosaico",
        "Acercar                                         +",
        "Alejar                                             -",
        "Preferencias...",
        "Preferencias",
        "Color de la barra de estado",
        "Modo de uso del raton",
        "La imagen sige al raton",
        "La imagen se aleja del raton",
        "Castellano",
        "Idioma",
        "Has cambiado el idioma.\nEsta modificacion sera tenida en cuenta la proxima vez que inicies Butterfly.",
        "Cargando...",
        "Elige formato",
        "Formatos de salida",
        "Usar",
        "Informacion del traductor",
        "Preferencias del traductor",
        "Seguro que quieres borrar",
        "No, no hagas eso!",
        "Hazlo!"
};

char* GermanStrings[] = {
	"Über...",
	"Ende",
	"Ok",
	"Abbruch",
	"Vollbild",
	"Anpassen",
	"Konvertieren",
	"Als Hintergrund (dieser Workspace)",
	"Als Hintergrund (alle Workspaces)",
	"Zentriert",
	"Skalieren",
	"Kacheln",
	"Reinzoomen                                +",
	"Rauszoomen                                -",
	"Einstellungen...",
	"Einstellungen",
	"Farbe der Statusleiste",
	"Scrollmodus",
	"Bild folgt der Maus",
	"Bild gegen Mausrichtung",
	"Englisch",
	"Sprache",
	"Sie haben die Sprache gewechselt.\nDiese Änderung wird sich beim nächsten Start von Butterfly auswirken.",
	"Lade...",
	"Format wählen",
	"Ausgabeformate",
	"Benutzen",
	"Translator-Information",
	"Translator-Einstellungen",
	"Diese Datei wirklich löschen:",
	"Auf keinen Fall!",
	"Tu es!"
};

