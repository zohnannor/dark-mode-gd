#include <Geode/cocos/layers_scenes_transitions_nodes/CCScene.h>

#include <Geode/Geode.hpp>
#include <Geode/loader/SettingEvent.hpp>
#include <Geode/modify/CCLayerColor.hpp>
#include <Geode/modify/CommentCell.hpp>
#include <Geode/modify/GJChestSprite.hpp>
#include <Geode/modify/GJCommentListLayer.hpp>
#include <Geode/modify/GJScoreCell.hpp>
#include <Geode/modify/InfoLayer.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/modify/LevelPage.hpp>
#include <Geode/modify/LevelSearchLayer.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/ui/GeodeUI.hpp>

using namespace geode::prelude;

// TODO
// - chest platforms (in the basement)
// - compatibility with other mods
// - define constants?
// - ensure there is no crashes
// - gauntlet colors
// - leaderboard highlight color
// - level loading play button background
// - level name and description inputs backgrounds
// - level share description
// - lists progress bars
// - map packs progress bars
// - platformer controls settings background
// - shop colors
// - split into files?
// - text?

const ccColor3B getColorByName(std::string_view const key) {
    return Mod::get()->getSettingValue<ccColor3B>(key);
}

const int ccc3Btoint(const ccColor3B& color) {
    return color.r << 16 | color.g << 8 | color.b << 0;
};

const ccColor3B intToCcc3B(int color) {
    auto r = static_cast<GLubyte>((color & 0xff0000) >> 16);
    auto g = static_cast<GLubyte>((color & 0x00ff00) >> 8);
    auto b = static_cast<GLubyte>((color & 0x0000ff) >> 0);
    return {.r = r, .g = g, .b = b};
};

const ccColor3B darken(const ccColor3B& color) {
    auto hex = ccc3Btoint(color);
    switch (hex) {
        case 0x00FF00:
            return getColorByName("normal-mode-bar");
        case 0x00FFFF:
            return getColorByName("practice-mode-bar");
        case 0xBF723E:
            return getColorByName("icon-set-bg");
        case 0xA1582C:
            return getColorByName("even-list-item-bg");
        case 0x9C552A:
            return getColorByName("even-list-item-thin-bg");
        case 0x824021:
            return getColorByName("comment-bg");
        case 0xC2723E:
            return getColorByName("odd-list-item-bg");
        case 0x904F27:
            return getColorByName("odd-list-item-thin-bg");
        case 0x00388D:
            return getColorByName("level-search-bg");
        case 0x002762:
            return getColorByName("level-search-bar-bg");
        case 0x6C99D8:
            return getColorByName("level-search-bar-font");
        case 0x002E75:
            return getColorByName("quick-search-bg");
        case 0x00245B:
            return getColorByName("difficulty-filters-bg");
        case 0x001F4F:
            return getColorByName("length-filters-bg");
        case 0xFFC800:
            return getColorByName("reward-shine-yellow-cw");
        case 0xFF9600:
            return getColorByName("reward-shine-yellow-ccw");
        case 0x0096FF:
            return getColorByName("reward-shine-blue");
        case 0x334499:
            return getColorByName("reward-chest-bg");
        default:
            return color;
    }
};

const int darken(const int color) {
    return ccc3Btoint(darken(intToCcc3B(color)));
}

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

class $modify(CustomColorsInfoLayer, InfoLayer) {
    bool init(GJGameLevel * p0, GJUserScore * p1, GJLevelList * p2) {
        if (!InfoLayer::init(p0, p1, p2)) {
            return false;
        }

        if (auto bar = this->getChildByIDRecursive("desc-background")) {
            darkenNode(typeinfo_cast<CCNodeRGBA*>(bar));
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

class $modify(CustomGJChestSprite, GJChestSprite) {
    TodoReturn switchToState(ChestSpriteState p0, bool state) {
        GJChestSprite::switchToState(p0, state);

        // chest is closed
        if (this->getChildrenCount() < 5) {
            return;
        }

        if (auto shine = getChildOfType<CCSprite>(this, 3)) {
            darkenNode(typeinfo_cast<CCNodeRGBA*>(shine));
        }

        if (auto shine = getChildOfType<CCSprite>(this, 4)) {
            darkenNode(typeinfo_cast<CCNodeRGBA*>(shine));
        }

        if (auto glowBg = getChildOfType<CCSprite>(this, 5)) {
            darkenNode(typeinfo_cast<CCNodeRGBA*>(glowBg));
        }

        if (auto glowBg = getChildOfType<CCSprite>(this, 6)) {
            darkenNode(typeinfo_cast<CCNodeRGBA*>(glowBg));
        }

        if (auto block = getChildOfType<CCSprite>(this, 7)) {
            darkenNode(typeinfo_cast<CCNodeRGBA*>(block));
        }
    };
};

// TODO: not implemented in Geode apparently
// class $modify(CustomColorsGJScoreCell, GJScoreCell) {
//     void draw() {
//         darkenNode(getChildOfType<CCLayerColor>(this, 0));

//         GJScoreCell::draw();
//     };

//     void updateBGColor(int p0) { GJScoreCell::updateBGColor(darken(p0)); }
// };

CCMenu* settingsBtnMenu = nullptr;

struct ModSettingsPopup : public CCObject {
    void openSettings(CCObject* _obj) { openSettingsPopup(Mod::get()); };
};

$execute {
    listenForSettingChanges(
        "floating-settings-button", +[](bool floatingSettingsBtnEnabled) {
            if (floatingSettingsBtnEnabled) {
                settingsBtnMenu = CCMenu::create();

                auto winSize = CCDirector::sharedDirector()->getWinSize();

                auto sprite = CCSprite::createWithSpriteFrameName(
                    "GJ_optionsBtn_001.png");
                sprite->setScale(0.6);
                ModSettingsPopup s;
                auto settingsBtn = CCMenuItemSpriteExtra::create(
                    sprite, &s, menu_selector(ModSettingsPopup::openSettings));
                settingsBtn->setPosition({-((winSize.width / 2) - 25), 0});
                settingsBtn->setSizeMult(1.2f);
                settingsBtn->setOpacity(128);
                settingsBtn->setZOrder(-10000);
                settingsBtnMenu->addChild(settingsBtn);

                CCScene::get()->addChild(settingsBtnMenu);
                SceneManager::get()->keepAcrossScenes(settingsBtnMenu);
            } else {
                SceneManager::get()->forget(settingsBtnMenu);
            }
        });
}

$on_mod(DataSaved) {
    Mod::get()->setSettingValue<bool>("floating-settings-button", false);
}
