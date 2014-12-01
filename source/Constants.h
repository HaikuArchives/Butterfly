// Some useful constants

const char version[5] = "v1.5";

// Application messages
// ====================
#define MSG_FILE_SELECTED			0x8000
#define MSG_FILE_INVOCATED			0x8001
#define MSG_AUTOSCALE_MODIFIED		0x8002
#define MSG_PATH_MODIFIED			0x8003
#define MSG_PIC_NAME_MODIFIED		0x8004
#define MSG_FULLSCREEN_MODIFIED		0x8005
#define MSG_SAVE_REQUESTED			0x8006
#define MSG_OUTPUT_FORMAT_SELECTED	0x8007
#define MSG_ZOOM_FACTOR_MODIFIED	0x8008
#define MSG_SET_CURDESKTOP_CENTERED	0x8009
#define MSG_SET_CURDESKTOP_SCALED	0x8010
#define MSG_SET_CURDESKTOP_TILED	0x8011
#define MSG_SET_ALLDESKTOP_CENTERED	0x8012
#define MSG_SET_ALLDESKTOP_SCALED	0x8013
#define MSG_SET_ALLDESKTOP_TILED	0x8014
#define MSG_ZOOM_IN					0x8015
#define MSG_ZOOM_OUT				0x8016
#define MSG_PREF_WINDOW				0x8017
#define MSG_CFG_MODIFIED			0x8018

#define MSG_PREF_CANCEL				0x8100
#define MSG_PREF_OK					0x8101
#define MSG_PREF_COLOR				0x8102
#define MSG_UPDATE_VIEW_COLOR       0x8103


// Scrolling Direction
// ===================
enum {
		SCROLL_LEFT,
		SCROLL_RIGHT,
		SCROLL_UP,
		SCROLL_DOWN
	};


const int PIC_FOLLOW_MOUSE	= 0;
const int PIC_FLEE_MOUSE	= 1;

#define DEFAULT_MOUSE_MODE PIC_FOLLOW_MOUSE
//#define DEFAULT_INFOBAR_COLOR 0xbeb3cf00
#define DEFAULT_INFOBAR_COLOR 0xF0F0F000

// Attribute names (configuration)
// ==============================
#define ATT_INFOBAR_COLOR	"infcol"
#define ATT_MOUSE_MODE		"mousemode"
#define ATT_AUTOFIT         "autofit"
#define ATT_FULLSCREEN      "fullscreen"
#define ATT_XPOS            "xpos"
#define ATT_YPOS            "ypos"
#define ATT_WIDTH           "width"
#define ATT_HEIGHT          "height"
#define ATT_LANGUAGE        "language"

#define CFG_FILENAME "/boot/home/config/settings/Butterfly_settings"

// Languages
#define ENGLISH   0
#define FRENCH    1
#define RUSSIAN   2
#define PORTUGUES 3
#define GERMAN    4
#define ITALIAN   5
#define SPANISH   6
#define BASQUE    7


