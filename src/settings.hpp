#pragma once
#include <Geode/loader/SettingNode.hpp>
using namespace geode::prelude;


// Stole from gd utils because stuff on the docs wouldent work so yea
// Thanks Jouca and Firee
class SettingRefreshValue;

class SettingRefreshValue : public SettingValue {
protected:
    std::string m_placeholder;
public:
    SettingRefreshValue(std::string const& key, std::string const& modID, std::string const& placeholder)
      : SettingValue(key, modID), m_placeholder(placeholder) {}

    bool load(matjson::Value const& json) override {
        return true;
    }
    bool save(matjson::Value& json) const override {
        return true;
    }
    SettingNode* createNode(float width) override;
};


class SettingRefreshNode : public SettingNode {
protected:
 bool init(SettingRefreshValue* value, float width) {
        if (!SettingNode::init(value))
            return false;
        this->setContentSize({ width, 35.f });
        auto menu = CCMenu::create();
        auto label = CCLabelBMFont::create("Refresh Quotes", "bigFont.fnt");
        label->setScale(.6F);
        label->setPositionX(-75);
        auto spr = CCSprite::createWithSpriteFrameName("GJ_updateBtn_001.png");
        spr->setScale(.525F);
        auto refreshBtn = CCMenuItemSpriteExtra::create(
            spr,
            this,
            menu_selector(SettingRefreshNode::onRefreshBtn)
        );
        refreshBtn->setPositionX(142);
        menu->setPosition(width / 2, 18.f);
        menu->addChild(label);
        menu->addChild(refreshBtn);
        this->addChild(menu);
        return true;
    }

public:
    void onRefreshBtn(CCObject*);

    void commit() override {
        this->dispatchCommitted();
    }

    bool hasUncommittedChanges() override {
        return false;
    }


    bool hasNonDefaultValue() override {
        return true;
    }
	    
    void resetToDefault() override {

    }
    static SettingRefreshNode* create(SettingRefreshValue* value, float width) {
        auto ret = new SettingRefreshNode;
        if (ret && ret->init(value, width)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};
// end of stealing from GDUtils THANKS!
