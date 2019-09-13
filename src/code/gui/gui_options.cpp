//
// Created by screemer on 2019-01-24.
//

#include "gui_options.h"
#include "../util.h"
#include "gui.h"
#include "../lang.h"
#include "../environment.h"

using namespace std;

//*******************************
// GuiOptions::getOption
//*******************************
string GuiOptions::getOption(const vector<string> & list, const string & current, bool next) {
    int pos = 0;
    for (int i = 0; i < list.size(); i++) {
        if (list[i] == current) {
            pos = i;
        }
    }

    if (next) {
        pos++;
        if (pos >= list.size()) {
            pos = list.size() - 1;
        }
    } else {
        pos--;
        if (pos < 0) pos = 0;
    }

    return list[pos];
}

//*******************************
// GuiOptions::init
//*******************************
void GuiOptions::init() {
    shared_ptr<Lang> lang(Lang::getInstance());

    autobleemUIThemes.clear();
    menuThemes.clear();
    menuThemes.push_back("default");

    string uiThemePath = Env::getPathToUIThemeDir();
    DirEntries uiThemeFolders = DirEntry::diru_DirsOnly(uiThemePath);
    for (const DirEntry & entry : uiThemeFolders) {
        if (DirEntry::exists(uiThemePath + sep + entry.name + sep + "theme.ini")) {
            autobleemUIThemes.push_back(entry.name);
        }
    }

    string menuThemePath = Env::getPathToMenuThemesDir();
    DirEntries menuThemeFolders = DirEntry::diru_DirsOnly(menuThemePath);
    for (const DirEntry & entry : menuThemeFolders) {
        if (DirEntry::exists(menuThemePath + sep + entry.name + sep + "images")) {
            menuThemes.push_back(entry.name);
        }
    }
    pcsx.clear();
    pcsx.push_back("original");
    pcsx.push_back("bleemsync");
    mip.clear();
    mip.push_back("true");
    mip.push_back("false");
    nomusic.clear();
    nomusic.push_back("true");
    nomusic.push_back("false");
    autoregion.clear();
    autoregion.push_back("false");
    autoregion.push_back("true");
    retroarch.clear();
    retroarch.push_back("false");
    retroarch.push_back("true");
    adv.clear();
    adv.push_back("false");
    adv.push_back("true");
    quickboot.clear();
    quickboot.push_back("false");
    quickboot.push_back("true");
    languages.clear();
    languages = lang->getListOfLanguages();
    ui.clear();
    ui.push_back("classic");
    ui.push_back("EvolutionUI");
    aspect.clear();
    aspect.push_back("false");
    aspect.push_back("true");
    origames.clear();
    origames.push_back("false");
    origames.push_back("true");
    jewels.clear();
    jewels.push_back("none");
    jewels.push_back("default");

    DirEntries folders = DirEntry::diru_FilesOnly(Env::getWorkingPath() + sep + "evoimg/frames");
    for (const DirEntry & entry : folders) {
        if (DirEntry::getFileExtension(entry.name) == "png") {
            jewels.push_back(entry.name);
        }
    }
    quickmenu.clear();
    quickmenu.push_back("UI");
    quickmenu.push_back("RetroArch");
    music.clear();
    music.push_back("--");
    folders = DirEntry::diru_FilesOnly(Env::getWorkingPath() + sep + "music");
    for (const DirEntry & entry:folders) {
        if (DirEntry::getFileExtension(entry.name) == "ogg") {
            music.push_back(entry.name);
        }
    }
    for (int i=0; i <= 20; ++i) {
        showingtimeout.push_back(to_string(i));
    }
}

#define CFG_LANG           0
#define CFG_THEME          1
#define CFG_MENUTH         2
#define CFG_UI             3
#define CFG_JEWEL          4
#define CFG_MUSIC          5
#define CFG_ORIGAMES       6
#define CFG_ASPECT         7
#define CFG_QUICK          8
#define CFG_QUICKMENU      9
#define CFG_BGM            10
#define CFG_MIP            11
#define CFG_RA             12
#define CFG_ADV            13
#define CFG_SHOWINGTIMEOUT 14
#define CFG_LAST           14

//*******************************
// GuiOptions::getBooleanIcon
//*******************************
string GuiOptions::getBooleanIcon(const string & input) {
    shared_ptr<Gui> gui(Gui::getInstance());
    string value = gui->cfg.inifile.values[input];
    if (input == "quick") {
        if (value == "true") return gui->cfg.inifile.values["delay"] + "s  " + "|@Check|"; else return "|@Uncheck|";
    }

    if ((input != "nomusic") && (input != "mip")) {
        if (value == "true") return "|@Check|"; else return "|@Uncheck|";
    } else {
        if (value != "true") return "|@Check|"; else return "|@Uncheck|";
    }
}

//*******************************
// GuiOptions::renderOptionLine
//*******************************
void GuiOptions::renderOptionLine(const string & text, int pos, int offset) {
    shared_ptr<Gui> gui(Gui::getInstance());
    string fg = gui->themeData.values["text_fg"];
    int height = gui->renderTextLineOptions(text, pos, offset, POS_LEFT);
    totalHeight += height;

    if (selOption + 1 == pos) {
        SDL_Rect rect2;
        rect2.x = atoi(gui->themeData.values["opscreenx"].c_str());
        rect2.y = atoi(gui->themeData.values["opscreeny"].c_str());
        rect2.w = atoi(gui->themeData.values["opscreenw"].c_str());
        rect2.h = atoi(gui->themeData.values["opscreenh"].c_str());

        SDL_Rect rectSelection;
        rectSelection.x = rect2.x + 5;
        rectSelection.y = offset + totalHeight;
        rectSelection.w = rect2.w - 10;
        rectSelection.h = height;

        SDL_SetRenderDrawColor(renderer, gui->getR(fg), gui->getG(fg), gui->getB(fg), 255);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_RenderDrawRect(renderer, &rectSelection);
    }
}

//*******************************
// GuiOptions::render
//*******************************
void GuiOptions::render() {
    shared_ptr<Gui> gui(Gui::getInstance());
    gui->renderBackground();
    gui->renderTextBar();
    int offset = gui->renderLogo(true);
    totalHeight = 0;
    gui->renderTextLine("-=" + _("Configuration") + "=-", 0, offset, POS_CENTER);
    renderOptionLine(_("Language:") + " " + gui->cfg.inifile.values["language"], CFG_LANG + 1, offset);
    renderOptionLine(_("AutoBleem Theme:") + " " + gui->cfg.inifile.values["theme"], CFG_THEME + 1, offset);
    renderOptionLine(_("Menu Theme:") + " " + gui->cfg.inifile.values["stheme"], CFG_MENUTH + 1, offset);
    renderOptionLine(_("UI:") + " " + gui->cfg.inifile.values["ui"], CFG_UI + 1, offset);
    renderOptionLine(_("Cover Style:") + " " + gui->cfg.inifile.values["jewel"], CFG_JEWEL + 1, offset);
    renderOptionLine(_("Music:") + " " + gui->cfg.inifile.values["music"], CFG_MUSIC + 1, offset);
    renderOptionLine(_("Internal Games:") + " " + getBooleanIcon("origames"), CFG_ORIGAMES + 1, offset);
    renderOptionLine(_("Widescreen:") + " " + getBooleanIcon("aspect"), CFG_ASPECT + 1, offset);
    renderOptionLine(_("QuickBoot:") + " " + getBooleanIcon("quick"), CFG_QUICK + 1, offset);
    renderOptionLine(_("QuickBoot Init:") + " " + gui->cfg.inifile.values["quickmenu"], CFG_QUICKMENU + 1, offset);
    renderOptionLine(_("Background Music:") + " " + getBooleanIcon("nomusic"), CFG_BGM + 1, offset);
    gui->cfg.inifile.values["autoregion"] = "true"; // removing this as an option - not needed - just set to true
    renderOptionLine(_("GFX Filter:") + " " + getBooleanIcon("mip"), CFG_MIP + 1, offset);
    renderOptionLine(_("Show RetroArch:") + " " + getBooleanIcon("retroarch"), CFG_RA + 1, offset);
    renderOptionLine(_("Advanced:") + " " + getBooleanIcon("adv"), CFG_ADV + 1, offset);
    renderOptionLine(_("Showing Timeout (0 = no timeout):") + " " + gui->cfg.inifile.values["showingtimeout"], CFG_SHOWINGTIMEOUT + 1, offset);
    gui->renderStatus("|@X| " + _("OK") + "     " + "|@O| " + _("Cancel") + "|");

    //   gui->renderSelectionBox(selOption+1,offset);
    SDL_RenderPresent(renderer);
}

// local routine
//*******************************
// findNumberOfMatchingCharsFromBeginning
//*******************************
static int findNumberOfMatchingCharsFromBeginning(const string &_s1, const string &_s2) {
    string s1 = ReturnLowerCase(_s1);
    string s2 = ReturnLowerCase(_s2);
    int minLength = s1.size();
    if (s2.size() < minLength)
        minLength = s2.size();
    int matchingLength = 0;
    for (int i = 0; i < minLength; ++i) {
        if (s1[i] == s2[i])
            ++matchingLength;
        else
            break;  // chars don't match
    }
    return matchingLength;
};

// local routine
//*******************************
// bestMenuThemeMatch
//*******************************
static tuple<int, string> bestMenuThemeMatch(const string UITheme, const vector<string> &menuThemes) {
    int bestMatchingSize = 0;
    string bestMatchingString = "";
    for (auto menuTheme : menuThemes) {
        int matchSize = findNumberOfMatchingCharsFromBeginning(UITheme, menuTheme);
        if (matchSize > bestMatchingSize) {
            bestMatchingSize = matchSize;
            bestMatchingString = menuTheme;
        }
    }
    return make_tuple(bestMatchingSize, bestMatchingString);
}

//*******************************
// GuiOptions::loop
//*******************************
void GuiOptions::loop() {
    shared_ptr<Gui> gui(Gui::getInstance());
    shared_ptr<Lang> lang(Lang::getInstance());

    render();

    bool waitForButtonToBeReleased = true;
    bool menuVisible = true;
    while (menuVisible) {
        gui->watchJoystickPort();
        SDL_Event e;

        if (waitForButtonToBeReleased) {
            if (SDL_PollEvent(&e) && e.type == SDL_KEYDOWN)
                continue;   // wait until the button that was pressed is released
            else
                waitForButtonToBeReleased = false;
        }

        if (SDL_PollEvent(&e)) {
            if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.scancode == SDL_SCANCODE_SLEEP) {
                    gui->drawText(_("POWERING OFF... PLEASE WAIT"));
                    Util::powerOff();
                }
            }
            // this is for pc Only
            if (e.type == SDL_QUIT) {
                menuVisible = false;
            }

            switch (e.type) {
                case SDL_JOYBUTTONDOWN:  /* Handle Joystick Button Presses */
                    if (e.jbutton.button == gui->_cb(PCS_BTN_CIRCLE, &e)) {
                        Mix_PlayChannel(-1, gui->cancel, 0);
                        string cfg_path = Env::getWorkingPath() + sep + "config.ini";
                        gui->cfg.inifile.load(cfg_path);    // restore the original config.ini settings
                        lang->load(gui->cfg.inifile.values["language"]);    // restore the original lang
                        gui->loadAssets();                                  // restore original themes
                        gui->overrideQuickBoot = true;
                        menuVisible = false;
                        exitCode = -1;
                    };

                    if (e.jbutton.button == gui->_cb(PCS_BTN_CROSS, &e)) {
                        Mix_PlayChannel(-1, gui->cancel, 0);
                        gui->cfg.save();
                        gui->overrideQuickBoot = true;
                        menuVisible = false;
                        exitCode = 0;
                    };
                    break;

                case SDL_JOYAXISMOTION:  /* Handle Joystick Motion */
                case SDL_JOYHATMOTION:

                    int device;
                    if (e.type==SDL_JOYHATMOTION)
                    {
                        device = e.jhat.which;
                    } else
                    {
                        device = e.jaxis.which;
                    }
                    if (gui->mapper.isUp(&e)) {
                        Mix_PlayChannel(-1, gui->cursor, 0);
                        selOption--;
                        if (selOption < 0) {
                            selOption = 0;
                        }
                        render();
                    }

                    if (gui->mapper.isDown(&e)) {
                        Mix_PlayChannel(-1, gui->cursor, 0);
                        selOption++;
                        if (selOption > CFG_LAST) {
                            selOption = CFG_LAST;
                        }
                        render();
                    }

                    if (gui->mapper.isRight(&e)) {
                        Mix_PlayChannel(-1, gui->cursor, 0);
                        if (selOption == CFG_LANG) {
                            string nextValue = getOption(languages, gui->cfg.inifile.values["language"], true);
                            gui->cfg.inifile.values["language"] = nextValue;
                            lang->load(nextValue);
                            init();
                        }

                        if (selOption == CFG_THEME) {
                            string nextValue = getOption(autobleemUIThemes, gui->cfg.inifile.values["theme"], true);
                            gui->cfg.inifile.values["theme"] = nextValue;

                            int bestMatchingSize = 0;
                            string bestMatchingString;
                            tie(bestMatchingSize, bestMatchingString) = bestMenuThemeMatch(nextValue, menuThemes);
                            if (bestMatchingSize >= 3)
                                gui->cfg.inifile.values["stheme"] = bestMatchingString;

                            init();
                            gui->loadAssets();
                        }

                        if (selOption == CFG_MENUTH) {
                            string nextValue = getOption(menuThemes, gui->cfg.inifile.values["stheme"], true);
                            gui->cfg.inifile.values["stheme"] = nextValue;
                            init();
                            gui->loadAssets();
                        }

                        if (selOption == CFG_BGM) {
                            string nextValue = getOption(nomusic, gui->cfg.inifile.values["nomusic"], true);
                            gui->cfg.inifile.values["nomusic"] = nextValue;
                            init();
                            gui->loadAssets();
                        }

                        if (selOption == CFG_MUSIC) {
                            string nextValue = getOption(music, gui->cfg.inifile.values["music"], true);
                            gui->cfg.inifile.values["music"] = nextValue;
                            init();
                            gui->loadAssets();
                        }

                        if (selOption == CFG_MIP) {
                            string nextValue = getOption(mip, gui->cfg.inifile.values["mip"], true);
                            gui->cfg.inifile.values["mip"] = nextValue;
                        }

                        if (selOption == CFG_UI) {
                            string nextValue = getOption(ui, gui->cfg.inifile.values["ui"], true);
                            gui->cfg.inifile.values["ui"] = nextValue;
                        }

                        if (selOption == CFG_RA) {
                            string nextValue = getOption(retroarch, gui->cfg.inifile.values["retroarch"], true);
                            gui->cfg.inifile.values["retroarch"] = nextValue;
                        }

                        if (selOption == CFG_ADV) {
                            string nextValue = getOption(adv, gui->cfg.inifile.values["adv"], true);
                            gui->cfg.inifile.values["adv"] = nextValue;
                        }

                        if (selOption == CFG_ASPECT) {
                            string nextValue = getOption(aspect, gui->cfg.inifile.values["aspect"], true);
                            gui->cfg.inifile.values["aspect"] = nextValue;
                        }

                        if (selOption == CFG_ORIGAMES) {
                            string nextValue = getOption(origames, gui->cfg.inifile.values["origames"], true);
                            gui->cfg.inifile.values["origames"] = nextValue;
                        }

                        if (selOption == CFG_JEWEL) {
                            string nextValue = getOption(jewels, gui->cfg.inifile.values["jewel"], true);
                            gui->cfg.inifile.values["jewel"] = nextValue;
                        }

                        if (selOption == CFG_QUICKMENU) {
                            string nextValue = getOption(quickmenu, gui->cfg.inifile.values["quickmenu"], true);
                            gui->cfg.inifile.values["quickmenu"] = nextValue;
                        }

                        if (selOption == CFG_QUICK) {
                            string nextValue = getOption(quickboot, gui->cfg.inifile.values["quick"], true);
                            string last = gui->cfg.inifile.values["quick"];
                            gui->cfg.inifile.values["quick"] = nextValue;
                            int delay = atoi(gui->cfg.inifile.values["delay"].c_str());
                            delay++;
                            if (last == "false") delay = 1;
                            gui->cfg.inifile.values["delay"] = to_string(delay);
                        }

                        if (selOption == CFG_SHOWINGTIMEOUT) {
                            string nextValue = getOption(showingtimeout, gui->cfg.inifile.values["showingtimeout"], true);
                            gui->cfg.inifile.values["showingtimeout"] = nextValue;
                        }

                        render();
                    }

                    if (gui->mapper.isLeft(&e)) {
                        Mix_PlayChannel(-1, gui->cursor, 0);
                        if (selOption == CFG_LANG) {
                            string nextValue = getOption(languages, gui->cfg.inifile.values["language"], false);
                            gui->cfg.inifile.values["language"] = nextValue;
                            lang->load(nextValue);
                            init();
                        }

                        if (selOption == CFG_THEME) {
                            string nextValue = getOption(autobleemUIThemes, gui->cfg.inifile.values["theme"], false);
                            gui->cfg.inifile.values["theme"] = nextValue;

                            int bestMatchingSize = 0;
                            string bestMatchingString;
                            tie(bestMatchingSize, bestMatchingString) = bestMenuThemeMatch(nextValue, menuThemes);
                            if (bestMatchingSize >= 3)
                                gui->cfg.inifile.values["stheme"] = bestMatchingString;

                            init();
                            gui->loadAssets();
                        }

                        if (selOption == CFG_MENUTH) {
                            string nextValue = getOption(menuThemes, gui->cfg.inifile.values["stheme"], false);
                            gui->cfg.inifile.values["stheme"] = nextValue;
                            init();
                            gui->loadAssets();
                        }

                        if (selOption == CFG_MUSIC) {
                            string nextValue = getOption(music, gui->cfg.inifile.values["music"], false);
                            gui->cfg.inifile.values["music"] = nextValue;
                            init();
                            gui->loadAssets();
                        }

                        if (selOption == CFG_BGM) {
                            string nextValue = getOption(nomusic, gui->cfg.inifile.values["nomusic"], false);
                            gui->cfg.inifile.values["nomusic"] = nextValue;
                            init();
                            gui->loadAssets();
                        }

                        if (selOption == CFG_MIP) {
                            string nextValue = getOption(mip, gui->cfg.inifile.values["mip"], false);
                            gui->cfg.inifile.values["mip"] = nextValue;
                        }

                        if (selOption == CFG_UI) {
                            string nextValue = getOption(ui, gui->cfg.inifile.values["ui"], false);
                            gui->cfg.inifile.values["ui"] = nextValue;
                        }

                        if (selOption == CFG_RA) {
                            string nextValue = getOption(retroarch, gui->cfg.inifile.values["retroarch"], false);
                            gui->cfg.inifile.values["retroarch"] = nextValue;
                        }

                        if (selOption == CFG_ADV) {
                            string nextValue = getOption(adv, gui->cfg.inifile.values["adv"], false);
                            gui->cfg.inifile.values["adv"] = nextValue;
                        }

                        if (selOption == CFG_ASPECT) {
                            string nextValue = getOption(aspect, gui->cfg.inifile.values["aspect"], false);
                            gui->cfg.inifile.values["aspect"] = nextValue;
                        }

                        if (selOption == CFG_ORIGAMES) {
                            string nextValue = getOption(origames, gui->cfg.inifile.values["origames"], false);
                            gui->cfg.inifile.values["origames"] = nextValue;
                        }

                        if (selOption == CFG_JEWEL) {
                            string nextValue = getOption(jewels, gui->cfg.inifile.values["jewel"], false);
                            gui->cfg.inifile.values["jewel"] = nextValue;
                        }

                        if (selOption == CFG_QUICKMENU) {
                            string nextValue = getOption(quickmenu, gui->cfg.inifile.values["quickmenu"], false);
                            gui->cfg.inifile.values["quickmenu"] = nextValue;
                        }

                        if (selOption == CFG_QUICK) {
                            string nextValue = getOption(quickboot, gui->cfg.inifile.values["quick"], false);
                            int delay = atoi(gui->cfg.inifile.values["delay"].c_str());
                            delay++;
                            gui->cfg.inifile.values["delay"] = to_string(1);
                            gui->cfg.inifile.values["quick"] = nextValue;
                        }

                        if (selOption == CFG_SHOWINGTIMEOUT) {
                            string curValue =gui->cfg.inifile.values["showingtimeout"];
                            string nextValue = getOption(showingtimeout, curValue, false);
                            if (curValue != "0")
                                gui->cfg.inifile.values["showingtimeout"] = nextValue;
                        }

                        render();
                    }
                    break;
            }
        }
    }
}