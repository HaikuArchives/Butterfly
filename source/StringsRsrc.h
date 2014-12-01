#ifndef _StringsRsrc_h_
#define _StringsRsrc_h_

enum strID {
	STR_ABOUT = 0,
	STR_QUIT,
	STR_OK,
	STR_CANCEL,
	STR_FULLSCREEN,
	STR_AUTOFIT,
	STR_CONVERT,
	STR_SET_CURRENT_BACKGRND,
	STR_SET_ALL_BACKGRND,
	STR_CENTERED,
	STR_SCALED,
	STR_TILED,
	STR_ZOOM_IN,
	STR_ZOOM_OUT,
	STR_PREFERENCES,
	STR_PREFERENCES_TITLE,
	STR_INFOBAR_COLOR,
	STR_PAN_MODE,
	STR_PICTURE_FOLLOW_MOUSE,
	STR_PICTURE_FLEE_MOUSE,
	STR_LANGUAGE,
	STR_CHOOSE_LANGUAGE,
	STR_LANG_CHANGED_WARNING,
	STR_LOADING,
	STR_SELECT_FORMAT,
	STR_OUTPUT_FORMAT,
	STR_USE,
	STR_TRANSLATOR_INFORMATION,
	STR_TRANSLATOR_SETTINGS,
	STR_DELETE_QUESTION,
	STR_DELETE_YES,
	STR_DELETE_NO
};

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

extern char* EnglishStrings[];
extern char* FrenchStrings[];
extern char* RussianStrings[];
extern char* PortuguesStrings[];
extern char* ItalianStrings[];
extern char* EuskeraStrings[];
extern char* SpanishStrings[];
extern char* GermanStrings[];

extern char** StringsRsrc;

#ifdef __cplusplus
}
#endif

#endif // _StringsRsrc_h_
