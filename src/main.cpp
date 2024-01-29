#include <Geode/Geode.hpp>
#include <Geode/modify/CCLayerColor.hpp>
#include <Geode/modify/CommentCell.hpp>
#include <Geode/modify/GJCommentListLayer.hpp>
#include <Geode/modify/GJScoreCell.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/modify/LevelPage.hpp>
#include <Geode/modify/LevelSearchLayer.hpp>
#include <Geode/modify/PauseLayer.hpp>

using namespace geode::prelude;

// TODO:
// - define constants
// - add settings
// - split into files?
// - exception safety
// - expand readme, about.md
// - compatibility with other mods?
// - lists progress bars
// - name input, description input
// - text?
// - level share description
// - level loading play button bg
// - player controls settings bg
// - map packs?
// - chest background
// - chest platforms
// - shop colors
// - leaderboard highlight

const int ccc3Btoint(const ccColor3B& color) {
    return color.r << 16 | color.g << 8 | color.b << 0;
};

const ccColor3B intToCcc3B(int color) {
    auto r = static_cast<GLubyte>((color & 0xff0000) >> 16);
    auto g = static_cast<GLubyte>((color & 0x00ff00) >> 8);
    auto b = static_cast<GLubyte>((color & 0x0000ff) >> 0);
    return {.r = r, .g = g, .b = b};
};

const int darken(int color) {
    switch (color) {
        case 0x944E27:
            [[fallthrough]];
        case 0x9C552A:
            [[fallthrough]];
        case 0xA1582C:
            [[fallthrough]];
        case 0xBF723E:
            return 0x222222;
        case 0x00FF00:
            return 0x008000;
        case 0x00FFFF:
            return 0x008080;
        case 0xC2723E:
        case 0x6C99D8:
            return 0x333333;
        case 0x904F27:
            return 0x323232;
        case 0x001F4F:
            [[fallthrough]];
        case 0x00245B:
            [[fallthrough]];
        case 0x002762:
            [[fallthrough]];
        case 0x002E75:
            [[fallthrough]];
        case 0x00388D:
            [[fallthrough]];
        case 0x824021:
            return 0x000000;
        default:
            return color;
    }
}

const ccColor3B darken(const ccColor3B& color) {
    return intToCcc3B(darken(ccc3Btoint(color)));
};

const ccColor4B darken(const ccColor4B& color) {
    ccColor3B color3B = {.r = color.r, .g = color.g, .b = color.b};
    auto a = color.a;
    auto darkened = darken(color3B);
    return {.r = darkened.r, .g = darkened.g, .b = darkened.b, .a = a};
};

void darkenNode(CCRGBAProtocol* rgbaNode) {
    rgbaNode->setColor(darken(rgbaNode->getColor()));
}

class $modify(CustomColorsGJCommentListLayer, GJCommentListLayer) {
    static GJCommentListLayer* create(BoomListView * p0, char const* p1,
                                      cocos2d::ccColor4B p2, float p3, float p4,
                                      bool p5) {
        auto commentListLayer =
            GJCommentListLayer::create(p0, p1, p2, p3, p4, p5);
        darkenNode(commentListLayer);
        return commentListLayer;
    };
};

class $modify(CustomColorsCommentCell, CommentCell) {
    void draw() {
        if (auto c = getChildOfType<CCLayerColor>(this, 0)) {
            darkenNode(c);
        }
        if (auto background = this->getChildByIDRecursive("background")) {
            darkenNode(typeinfo_cast<CCRGBAProtocol*>(background));
        }

        CommentCell::draw();
    };

    void updateBGColor(int p0) { CommentCell::updateBGColor(darken(p0)); }
};

class $modify(CustomColorsCCLayerColor, CCLayerColor) {
    void draw() {
        darkenNode(this);
        CCLayerColor::draw();
    };
};

class $modify(CustomColorsLevelSearchLayer, LevelSearchLayer) {
    bool init(int p0) {
        if (!LevelSearchLayer::init(p0)) {
            return false;
        }

        if (auto bg = this->getChildByIDRecursive("level-search-bg")) {
            darkenNode(typeinfo_cast<CCNodeRGBA*>(bg));
        }
        if (auto bg = this->getChildByIDRecursive("level-search-bar-bg")) {
            darkenNode(typeinfo_cast<CCNodeRGBA*>(bg));
        }
        if (auto bg = this->getChildByIDRecursive("quick-search-bg")) {
            darkenNode(typeinfo_cast<CCNodeRGBA*>(bg));
        }
        if (auto bg = this->getChildByIDRecursive("difficulty-filters-bg")) {
            darkenNode(typeinfo_cast<CCNodeRGBA*>(bg));
        }
        if (auto bg = this->getChildByIDRecursive("length-filters-bg")) {
            darkenNode(typeinfo_cast<CCNodeRGBA*>(bg));
        }

        if (auto bar = this->getChildByIDRecursive("search-bar")) {
            if (auto placeholder = getChildOfType<CCLabelBMFont>(bar, 0)) {
                darkenNode(typeinfo_cast<CCRGBAProtocol*>(placeholder));
            }
        }

        return true;
    };
};

class $modify(CustomColorsLevelInfoLayer, LevelInfoLayer) {
    bool init(GJGameLevel * p0, bool p1) {
        if (!LevelInfoLayer::init(p0, p1)) {
            return false;
        }

        if (auto bar = this->getChildByIDRecursive("normal-mode-bar")) {
            if (auto sprite = getChildOfType<CCSprite>(bar, 0)) {
                darkenNode(typeinfo_cast<CCNodeRGBA*>(sprite));
            }
        }
        if (auto bar = this->getChildByIDRecursive("practice-mode-bar")) {
            if (auto sprite = getChildOfType<CCSprite>(bar, 0)) {
                darkenNode(typeinfo_cast<CCNodeRGBA*>(sprite));
            }
        }

        return true;
    };
};

class $modify(CustomColorsPauseLayer, PauseLayer) {
    static PauseLayer* create(bool p0) {
        auto pauseLayer = PauseLayer::create(p0);

        if (auto bar = getChildOfType<CCSprite>(pauseLayer, 0)) {
            if (auto sprite = getChildOfType<CCSprite>(bar, 0)) {
                darkenNode(typeinfo_cast<CCNodeRGBA*>(sprite));
            }
        }
        if (auto bar = getChildOfType<CCSprite>(pauseLayer, 1)) {
            if (auto sprite = getChildOfType<CCSprite>(bar, 0)) {
                darkenNode(typeinfo_cast<CCNodeRGBA*>(sprite));
            }
        }

        return pauseLayer;
    };
};

class $modify(CustomColorsLevelPage, LevelPage) {
    static LevelPage* create(GJGameLevel * p0) {
        auto levelPage = LevelPage::create(p0);

        if (auto bar = getChildOfType<CCSprite>(levelPage, 0)) {
            if (auto sprite = getChildOfType<CCSprite>(bar, 0)) {
                darkenNode(typeinfo_cast<CCNodeRGBA*>(sprite));
            }
        }

        if (auto bar = getChildOfType<CCSprite>(levelPage, 1)) {
            if (auto sprite = getChildOfType<CCSprite>(bar, 0)) {
                darkenNode(typeinfo_cast<CCNodeRGBA*>(sprite));
            }
        }

        return levelPage;
    };
};

// TODO: not implemented in Geode.
// class $modify(CustomColorsGJScoreCell, GJScoreCell) {
//     void draw() {
//         darkenNode(getChildOfType<CCLayerColor>(this, 0));

//         GJScoreCell::draw();
//     };

//     void updateBGColor(int p0) { GJScoreCell::updateBGColor(darken(p0)); }
// };
