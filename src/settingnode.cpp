#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
#include <matjson.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/LevelBrowserLayer.hpp>
#include <Geode/loader/SettingNode.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <Geode/loader/Loader.hpp>
#include <Geode/loader/Mod.hpp>

using namespace geode::prelude;

class MySettingNode : public SettingNode {
protected:
    bool init(MySettingValue* value, float width) {
        if (!SettingNode::init(value))
            return false;
        
        // You may change the height to anything, but make sure to call 
        // setContentSize!
        this->setContentSize({ width, 40.f });

        // Set up the UI. Note that Geode provides a background for the 
        // setting automatically
	auto spr = CCSprite::createWithSpriteFrameName("GJ_updateBtn_001.png");

	auto btn = CCMenuItemSpriteExtra::create(
		spr, this, menu_selector(refresh::onRefreshPress)
	);
	    
        return true;
    }

public:
    void commit() override {
        this->dispatchCommitted();
    }
    bool hasUncommittedChanges() override {
    }
    bool hasNonDefaultValue() override {
    }
    void resetToDefault() override {
    }

    static MySettingNode* create(MySettingValue* value, float width) {
        auto ret = new MySettingNode();
        if (ret && ret->init(value, width)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};
