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

//auto data = Mod::get()->getSavedValue<json>("list");

void refreshFunc() {
	web::AsyncWebRequest()
    .fetch("http://projectbdash.com/api/v1/funfacts/fact/1.0.1.php")
    .json()
    .then([this](auto const& json) {
		matjson::Value json = this
		Mod::get()->setSavedValue<std::string>("list", json.dump(matjson::NO_INDENTATION));
    });
    //.expect((std::string error) {});
}

class $modify(refresh,MenuLayer) {
	void onRefreshPress(CCObject*) {
		refreshFunc();
	}
};


// Stole from gd utils because stuff on the docs wouldent work so yea
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
    SettingNode* SettingRefreshValue::createNode(float width) {
    	return SettingRefreshNode::create(this, width);
    }
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
        label->setPositionX(-130);
        auto spr = CCSprite::createWithSpriteFrameName("GJ_updateBtn_001.png");
        spr->setScale(.4F);
        auto refreshBtn = CCMenuItemSpriteExtra::create(
            playSpr,
            this,
            menu_selector(SettingRefreshNode::onRefreshBtn)
        );
        refreshBtn->setPositionX(140);
        menu->setPosition(width / 2, 18.f);
        menu->addChild(label);
        menu->addChild(refreshBtn);
        this->addChild(menu);
        return true;
    }

public:
    void onRefreshBtn(CCObject*) {
	    refreshFunc();
    }

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
$on_mod(Loaded) {
    Mod::get()->addCustomSetting<MySettingValue>("refresh-btn");
}

class $modify(funFacts, MenuLayer) {
	std::string funFact;
	
	bool init() {
		if (!MenuLayer::init()) {
			return false;
		} 
		auto menu = static_cast<cocos2d::CCMenu*>(this->getChildByID("bottom-menu"));
		auto spr = CCSprite::createWithSpriteFrameName("GJ_chatBtn_001.png");

		auto btn = CCMenuItemSpriteExtra::create(
			spr, this, menu_selector(funFacts::onFunFact)
		);


		if (!Mod::get()->setSavedValue("notFirstLaunch", true)) {
    			refreshFunc();
		}
		
		menu->addChild(btn);
		menu->updateLayout();
		return true;
	} 

	void onFunFact(CCObject*) {
		auto data = Mod::get()->getSavedValue<std::string>("list");
		auto json = matjson::parse(data);
				auto& firstObject = json[rand() % data.size()];
				std::string quote = fmt::format("{}\nBy {}", firstObject["funFact"], firstObject["userOfReq"]);
				FLAlertLayer::create(
					"Quote",
					quote,
					"OK"
				)->show();
	}
};
